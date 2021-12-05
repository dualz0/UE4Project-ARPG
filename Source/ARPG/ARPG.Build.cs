// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ARPG : ModuleRules
{
	public ARPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule", "GameplayTasks", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });


	}
}
