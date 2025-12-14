//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#include "TraversalNavDataEditorCommands.h"

#include "Framework/Commands/Commands.h"
#include "Framework/Commands/InputChord.h"
#include "Framework/Commands/UICommandInfo.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "TraversalNavDataEditorCommands"

void FTraversalNavDataEditorCommands::RegisterCommands()
{
	UI_COMMAND(BuildTraversalNavData, "Build Traversal Navigation Data", "Builds traversal navigation data for current level.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(EditTraversalNavDataSettings, "Edit Traversal Navigation Settings", "Opens traversal navigation default settings tab.", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE