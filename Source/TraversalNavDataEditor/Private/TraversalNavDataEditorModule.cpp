//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

#include "TraversalNavDataEditorModule.h"

#include "Editor.h"
#include "Editor/EditorEngine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "ISettingsModule.h"
#include "Modules/ModuleManager.h"
#include "Styling/AppStyle.h"
#include "ToolMenu.h"
#include "ToolMenus.h"
#include "ToolMenuSection.h"
#include "TraversalNavData.h"
#include "TraversalNavDataEditorCommands.h"
#include "TraversalNavDataEditorSubsystem.h"

#define LOCTEXT_NAMESPACE "TraversalNavDataEditorModule"

void FTraversalNavDataEditorModule::StartupModule()
{
	RegisterCommands();
	RegisterMenus();
}

void FTraversalNavDataEditorModule::ShutdownModule()
{
	UnregisterCommands();
}

void FTraversalNavDataEditorModule::RegisterCommands()
{
	FTraversalNavDataEditorCommands::Register();

	TraversalNavDataEditorCommands = MakeShareable(new FUICommandList);
	TraversalNavDataEditorCommands->MapAction(
		FTraversalNavDataEditorCommands::Get().BuildTraversalNavData,
		FExecuteAction::CreateRaw(this, &FTraversalNavDataEditorModule::BuildTraversalNavData),
		FCanExecuteAction());

	TraversalNavDataEditorCommands->MapAction(
		FTraversalNavDataEditorCommands::Get().EditTraversalNavDataSettings,
		FExecuteAction::CreateRaw(this, &FTraversalNavDataEditorModule::OpenTraversalNavDataProjectSettings),
		FCanExecuteAction());
}

void FTraversalNavDataEditorModule::UnregisterCommands()
{
	TraversalNavDataEditorCommands.Reset();
}

void FTraversalNavDataEditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	if (UToolMenu* BuildMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Build"))
	{
		const TSharedPtr<FUICommandList> CommandsList = TraversalNavDataEditorCommands;

		FToolMenuSection& TraversalNavSection = BuildMenu->AddSection("TraversalNav", LOCTEXT("TraversalNavHeading", "Traversal Navigation"), FToolMenuInsert("LevelEditorNavigation", EToolMenuInsertType::After));
		TraversalNavSection.AddMenuEntryWithCommandList(FTraversalNavDataEditorCommands::Get().BuildTraversalNavData, CommandsList);
		TraversalNavSection.AddMenuEntryWithCommandList(FTraversalNavDataEditorCommands::Get().EditTraversalNavDataSettings, CommandsList);
	}
}

void FTraversalNavDataEditorModule::BuildTraversalNavData()
{
	if (GEditor != nullptr)
	{
		if (const UWorld* World = GEditor->GetEditorWorldContext().World())
		{
			UTraversalNavDataEditorSubsystem* TraversalNavDataEditorSubsystem = UWorld::GetSubsystem<UTraversalNavDataEditorSubsystem>(World);
			TraversalNavDataEditorSubsystem->BuildTraversalNavData();
		}
	}
}

void FTraversalNavDataEditorModule::OpenTraversalNavDataProjectSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->ShowViewer(FName("Project"), FName("Engine"), FName("TraversalNavDataSettings"));
		SettingsModule->ShowViewer(FName("Project"), FName("Engine"), FName("TraversalNavDataSettings"));
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTraversalNavDataEditorModule, TraversalNavDataEditor)
