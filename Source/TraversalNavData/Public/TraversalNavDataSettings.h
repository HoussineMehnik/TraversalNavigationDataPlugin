//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "CoreMinimal.h"

#include "Engine/DeveloperSettings.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagContainer.h"
#include "TraversalNavSegmentGenerationValidator_Base.h"

#include "TraversalNavDataSettings.generated.h"

struct FTraversalNavSegmentData;

UCLASS(config = Engine, defaultconfig)
class TRAVERSALNAVDATA_API UTraversalNavDataSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UTraversalNavDataSettings();

	UPROPERTY(config, EditAnywhere, Category = "Generation", meta = (ClampMin = "1.0", UIMin = "1.0", ClampMax = "5.0", UIMax = "5.0"))
	float VoxelizationCellSize = 2.5f;

	UPROPERTY(config, EditAnywhere, Category = "Generation", meta = (ClampMin = "1.0", UIMin = "1.0"))
	float CharacterRadius = 35.0f;

	UPROPERTY(config, EditAnywhere, Category = "Generation", meta = (ClampMin = "1.0", UIMin = "1.0"))
	float CharacterHeight = 170.0f;

	UPROPERTY(config, EditAnywhere, Category = "Generation", meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "89.0", UIMax = "89.0"))
	float CharacterMaxSlope = 44.5f;

	UPROPERTY(config, EditAnywhere, Category = "Generation", meta = (ClampMin = "0.0"))
	float SimplificationElevationRatio = 1.0f;

	UPROPERTY(config, EditAnywhere, Category = "Generation", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MinSegmentLength = 25.0f;

	UPROPERTY(config, EditAnywhere, Category = "Generation")
	bool bExcludeLandscapesWhileGenerating = true;

	UPROPERTY(config, EditAnywhere, Category = "Generation")
	bool bDisableDefaultValidator = false;

	UPROPERTY(config, EditAnywhere, Category = "Generation")
	TEnumAsByte<ECollisionChannel> DefaultValidatorTraceChannel = ECC_Visibility;

	UPROPERTY(config, EditAnywhere, Category = "Generation", meta = (DisplayName = "Additional Validator"))
	TSoftClassPtr<UTraversalNavSegmentGenerationValidator_Base> AdditionalValidatorClassPtr;

	/** vertical offset added to lines debug representation for better readability */
	UPROPERTY(config, EditAnywhere, Category = "Debug", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float DrawOffset = 2.0f;

	UPROPERTY(config, EditAnywhere, Category = "Debug", meta = (ClampMin = "1000.0", UIMin = "1000.0"))
	float MaxDrawDistance = 100000.0f;

	UPROPERTY(config, EditAnywhere, Category = "Debug", meta = (Categories="TraversalSegment"))
	TMap<FGameplayTag, FColor> SegmentColorMap;

	UPROPERTY(config, EditAnywhere, Category = "Debug")
	bool bShowDefaultValidatorTraceChecks = false;

	UPROPERTY(config, EditAnywhere, Category = "Debug", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float DefaultValidatorDebugDrawTime = 10.0f;

public:
	FColor GetTraversalNavSegmentColor(const FGameplayTag& Tag) const;
	FColor GetTraversalNavSegmentColor(const FTraversalNavSegmentData& TraversalSegment) const;
	
};
