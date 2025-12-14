//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//


#include "TraversalNavDataTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TraversalNavDataTypes)

DEFINE_LOG_CATEGORY(LogTraversalNavigation);

UE_DEFINE_GAMEPLAY_TAG_COMMENT(TraversalSegment_Default, "TraversalSegment.Default", "Default traversal segment type.");

FVector FTraversalNavSegmentData::GetNormal() const
{
	return FVector::CrossProduct(End - Start, Binormal).GetSafeNormal();
}

FVector FTraversalNavSegmentData::GetDirection() const
{
	return (End - Start).GetSafeNormal();
}

FVector FTraversalNavSegmentData::GetMidPoint() const
{
	return (End - Start) * 0.5f;
}

float FTraversalNavSegmentData::GetLength() const
{
	return (End - Start).Size();
}
