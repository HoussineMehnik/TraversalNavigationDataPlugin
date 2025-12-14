//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#include "TraversalNavDataEditorSubsystem.h"

#include "Detour/DetourCommon.h"
#include "Detour/DetourNavMesh.h"
#include "Editor.h"
#include "Editor/EditorEngine.h"
#include "EditorBuildUtils.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Landscape.h"
#include "LandscapeProxy.h"
#include "Misc/ScopedSlowTask.h"
#include "NavigationSystem.h"
#include "NavMesh/RecastHelpers.h"
#include "NavMesh/RecastNavMesh.h"
#include "TraversalNavSegmentGenerationValidator_Default.h"
#include "TraversalNavData.h"
#include "TraversalNavDataFunctionLibrary.h"
#include "TraversalNavDataSettings.h"
#include "TraversalNavDataSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TraversalNavDataEditorSubsystem)

DEFINE_LOG_CATEGORY(LogTraversalNavDataEditor);

#define LOCTEXT_NAMESPACE "TraversalNavDataEditorSubsystem"

UTraversalNavDataEditorSubsystem::UTraversalNavDataEditorSubsystem()
{
}

void UTraversalNavDataEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);	
}

void UTraversalNavDataEditorSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UTraversalNavDataEditorSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Editor;
}

TStatId UTraversalNavDataEditorSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTraversalNavDataEditorSubsystem, STATGROUP_Tickables);
}

void UTraversalNavDataEditorSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		if (bIsBuildingStarted)
		{
			UE_LOG(LogTraversalNavDataEditor, Warning, TEXT("%s: Traversal NavData build failed due to an invalid world."), ANSI_TO_TCHAR(__FUNCTION__));
			FinishBuild();
		}
		return;
	}

	if (!UNavigationSystemV1::IsNavigationBeingBuilt(World) && bIsBuildingStarted)
	{
		GenerateTraversalNavSegments();
		FinishBuild();
	}
}

void UTraversalNavDataEditorSubsystem::BuildTraversalNavData()
{
	if (bIsBuildingStarted)
	{
		UE_LOG(LogTraversalNavDataEditor, Warning, TEXT("%s: Trying to trigger traversal nav building while a build is already active."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	const UTraversalNavDataSettings* TraversalNavDataSettings = GetDefault<UTraversalNavDataSettings>();
	if (TraversalNavDataSettings == nullptr)
	{
		UE_LOG(LogTraversalNavDataEditor, Warning, TEXT("%s: Traversal NavData build failed due to invalid TraversalNavDataSettings."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTraversalNavDataEditor, Warning, TEXT("%s: Traversal NavData build failed due to missing world."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(World);
	if (NavSys == nullptr)
	{
		UE_LOG(LogTraversalNavDataEditor, Warning, TEXT("%s: Traversal NavData build failed due to missing NavigationSystem."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	DefaultNavMeshPtr = Cast<ARecastNavMesh>(NavSys->GetDefaultNavDataInstance());
	if (!DefaultNavMeshPtr.IsValid())
	{
		UE_LOG(LogTraversalNavDataEditor, Warning, TEXT("%s: Traversal NavData build failed due to missng DefaultNavMesh."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	if (TraversalNavDataSettings->bExcludeLandscapesWhileGenerating)
	{
		for (TActorIterator<ALandscape> LandscapeIt(World, ALandscape::StaticClass()); LandscapeIt; ++LandscapeIt)
		{
			if (ALandscape* Landscape = *LandscapeIt)
			{
				if (Landscape->bUsedForNavigation)
				{
					Landscape->bUsedForNavigation = false;
					FPropertyChangedEvent PropertyChangedEvent(ALandscapeProxy::StaticClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(ALandscapeProxy, bUsedForNavigation)));
					Landscape->PostEditChangeProperty(PropertyChangedEvent);
					ModifiedLandsacpes.Add(Landscape);
				}
			}
		}
	}

	NavMeshCachedState.SaveAndApplySettings(*DefaultNavMeshPtr.Get(), *TraversalNavDataSettings);
	FEditorBuildUtils::EditorBuild(World, FBuildOptions::BuildAIPaths);

	bIsBuildingStarted = true;
}

void UTraversalNavDataEditorSubsystem::FinishBuild()
{
	for (const TWeakObjectPtr<ALandscape>& LandscapePtr : ModifiedLandsacpes)
	{
		if (ALandscape* Landscape = LandscapePtr.Get())
		{
			Landscape->bUsedForNavigation = true;
			FPropertyChangedEvent PropertyChangedEvent(ALandscapeProxy::StaticClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(ALandscapeProxy, bUsedForNavigation)));
			Landscape->PostEditChangeProperty(PropertyChangedEvent);
		}
	}

	if (ARecastNavMesh* DefaultNavMesh = DefaultNavMeshPtr.Get())
	{
		NavMeshCachedState.Restore(*DefaultNavMesh);
	}
	
	bIsBuildingStarted = false;
	OnnTraversalNavDataDataBuildDone.Broadcast();
}

void UTraversalNavDataEditorSubsystem::GenerateTraversalNavSegments()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTraversalNavDataEditor, Warning, TEXT("%s: Traversal NavData build failed due to an invalid world."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	const UTraversalNavDataSettings* TraversalNavDataSettings = GetDefault<UTraversalNavDataSettings>();
	if (TraversalNavDataSettings == nullptr)
	{
		UE_LOG(LogTraversalNavDataEditor, Warning, TEXT("%s: Traversal NavData build failed due to an invalid TraversalNavDataSettings."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	if (!DefaultNavMeshPtr.IsValid())
	{
		UE_LOG(LogTraversalNavDataEditor, Warning, TEXT("%s: Traversal NavData build failed due to missng DefaultNavMesh."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
	const dtNavMesh* DetourMesh = DefaultNavMeshPtr->GetRecastMesh();
	if (!DetourMesh)
	{
		UE_LOG(LogTraversalNavDataEditor, Warning, TEXT("%s: Traversal NavData build failed due to invalid navmesh."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	UTraversalNavDataSubsystem* TraversalNavDataSubsystem = UWorld::GetSubsystem<UTraversalNavDataSubsystem>(World);
	ATraversalNavData* TraversalNavData = TraversalNavDataSubsystem->GetOrCreateTraversalNavData();

	TraversalNavData->Modify();
	TraversalNavData->GetTraversalNavSegmentsMutable().Empty();

	const UTraversalNavSegmentGenerationValidator_Default* DefaultSegmentValidator = nullptr;
	if (!TraversalNavDataSettings->bDisableDefaultValidator)
	{
		DefaultSegmentValidator = GetDefault<UTraversalNavSegmentGenerationValidator_Default>();

#if ENABLE_DRAW_DEBUG
		if (!TraversalNavDataSettings->bDisableDefaultValidator && TraversalNavDataSettings->bShowDefaultValidatorTraceChecks)
		{
			FlushPersistentDebugLines(World);
		}
#endif // ENABLE_DRAW_DEBUG
	}

	const UTraversalNavSegmentGenerationValidator_Base* AdditionaltSegmentValidator = nullptr;
	if (TSubclassOf<UTraversalNavSegmentGenerationValidator_Base> AdditionalValidatorClass = TraversalNavDataSettings->AdditionalValidatorClassPtr.LoadSynchronous())
	{
		AdditionaltSegmentValidator = AdditionalValidatorClass.GetDefaultObject();
	}

	const float EdgeMinLengthSqr = FMath::Square(TraversalNavDataSettings->MinSegmentLength);
	const int32 TileNum = DetourMesh->getMaxTiles();
	FScopedSlowTask GenerateTraversalNavSegmentsTask(TileNum, LOCTEXT("%s: GenerateTraversalNavSegments", "Generate Traversal Segments :"));
	GenerateTraversalNavSegmentsTask.MakeDialog(true);

	for (int32 TileIdx = 0; TileIdx < TileNum; ++TileIdx)
	{
		GenerateTraversalNavSegmentsTask.EnterProgressFrame(1, FText::FromString(TEXT("%s: Generate Traversal Segments : Tile [") + FString::FromInt(TileIdx) + TEXT("%s: ]")));
		const dtMeshTile* Tile = DetourMesh->getTile(TileIdx);
		if (!Tile || !Tile->header)
		{
			continue;
		}

		// Iterate all polygons in tile
		for (int32 PolyIdx = 0; PolyIdx < Tile->header->polyCount; ++PolyIdx)
		{
			const dtPoly* Poly = &Tile->polys[PolyIdx];
			if (Poly->vertCount < 3)
			{
				continue;
			}

			FVector PolyNormal = FVector::UpVector;
			{
				if (Poly->vertCount >= 3)
				{
					// Get first three vertices to calculate triangle normal
					const dtReal* V0 = &Tile->verts[Poly->verts[0] * 3];
					const dtReal* V1 = &Tile->verts[Poly->verts[1] * 3];
					const dtReal* V2 = &Tile->verts[Poly->verts[2] * 3];

					FVector P0 = Recast2UnrealPoint(V0);
					FVector P1 = Recast2UnrealPoint(V1);
					FVector P2 = Recast2UnrealPoint(V2);

					FVector Edge1 = P1 - P0;
					FVector Edge2 = P2 - P0;
					PolyNormal = -FVector::CrossProduct(Edge1, Edge2).GetSafeNormal();
				}
			}

			// Iterate all edges in polygon
			for (int32 VertIdx = 0; VertIdx < Poly->vertCount; ++VertIdx)
			{
				// If no neighbor on this edge, it's external
				if (Poly->neis[VertIdx] == 0)
				{
					int32 Index1 = Poly->verts[VertIdx];
					int32 Index2 = Poly->verts[(VertIdx + 1) % Poly->vertCount];

					const dtReal* V1 = &Tile->verts[Index1 * 3];
					const dtReal* V2 = &Tile->verts[Index2 * 3];

					const FVector EdgeStart = Recast2UnrealPoint(V1);
					const FVector EdgeEnd = Recast2UnrealPoint(V2);
					FVector EndToStart = (EdgeEnd - EdgeStart);

					if (EndToStart.SquaredLength() < EdgeMinLengthSqr)
					{
						continue;
					}

					const FVector EdgeDir = EndToStart.GetSafeNormal();
					const FVector EdgeNormal = FVector::CrossProduct(PolyNormal, EdgeDir).GetSafeNormal();

					if (DefaultSegmentValidator != nullptr && !DefaultSegmentValidator->ValidateNavMeshEdge(EdgeStart, EdgeEnd, EdgeNormal, PolyNormal))
					{
						continue;
					}

					if (AdditionaltSegmentValidator != nullptr && !AdditionaltSegmentValidator->ValidateNavMeshEdge(EdgeStart, EdgeEnd, EdgeNormal, PolyNormal))
					{
						continue;
					}

					FTraversalNavSegmentData NewSegment;
					{
						NewSegment.Start = EdgeStart;
						NewSegment.End = EdgeEnd;
						NewSegment.Binormal = EdgeNormal;
					}

					if (DefaultSegmentValidator != nullptr)
					{
						DefaultSegmentValidator->PostProcessTraversalNavSegment(NewSegment);
					}

					if (AdditionaltSegmentValidator != nullptr)
					{
						AdditionaltSegmentValidator->PostProcessTraversalNavSegment(NewSegment);
					}

					TraversalNavData->GetTraversalNavSegmentsMutable().Add(MoveTemp(NewSegment));
				}
			}
		}
	}

	TraversalNavData->MarkComponentsRenderStateDirty();

	if (TraversalNavData->GetTraversalNavSegments().IsEmpty())
	{
		UE_LOG(LogTraversalNavDataEditor, Warning, TEXT("%s: Traversal NavData build done but no data generated due navmesh generation fail."), ANSI_TO_TCHAR(__FUNCTION__));
	}
	else
	{
		UE_LOG(LogTraversalNavDataEditor, Log, TEXT("%s: Traversal NavData build succeeded."), ANSI_TO_TCHAR(__FUNCTION__));
	}
}

void UTraversalNavDataEditorSubsystem::FRecastNavMeshCachedState::SaveAndApplySettings(ARecastNavMesh& NavMesh, const UTraversalNavDataSettings& TraversalNavDataSettings)
{
	if (bIsSet)
	{
		return;
	}

	bIsSet = true;

	// Cache
	AgentRadius = NavMesh.AgentRadius;
	AgentHeight = NavMesh.AgentHeight;
	AgentMaxSlope = NavMesh.AgentMaxSlope;
	AgentMaxStepHeight = NavMesh.NavMeshResolutionParams[(uint8)ENavigationDataResolution::Default].AgentMaxStepHeight;
	CellHeight = NavMesh.NavMeshResolutionParams[(uint8)ENavigationDataResolution::Default].CellHeight;
	CellSize = NavMesh.NavMeshResolutionParams[(uint8)ENavigationDataResolution::Default].CellSize;
	SimplificationElevationRatio = NavMesh.SimplificationElevationRatio;

	// Apply settings
	NavMesh.AgentRadius = TraversalNavDataSettings.VoxelizationCellSize;
	NavMesh.AgentHeight = TraversalNavDataSettings.CharacterHeight;
	NavMesh.AgentMaxSlope = TraversalNavDataSettings.CharacterMaxSlope;
	NavMesh.NavMeshResolutionParams[(uint8)ENavigationDataResolution::Default].AgentMaxStepHeight = TraversalNavDataSettings.VoxelizationCellSize;
	NavMesh.NavMeshResolutionParams[(uint8)ENavigationDataResolution::Default].CellHeight = TraversalNavDataSettings.VoxelizationCellSize;
	NavMesh.NavMeshResolutionParams[(uint8)ENavigationDataResolution::Default].CellSize = TraversalNavDataSettings.VoxelizationCellSize;
	NavMesh.SimplificationElevationRatio = TraversalNavDataSettings.SimplificationElevationRatio;
}

void UTraversalNavDataEditorSubsystem::FRecastNavMeshCachedState::Restore(ARecastNavMesh& NavMesh)
{
	if (!bIsSet)
	{
		return;
	}

	NavMesh.AgentRadius = AgentRadius;
	NavMesh.AgentHeight = AgentHeight;
	NavMesh.AgentMaxSlope = AgentMaxSlope;
	NavMesh.NavMeshResolutionParams[(uint8)ENavigationDataResolution::Default].AgentMaxStepHeight = AgentMaxStepHeight;
	NavMesh.NavMeshResolutionParams[(uint8)ENavigationDataResolution::Default].CellHeight = CellHeight;
	NavMesh.NavMeshResolutionParams[(uint8)ENavigationDataResolution::Default].CellSize = CellSize;
	NavMesh.SimplificationElevationRatio = SimplificationElevationRatio;
	FPropertyChangedEvent PropertyChangedEvent(ARecastNavMesh::StaticClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(ARecastNavMesh, AgentRadius)));
	NavMesh.PostEditChangeProperty(PropertyChangedEvent);
	bIsSet = false;
}

#undef LOCTEXT_NAMESPACE