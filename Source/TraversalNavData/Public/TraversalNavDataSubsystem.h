//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "CoreMinimal.h"

#include "HAL/CriticalSection.h"
#include "Subsystems/WorldSubsystem.h"
#include "TraversalNavDataOctree.h"
#include "TraversalNavDataTypes.h"
#include "UObject/Object.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include "TraversalNavDataSubsystem.generated.h"

class ATraversalNavData ;
class UWorld;

UCLASS(meta = (DisplayName = "Traversal Navigation Subsystem"))
class TRAVERSALNAVDATA_API UTraversalNavDataSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	ATraversalNavData* GetOrCreateTraversalNavData() const;

	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	// End USubsystem

	// Begin UTickableWorldSubsystem
	virtual bool IsTickable() const override { return true; }
	virtual bool IsTickableInEditor() const override { return false; }
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;
	// End UTickableWorldSubsystem

private:
	void InitOctree();
	void ClearOctree();
	void ProcessPendingRuntimeSegments();

public:
	UFUNCTION(BlueprintCallable, Category = "Traversal Navigation")
	void AddUserDefinedTraversalSegment(const FTraversalNavSegmentData& TraversalSegment);

	UFUNCTION(BlueprintCallable, Category = "Traversal Navigation", meta = (AutoCreateRefTerm = "QueryBoxOrigin , QueryBoxExtent , QueryBoxRotation"))
	bool FindTraversalSegments(TArray<FTraversalNavSegmentData>& OutSegments, const FVector& QueryBoxOrigin, const FRotator& QueryBoxRotation, const FVector& QueryBoxExtent, bool bDrawDebug, float DrawTime);

private:
	mutable TWeakObjectPtr<ATraversalNavData> TraversalNavDataPtr;
	mutable FCriticalSection Mutex;
	FTraversalNavDataOctree Octree;
	TArray<FTraversalNavSegmentData> PendingAddRuntimeSegments;
};
