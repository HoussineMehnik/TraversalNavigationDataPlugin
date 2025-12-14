//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "CoreMinimal.h"

#include "Math/GenericOctree.h"
#include "TraversalNavDataTypes.h"


struct FTraversalNavDataOctreeElement
{
	FTraversalNavDataOctreeElement() = default;

	explicit FTraversalNavDataOctreeElement(const FTraversalNavSegmentData& InTraversalNavSegment)
		: TraversalNavSegmentPtr(&InTraversalNavSegment)
	{
	}

	const FTraversalNavSegmentData* TraversalNavSegmentPtr = nullptr;
};

struct TTraversalNavDataOctreeSemantics
{
	enum { MaxElementsPerLeaf = 16 };
	enum { MinInclusiveElementsPerNode = 7 };
	enum { MaxNodeDepth = 12 };

	typedef TInlineAllocator<MaxElementsPerLeaf> ElementAllocator;

	static FBoxCenterAndExtent GetBoundingBox(const FTraversalNavDataOctreeElement& Element)
	{
		FBox Box(ForceInit);
		{
			Box += Element.TraversalNavSegmentPtr->Start;
			Box += Element.TraversalNavSegmentPtr->End;
		}
		return { Box };
	}

	static const bool AreElementsEqual(const FTraversalNavDataOctreeElement& Lhs, const FTraversalNavDataOctreeElement& Rhs)
	{
		if (Lhs.TraversalNavSegmentPtr == Rhs.TraversalNavSegmentPtr)
		{
			return true;
		}
		
		return 
			Lhs.TraversalNavSegmentPtr->Start == Rhs.TraversalNavSegmentPtr->Start && 
			Lhs.TraversalNavSegmentPtr->End == Rhs.TraversalNavSegmentPtr->End;
	}

	static void SetElementId(const FTraversalNavDataOctreeElement& Element, FOctreeElementId2 OctreeElementID)
	{
	
	}
};

class FTraversalNavDataOctree : public TOctree2<FTraversalNavDataOctreeElement, TTraversalNavDataOctreeSemantics>
{

};