//================= Traversal Navigation Data Plugin =======================//
// Copyright Elhoussine Mehnik (ue4resources@gmail.com). All Rights Reserved.
//================== http://unrealengineresources.com/ =====================//

using UnrealBuildTool;

public class TraversalNavDataEditor : ModuleRules
{
	public TraversalNavDataEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
				"DeveloperSettings",
				"Engine",
				"LevelEditor",
				"LevelSequence",
				"MainFrame",
				"PhysicsCore",
				"Settings",
				"Slate",
				"SlateCore",
				"ToolMenus",
				"UnrealEd",
                "NavigationSystem", 
				"Navmesh",	
				"Landscape",
                "GameplayTags",
                "TraversalNavData",
            }
			);
	}
}
