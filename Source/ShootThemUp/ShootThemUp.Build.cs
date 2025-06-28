// Shoot Them Up Game. All rights reserved

using UnrealBuildTool;

public class ShootThemUp : ModuleRules
{
	public ShootThemUp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
		"Core", 
		"CoreUObject", 
		"Engine", 
		"InputCore",
		"Niagara",
		"PhysicsCore",
		"NavigationSystem",
		"EnhancedInput",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayAbilities",
			"GameplayTasks",
			"GameplayTags"
		});

		PublicIncludePaths.AddRange(new string[] 
		{ 
		"ShootThemUP/Public/Player", 
		"ShootThemUP/Public/Components",
		"ShootThemUP/Public/Dev",
		"ShootThemUP/Public/Weapon",
		"ShootThemUP/Public/UI",
		"ShootThemUP/Public/Animation",
		"ShootThemUP/Public/Pickups",
		"ShootThemUP/Public/Weapon/Components",
		"ShootThemUP/Public/AI",
		"ShootThemUP/Public/AI/Tasks",
		"ShootThemUP/Public/AI/Services",
		"ShootThemUP/Public/AI/Decorators",
		"ShootThemUP/Public/AI/EQS",
		"ShootThemUP/Public/",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
