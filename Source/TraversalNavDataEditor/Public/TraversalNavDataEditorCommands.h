//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Styling/AppStyle.h"


class FTraversalNavDataEditorCommands : public TCommands<FTraversalNavDataEditorCommands>
{
public:

	FTraversalNavDataEditorCommands() : TCommands<FTraversalNavDataEditorCommands>(
		TEXT("TraversalNavDataEditorCommands"), 
		NSLOCTEXT("Contexts", "TraversalNavDataEditor", "Traversal NavData Editor"), 
		NAME_None, 
		FAppStyle::GetAppStyleSetName())
	{

	}

	// Begin TCommands
	virtual void RegisterCommands() override;
	// End TCommands

public:
	TSharedPtr<FUICommandInfo> BuildTraversalNavData;
	TSharedPtr<FUICommandInfo> EditTraversalNavDataSettings;
};
