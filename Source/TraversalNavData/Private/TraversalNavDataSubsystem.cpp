//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#include "TraversalNavDataSubsystem.h"

#include "DrawDebugHelpers.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "TraversalNavData.h"
#include "TraversalNavDataFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TraversalNavDataSubsystem)

ATraversalNavData* UTraversalNavDataSubsystem::GetOrCreateTraversalNavData() const
{
	if (!TraversalNavDataPtr.IsValid())
	{
		UWorld* World = GetWorld();

		for (TActorIterator<ATraversalNavData> It(World); It; ++It)
		{
			ATraversalNavData* TraversalNavData = (*It);
			if (IsValid(TraversalNavData))
			{
				TraversalNavDataPtr = TraversalNavData;
				return TraversalNavData;
			}
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.OverrideLevel = World->PersistentLevel;
		SpawnParams.ObjectFlags &= ~RF_Transactional;
		ATraversalNavData* TraversalNavData = World->SpawnActor<ATraversalNavData>(SpawnParams);

		if (TraversalNavData != nullptr)
		{
			TraversalNavData->SetFlags(RF_Transactional);
			TraversalNavDataPtr = TraversalNavData;
			return TraversalNavData;
		}
	}

	return TraversalNavDataPtr.Get();
}

void UTraversalNavDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UTraversalNavDataSubsystem::Deinitialize()
{
	ClearOctree();
	Super::Deinitialize();
}

void UTraversalNavDataSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	const EWorldType::Type WorldType = InWorld.WorldType;
	if (WorldType == EWorldType::Editor || WorldType == EWorldType::Inactive)
	{
		return;
	}

	InitOctree();
}

TStatId UTraversalNavDataSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTraversalNavDataSubsystem, STATGROUP_Tickables);
}

void UTraversalNavDataSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ProcessPendingRuntimeSegments();
}

void UTraversalNavDataSubsystem::InitOctree()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UTraversalNavDataSubsystem::InitOctree);

	const ATraversalNavData* TraversalNavData = GetOrCreateTraversalNavData();
	check(TraversalNavData );

	const TArray<FTraversalNavSegmentData>& TraversalNavSegments = TraversalNavData ->GetTraversalNavSegments();
	for (const FTraversalNavSegmentData& TraversalNavSegment : TraversalNavSegments)
	{
		Octree.AddElement(FTraversalNavDataOctreeElement(TraversalNavSegment));
	}
}

void UTraversalNavDataSubsystem::ClearOctree()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UTraversalNavDataSubsystem::ClearOctree);

	Octree.Destroy();
}

void UTraversalNavDataSubsystem::ProcessPendingRuntimeSegments()
{
	FScopeLock Lock(&Mutex);
	if (PendingAddRuntimeSegments.IsEmpty())
	{
		return;
	}

	TRACE_CPUPROFILER_EVENT_SCOPE(UTraversalNavDataSubsystem::ProcessPendingRuntimeSegments);

	for (const FTraversalNavSegmentData& TraversalNavSegment : PendingAddRuntimeSegments)
	{
		Octree.AddElement(FTraversalNavDataOctreeElement(TraversalNavSegment));
	}

	PendingAddRuntimeSegments.Reset();
}

void UTraversalNavDataSubsystem::AddUserDefinedTraversalSegment(const FTraversalNavSegmentData& TraversalSegment)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UTraversalNavDataSubsystem::AddUserDefinedTraversalSegment);
	FScopeLock Lock(&Mutex);
	PendingAddRuntimeSegments.Add(TraversalSegment);
}

bool UTraversalNavDataSubsystem::FindTraversalSegments(TArray<FTraversalNavSegmentData>& OutSegments, const FVector& QueryBoxOrigin, const FRotator& QueryBoxRotation, const FVector& QueryBoxExtent, bool bDrawDebug, float DrawTime)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UTraversalNavDataSubsystem::FindTraversalSegments);

	const FBox TransformedBox = FBox(-QueryBoxExtent, QueryBoxExtent).TransformBy(FTransform(QueryBoxRotation, QueryBoxOrigin, FVector::One()));
	OutSegments.Empty();
	const FBoxCenterAndExtent BoxBounds(TransformedBox);
	Octree.FindElementsWithBoundsTest(BoxBounds, [&OutSegments](const FTraversalNavDataOctreeElement& Element)
		{
			OutSegments.Add(*Element.TraversalNavSegmentPtr);
		});

#if ENABLE_DRAW_DEBUG
	if (bDrawDebug)
	{
		UWorld* World = GetWorld();
		DrawDebugBox(World, QueryBoxOrigin, QueryBoxExtent, QueryBoxRotation.Quaternion(), FColor::White, false, DrawTime);
		for (const FTraversalNavSegmentData& TraversalNavSegment : OutSegments)
		{
			UTraversalNavDataFunctionLibrary::DebugDrawTraversalSegment(World, TraversalNavSegment, DrawTime);
		}
	}
#endif // ENABLE_DRAW_DEBUG

	return !OutSegments.IsEmpty();
}

