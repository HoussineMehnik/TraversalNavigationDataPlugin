//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "CoreMinimal.h"
#include "TraversalNavSegmentGenerationValidator_Base.h"

#include "TraversalNavSegmentGenerationValidator_Default.generated.h"


UCLASS(NotBlueprintable, Hidden)
class TRAVERSALNAVDATAEDITOR_API UTraversalNavSegmentGenerationValidator_Default : public UTraversalNavSegmentGenerationValidator_Base
{
	GENERATED_BODY()
	
public:

	// Begin UTraversalNavSegmentGenerationValidator_Base
	virtual bool ValidateNavMeshEdge(const FVector& EdgeStart, const FVector& EdgeEnd, const FVector& EdgeBinormal, const FVector& EdgeNormal) const override;
	virtual void PostProcessTraversalNavSegment(FTraversalNavSegmentData& TraversalSegment) const override;
	// End UTraversalNavSegmentGenerationValidator_Base
};
