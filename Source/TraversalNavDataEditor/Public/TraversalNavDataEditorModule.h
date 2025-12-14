//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#pragma once

#include "Modules/ModuleManager.h"

class FUICommandList;
class UWorld;


class FTraversalNavDataEditorModule : public IModuleInterface
{
public:

	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface

private:
	void RegisterCommands();
	void UnregisterCommands();
	void RegisterMenus();

	void BuildTraversalNavData();
	void OpenTraversalNavDataProjectSettings();

private:
	TSharedPtr<FUICommandList> TraversalNavDataEditorCommands;
};
