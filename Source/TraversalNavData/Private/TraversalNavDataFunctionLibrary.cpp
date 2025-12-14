//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#include "TraversalNavDataFunctionLibrary.h"

#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "TraversalNavData.h"
#include "TraversalNavDataRenderingComponent.h"
#include "TraversalNavDataSettings.h"
#include "TraversalNavDataSubsystem.h"
#include "TraversalNavDataTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TraversalNavDataFunctionLibrary)

FVector UTraversalNavDataFunctionLibrary::GetTraversalSegmentNormal(const FTraversalNavSegmentData& TraversalSegment)
{
	return TraversalSegment.GetNormal();
}

FVector UTraversalNavDataFunctionLibrary::GetTraversalSegmentDirection(const FTraversalNavSegmentData& TraversalSegment)
{
	return TraversalSegment.GetDirection();
}

FVector UTraversalNavDataFunctionLibrary::GetTraversalSegmentMidPoint(const FTraversalNavSegmentData& TraversalSegment)
{
	return TraversalSegment.GetMidPoint();
}

float UTraversalNavDataFunctionLibrary::GetTraversalSegmentLength(const FTraversalNavSegmentData& TraversalSegment)
{
	return TraversalSegment.GetLength();
}

FVector UTraversalNavDataFunctionLibrary::FindClosestPointOnTraversalSegment(const FTraversalNavSegmentData& TraversalSegment, const FVector& Point, float ClampOffset /*= 0.0f*/)
{
	if (ClampOffset > 0.0f)
	{
		const float SegmentLength = (TraversalSegment.Start, TraversalSegment.End).Size();
		const float SafeClampOffset = FMath::Min(ClampOffset, SegmentLength * 0.5f);
		return FMath::ClosestPointOnSegment(Point, TraversalSegment.Start + TraversalSegment.GetDirection() * SafeClampOffset, TraversalSegment.End - TraversalSegment.GetDirection() * SafeClampOffset);
	}

	return FMath::ClosestPointOnSegment(Point, TraversalSegment.Start, TraversalSegment.End);
}

void UTraversalNavDataFunctionLibrary::ToggleTraversalNavigationDataDebug(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World == nullptr)
	{
		return;
	}

	UTraversalNavDataSubsystem* TraversalNavDataSubsystem = UWorld::GetSubsystem<UTraversalNavDataSubsystem>(World);
	if (TraversalNavDataSubsystem == nullptr)
	{
		return;
	}

	ATraversalNavData* TraversalNavData = TraversalNavDataSubsystem->GetOrCreateTraversalNavData();
	if (TraversalNavData == nullptr)
	{
		return;
	}

	UTraversalNavDataRenderingComponent* RenderingComponent = TraversalNavData->GetRenderingComponent();
	RenderingComponent->bEnableDrawing = !RenderingComponent->bEnableDrawing;

	RenderingComponent->MarkRenderStateDirty();
}

void UTraversalNavDataFunctionLibrary::DebugDrawTraversalSegment(const UObject* WorldContextObject, const FTraversalNavSegmentData& TraversalSegment, float DrawTime /*= 0.0f*/)
{
#if ENABLE_DRAW_DEBUG
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World == nullptr)
	{
		return;
	}

	const UTraversalNavDataSettings* TraversalNavDataSettings = GetDefault<UTraversalNavDataSettings>();
	if (TraversalNavDataSettings == nullptr)
	{
		return;
	}

	const FColor LineColor = TraversalNavDataSettings->GetTraversalNavSegmentColor(TraversalSegment.Tag);
	const float DirectionLineLength = TraversalNavDataSettings->VoxelizationCellSize + 10.0f;
	const FVector MidPoint = (TraversalSegment.Start + TraversalSegment.End) * 0.5f;
	DrawDebugLine(World, TraversalSegment.Start, TraversalSegment.End, LineColor, false, DrawTime, SDPG_World, 2.0f);
	DrawDebugLine(World, MidPoint, MidPoint + TraversalSegment.Binormal * DirectionLineLength, LineColor, SDPG_World, 0);
	DrawDebugLine(World, MidPoint, MidPoint + TraversalSegment.GetNormal() * DirectionLineLength, LineColor, SDPG_World, 0);
#endif // ENABLE_DRAW_DEBUG
}

FLinearColor UTraversalNavDataFunctionLibrary::GetTraversalSegmentColor(const FTraversalNavSegmentData& TraversalSegment)
{
	if (const UTraversalNavDataSettings* TraversalNavDataSettings = GetDefault<UTraversalNavDataSettings>())
	{
		return TraversalNavDataSettings->GetTraversalNavSegmentColor(TraversalSegment).ReinterpretAsLinear();
	}
	return FColor::Magenta.ReinterpretAsLinear();
}

FLinearColor UTraversalNavDataFunctionLibrary::GetTraversalSegmentColorByTag(const FGameplayTag Tag)
{
	if (const UTraversalNavDataSettings* TraversalNavDataSettings = GetDefault<UTraversalNavDataSettings>())
	{
		return TraversalNavDataSettings->GetTraversalNavSegmentColor(Tag).ReinterpretAsLinear();
	}
	return FColor::Magenta.ReinterpretAsLinear();
}
