//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "TraversalNavDataEditorSubsystem.generated.h"

class ALandscape;
class ARecastNavMesh;
class UTraversalNavDataSettings;

TRAVERSALNAVDATAEDITOR_API DECLARE_LOG_CATEGORY_EXTERN(LogTraversalNavDataEditor, Log, All);

UCLASS(NotBlueprintType, Hidden)
class TRAVERSALNAVDATAEDITOR_API UTraversalNavDataEditorSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UTraversalNavDataEditorSubsystem();

	// Begin UTickableWorldSubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End UTickableWorldSubsystem

	// Begin UTickableWorldSubsystem
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;
	virtual bool IsTickable() const override { return true; }
	virtual bool IsTickableInEditor() const override { return true; }
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
	// End UTickableWorldSubsystem

	void BuildTraversalNavData();
	bool IsBuilding() const { return bIsBuildingStarted; }
private:
	void FinishBuild();
	void GenerateTraversalNavSegments();

private:

	struct FRecastNavMeshCachedState
	{
		float AgentRadius;
		float AgentHeight;
		float AgentMaxSlope;
		float AgentMaxStepHeight;
		float CellHeight;
		float CellSize;
		float SimplificationElevationRatio;

		bool bIsSet = false;

		void SaveAndApplySettings(ARecastNavMesh& NavMesh, const UTraversalNavDataSettings& TraversalNavDataSettings);
		void Restore(ARecastNavMesh& NavMesh);
	};
	FRecastNavMeshCachedState NavMeshCachedState;
	TArray<TWeakObjectPtr<ALandscape>> ModifiedLandsacpes;
	TWeakObjectPtr<ARecastNavMesh> DefaultNavMeshPtr;
	bool bIsBuildingStarted = false;

public:
	DECLARE_MULTICAST_DELEGATE(FOnTraversalNavDataDataBuildDone);
	FOnTraversalNavDataDataBuildDone OnnTraversalNavDataDataBuildDone;
};
