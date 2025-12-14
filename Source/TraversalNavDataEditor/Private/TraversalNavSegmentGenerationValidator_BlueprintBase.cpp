//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#include "TraversalNavSegmentGenerationValidator_BlueprintBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TraversalNavSegmentGenerationValidator_BlueprintBase)

bool UTraversalNavSegmentGenerationValidator_BlueprintBase::ValidateNavMeshEdge(const FVector& EdgeStart, const FVector& EdgeEnd, const FVector& EdgeBinormal, const FVector& EdgeNormal) const
{
	if (ValidateNavMeshEdge_Blueprint(EdgeStart, EdgeEnd, EdgeBinormal, EdgeNormal))
	{
		return true;
	}

	return false;
}

void UTraversalNavSegmentGenerationValidator_BlueprintBase::PostProcessTraversalNavSegment(FTraversalNavSegmentData& TraversalSegment) const
{
	PostProcessTraversalNavSegment_Blueprint(TraversalSegment);
}

bool UTraversalNavSegmentGenerationValidator_BlueprintBase::ValidateNavMeshEdge_Blueprint_Implementation(const FVector& SegmentStart, const FVector& SegmentEnd, const FVector& SegmentBinormal, const FVector& SegmentNormal) const
{
	return true;
}

void UTraversalNavSegmentGenerationValidator_BlueprintBase::PostProcessTraversalNavSegment_Blueprint_Implementation(FTraversalNavSegmentData& TraversalSegment) const
{

}
