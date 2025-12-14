//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "TraversalNavDataTypes.h"

#include "TraversalNavData.generated.h"

class UTraversalNavDataRenderingComponent;

UCLASS(hidecategories = (Input, Tags, Transformation, Actor, Layers, Replication), NotBlueprintable, NotPlaceable)
class TRAVERSALNAVDATA_API ATraversalNavData : public AActor
{
	GENERATED_BODY()

public:
	ATraversalNavData();

	// Begin AActor
#if WITH_EDITOR
	virtual bool ActorTypeSupportsDataLayer() const override { return false; }
	virtual bool ActorTypeSupportsExternalDataLayer() const override { return false; }
	virtual bool CanChangeIsSpatiallyLoadedFlag() const override { return false; }
	virtual bool ShouldExport() override { return false; }
	virtual bool ShouldImport(FStringView ActorPropString, bool IsMovingLevel) override { return false; }
#endif // WITH_EDITOR
	// End AActor

#if WITH_EDITOR
	UFUNCTION(CallInEditor)
	void ToggleDebugDraw();
#endif // WITH_EDITOR

	FORCEINLINE UTraversalNavDataRenderingComponent* GetRenderingComponent() const { return RenderingComponent; }
	const TArray<FTraversalNavSegmentData>& GetTraversalNavSegments() const { return TraversalNavSegments; }
#if WITH_EDITOR
	TArray<FTraversalNavSegmentData>& GetTraversalNavSegmentsMutable() { return TraversalNavSegments; }
#endif // WITH_EDITOR

private:
	UPROPERTY(Transient)
	TObjectPtr<UTraversalNavDataRenderingComponent> RenderingComponent;

	UPROPERTY()
	TArray<FTraversalNavSegmentData> TraversalNavSegments;
};
