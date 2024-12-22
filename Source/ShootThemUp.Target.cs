// Shoot Them Up Game. All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class ShootThemUpTarget : TargetRules
{
	public ShootThemUpTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
       
        DefaultBuildSettings = BuildSettingsVersion.Latest;

        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.Add("ShootThemUp");
    }
}
