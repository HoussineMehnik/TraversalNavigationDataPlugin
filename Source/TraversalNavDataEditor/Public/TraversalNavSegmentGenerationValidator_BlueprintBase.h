//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "CoreMinimal.h"
#include "TraversalNavDataTypes.h"
#include "TraversalNavSegmentGenerationValidator_Base.h"

#include "TraversalNavSegmentGenerationValidator_BlueprintBase.generated.h"

/**
 * 
 */
UCLASS(Abstract,Blueprintable)
class TRAVERSALNAVDATAEDITOR_API UTraversalNavSegmentGenerationValidator_BlueprintBase : public UTraversalNavSegmentGenerationValidator_Base
{
	GENERATED_BODY()

public:
	// Begin UTraversalNavSegmentGenerationValidator_Base
	virtual bool ValidateNavMeshEdge(const FVector& EdgeStart, const FVector& EdgeEnd, const FVector& EdgeBinormal, const FVector& EdgeNormal) const final;
	virtual void PostProcessTraversalNavSegment(FTraversalNavSegmentData& TraversalSegment) const final;
	// End UTraversalNavSegmentGenerationValidator_Base

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Validate NavMesh Edge"))
	bool ValidateNavMeshEdge_Blueprint(const FVector& EdgeStart, const FVector& EdgeEnd, const FVector& EdgeBinormal, const FVector& EdgeNormal) const;

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "PostProcess Traversal Segment"))
	void PostProcessTraversalNavSegment_Blueprint(UPARAM(ref) FTraversalNavSegmentData& TraversalSegment) const;
};
