//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#include "TraversalNavData.h"

#include "Engine/World.h"
#include "TraversalNavDataFunctionLibrary.h"
#include "TraversalNavDataRenderingComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TraversalNavData)

static FAutoConsoleCommandWithWorldAndArgs CVarToggleTraversalNavDataDebug(
	TEXT("TraversalNavData.ToggleDebug"),
	TEXT("Toggles the visibility of traversal navigation segments data."),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		if (World != nullptr)
		{
			UTraversalNavDataFunctionLibrary::ToggleTraversalNavigationDataDebug(World);
		}
	}),
	ECVF_Default);


ATraversalNavData ::ATraversalNavData()
{
#if WITH_EDITOR
	bIsSpatiallyLoaded = false;
	bDefaultOutlinerExpansionState = false;
#endif // WITH_EDITOR

	PrimaryActorTick.bCanEverTick = false;
	bNetLoadOnClient = true;
	SetCanBeDamaged(false);

#if !UE_BUILD_SHIPPING
	RenderingComponent = CreateDefaultSubobject<UTraversalNavDataRenderingComponent>(TEXT("TraversalNavDataRenderingComponent"), true);
	RootComponent = RenderingComponent;
#endif // !UE_BUILD_SHIPPING

}

#if WITH_EDITOR
void ATraversalNavData ::ToggleDebugDraw()
{
	RenderingComponent->Modify();
	RenderingComponent->bEnableDrawing = !RenderingComponent->bEnableDrawing;
	RenderingComponent->MarkRenderStateDirty();
}
#endif // WITH_EDITOR