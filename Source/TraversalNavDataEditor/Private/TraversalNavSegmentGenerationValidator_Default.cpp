//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#include "TraversalNavSegmentGenerationValidator_Default.h"

#include "CollisionQueryParams.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "TraversalNavDataSettings.h"
#include "TraversalNavDataTypes.h"
#include "KismetTraceUtils.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TraversalNavSegmentGenerationValidator_Default)


namespace FTraversalDefaultValidatorHelpers
{
	static const FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(TraversalNavSegmentGenerationValidatorTrace), true);
	static const FLinearColor TraceColor = FLinearColor::Green;
	static const FLinearColor TraceHitColor = FLinearColor::Red;

	bool LineTrace(const UWorld& World, FHitResult& OutHit, const FVector& Start, const FVector& End, const UTraversalNavDataSettings& TraversalNavDataSettings)
	{
		bool const bHit = World.LineTraceSingleByChannel(OutHit, Start, End, TraversalNavDataSettings.DefaultValidatorTraceChannel, TraceParams);
		
#if ENABLE_DRAW_DEBUG
		if (TraversalNavDataSettings.bShowDefaultValidatorTraceChecks)
		{
			const FLinearColor& Color = bHit ? TraceHitColor : TraceColor;
			DrawDebugLineTraceSingle(&World, Start, End, EDrawDebugTrace::ForDuration, bHit, OutHit, Color, Color, TraversalNavDataSettings.DefaultValidatorDebugDrawTime);
		}
#endif

		return bHit;
	}
}

bool UTraversalNavSegmentGenerationValidator_Default::ValidateNavMeshEdge(const FVector& EdgeStart, const FVector& EdgeEnd, const FVector& EdgeBinormal, const FVector& EdgeNormal) const
{
	const UTraversalNavDataSettings* TraversalNavDataSettings = GetDefault<UTraversalNavDataSettings>();
	if (TraversalNavDataSettings == nullptr)
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return false;
	}

	const FVector MidPoint = (EdgeStart + EdgeEnd) * 0.5f;
	const ECollisionChannel DefaultValidatorTraceChannel = TraversalNavDataSettings->DefaultValidatorTraceChannel;
	const float AgentRadius = FMath::Max(TraversalNavDataSettings->CharacterRadius, TraversalNavDataSettings->VoxelizationCellSize);
	const float AgentHeight = FMath::Max(TraversalNavDataSettings->CharacterHeight, TraversalNavDataSettings->VoxelizationCellSize);
	FHitResult HitResult(ForceInit);
	FVector TraceStart, TraceEnd;

	// Horizontal Check 1
	{
		TraceStart = MidPoint + EdgeNormal * AgentRadius * 0.5f;
		TraceEnd = TraceStart - EdgeBinormal * AgentRadius;
		if (FTraversalDefaultValidatorHelpers::LineTrace(*World, HitResult, TraceStart, TraceEnd, *TraversalNavDataSettings))
		{
			return false;
		}

		if (FTraversalDefaultValidatorHelpers::LineTrace(*World, HitResult, TraceEnd, TraceStart, *TraversalNavDataSettings))
		{
			return false;
		}
	}

	// Horizontal Check 2
	{
		TraceStart = MidPoint + EdgeNormal * AgentRadius * 0.5f;
		TraceEnd = TraceStart + EdgeBinormal * AgentRadius;
		if (FTraversalDefaultValidatorHelpers::LineTrace(*World, HitResult, TraceStart, TraceEnd, *TraversalNavDataSettings))
		{
			return false;
		}

		if (FTraversalDefaultValidatorHelpers::LineTrace(*World, HitResult, TraceEnd, TraceStart, *TraversalNavDataSettings))
		{
			return false;
		}
	}

	// Vertical Check Down
	{
		TraceStart = TraceEnd;
		TraceEnd = TraceStart - EdgeNormal * AgentRadius * 1.5f;
		if (FTraversalDefaultValidatorHelpers::LineTrace(*World, HitResult, TraceStart, TraceEnd, *TraversalNavDataSettings))
		{
			return false;
		}

		if (FTraversalDefaultValidatorHelpers::LineTrace(*World, HitResult, TraceEnd, TraceStart, *TraversalNavDataSettings))
		{
			return false;
		}
	}

	// Vertical Check Up
	{
		TraceStart = TraceEnd;
		TraceEnd = TraceStart + EdgeNormal * AgentHeight * 1.5f;
		if (FTraversalDefaultValidatorHelpers::LineTrace(*World, HitResult, TraceStart, TraceEnd, *TraversalNavDataSettings))
		{	
			return false;
		}

		if (FTraversalDefaultValidatorHelpers::LineTrace(*World, HitResult, TraceEnd, TraceStart, *TraversalNavDataSettings))
		{		
			return false;
		}
	}

	return true;
}

void UTraversalNavSegmentGenerationValidator_Default::PostProcessTraversalNavSegment(FTraversalNavSegmentData& TraversalSegment) const
{
	// Default Tag
	TraversalSegment.Tag = TraversalSegment_Default;

	// Adjust 
	if (const UTraversalNavDataSettings* TraversalNavDataSettings = GetDefault<UTraversalNavDataSettings>())
	{
		const float AdjustmentOffset = FMath::Max(0.1f, TraversalNavDataSettings->VoxelizationCellSize - 0.5f);
		TraversalSegment.Start += TraversalSegment.Binormal * AdjustmentOffset;
		TraversalSegment.End += TraversalSegment.Binormal * AdjustmentOffset;
	}
}
