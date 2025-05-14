// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PUBG : ModuleRules
{
	public PUBG(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "AnimGraphRuntime" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks", "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Slate", "SlateCore", "UMG", "OnlineSubsystem", "OnlineSubsystemSteam", "Landscape"});

		//PublicIncludePaths.Add("/Pugs");
	}
}
