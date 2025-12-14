//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "CoreMinimal.h"

#include "Components/PrimitiveComponent.h"
#include "TraversalNavDataTypes.h"
#include "PrimitiveSceneProxy.h"

#include "TraversalNavDataRenderingComponent.generated.h"


UCLASS(config =  hidecategories = Object, editinlinenew)
class TRAVERSALNAVDATA_API UTraversalNavDataRenderingComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

public:
	UTraversalNavDataRenderingComponent();

	// Begin UPrimitiveComponent 
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	// End UPrimitiveComponent

public:
	UPROPERTY(config, Transient)
	bool bEnableDrawing = false;
};
