//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TraversalNavDataTypes.h"

#include "TraversalNavDataFunctionLibrary.generated.h"

class UWorld;

UCLASS()
class TRAVERSALNAVDATA_API UTraversalNavDataFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Traversal Navigation|Segment", meta = (DisplayName = "Normal"))
	static FVector GetTraversalSegmentNormal(const FTraversalNavSegmentData& TraversalSegment);

	UFUNCTION(BlueprintPure, Category = "Traversal Navigation|Segment", meta = (DisplayName = "Direction"))
	static FVector GetTraversalSegmentDirection(const FTraversalNavSegmentData& TraversalSegment);

	UFUNCTION(BlueprintPure, Category = "Traversal Navigation|Segment", meta = (DisplayName = "Mid Point"))
	static FVector GetTraversalSegmentMidPoint(const FTraversalNavSegmentData& TraversalSegment);

	UFUNCTION(BlueprintPure, Category = "Traversal Navigation|Segment", meta = (DisplayName = "Length"))
	static float GetTraversalSegmentLength(const FTraversalNavSegmentData& TraversalSegment);

	UFUNCTION(BlueprintPure, Category = "Traversal Navigation|Segment", meta = (AutoCreateRefTerm = "Point"))
	static FVector FindClosestPointOnTraversalSegment(const FTraversalNavSegmentData& TraversalSegment, const FVector& Point, float ClampOffset = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Traversal Navigation|Debug", meta = (WorldContext = "WorldContextObject", DevelopmentOnly))
	static void ToggleTraversalNavigationDataDebug(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Traversal Navigation|Debug", meta = (WorldContext = "WorldContextObject", DevelopmentOnly))
	static void DebugDrawTraversalSegment(const UObject* WorldContextObject, const FTraversalNavSegmentData& TraversalSegment, float DrawTime = 0.0f);

	UFUNCTION(BlueprintPure, Category = "Traversal Navigation|Debug", meta = (GameplayTagFilter = "TraversalSegment"))
	static FLinearColor GetTraversalSegmentColor(const FTraversalNavSegmentData& TraversalSegment);

	UFUNCTION(BlueprintPure, Category = "Traversal Navigation|Debug", meta = (GameplayTagFilter = "TraversalSegment"))
	static FLinearColor GetTraversalSegmentColorByTag(const FGameplayTag Tag);
};
