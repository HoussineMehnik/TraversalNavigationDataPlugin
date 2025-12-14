//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "CoreMinimal.h"

#include "EngineDefines.h"
#include "NativeGameplayTags.h"
#include "UObject/NoExportTypes.h"

#include "TraversalNavDataTypes.generated.h"

TRAVERSALNAVDATA_API DECLARE_LOG_CATEGORY_EXTERN(LogTraversalNavigation, Log, All);

TRAVERSALNAVDATA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TraversalSegment_Default);

USTRUCT(BlueprintType)
struct TRAVERSALNAVDATA_API FTraversalNavSegmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraversalSegment")
	FVector Start = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraversalSegment")
	FVector End = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraversalSegment")
	FVector Binormal = FVector::RightVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraversalSegment", meta = (Categories="TraversalSegment"))
	FGameplayTag Tag = TraversalSegment_Default;

public:
	FVector GetNormal() const;
	FVector GetDirection() const;
	FVector GetMidPoint() const;
	float GetLength() const;
};
