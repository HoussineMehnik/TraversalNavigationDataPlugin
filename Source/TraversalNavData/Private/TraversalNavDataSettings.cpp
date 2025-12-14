//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#include "TraversalNavDataSettings.h"

#include "TraversalNavDataTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TraversalNavDataSettings)

UTraversalNavDataSettings::UTraversalNavDataSettings()
{
	SegmentColorMap.Add(TraversalSegment_Default, FColor::Cyan);
}

FColor UTraversalNavDataSettings::GetTraversalNavSegmentColor(const FGameplayTag& Tag) const
{
	if (const FColor* Color = SegmentColorMap.Find(Tag))
	{
		return *Color;
	}

	return FColor::Magenta;
}

FColor UTraversalNavDataSettings::GetTraversalNavSegmentColor(const FTraversalNavSegmentData& TraversalSegment) const
{
	return GetTraversalNavSegmentColor(TraversalSegment.Tag);
}
