// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PUBGTarget : TargetRules
{
	public PUBGTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("PUBG");
		
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			bUsesSteam = true;  // Steam 활성화 (필요 없는 경우 삭제 가능)
		}
	}
}
