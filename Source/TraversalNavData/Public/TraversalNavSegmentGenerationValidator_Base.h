//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"

#include "TraversalNavSegmentGenerationValidator_Base.generated.h"

struct FTraversalNavSegmentData;

UCLASS(NotBlueprintable, Abstract)
class TRAVERSALNAVDATA_API UTraversalNavSegmentGenerationValidator_Base : public UObject
{
	GENERATED_BODY()

public:
	// Begin UObject
	virtual UWorld* GetWorld() const override;
	// End UObject

	// Begin UTraversalNavSegmentGenerationValidator_Base
	virtual bool ValidateNavMeshEdge(const FVector& EdgeStart, const FVector& EdgeEnd, const FVector& EdgeBinormal, const FVector& EdgeNormal) const PURE_VIRTUAL(UTraversalNavSegmentGenerationValidator_Base::ValidateTraversalNavSegment, return false; );
	virtual void PostProcessTraversalNavSegment(FTraversalNavSegmentData& TraversalSegment) const  PURE_VIRTUAL(UTraversalNavSegmentGenerationValidator_Base::PostProcessTraversalNavSegment);
	// End UTraversalNavSegmentGenerationValidator_Base
};
