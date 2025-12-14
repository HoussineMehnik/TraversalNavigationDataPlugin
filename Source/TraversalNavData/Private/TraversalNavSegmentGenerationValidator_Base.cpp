//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#include "TraversalNavSegmentGenerationValidator_Base.h"
#include "Engine/World.h"

#if WITH_EDITOR
#include "Editor.h"
#endif // WITH_EDITOR

#include UE_INLINE_GENERATED_CPP_BY_NAME(TraversalNavSegmentGenerationValidator_Base)

UWorld* UTraversalNavSegmentGenerationValidator_Base::GetWorld() const
{
#if WITH_EDITOR
	if (GEditor != nullptr)
	{
		return GEditor->GetEditorWorldContext().World();
	}

	if (GIsEditor)
	{
		return GWorld;
	}
#endif // WITH_EDITOR

	return Super::GetWorld();
}
