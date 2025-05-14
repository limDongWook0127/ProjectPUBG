// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PUBGEditorTarget : TargetRules
{
	public PUBGEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("PUBG");
		
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			bUsesSteam = true;  // Steam 활성화 (필요 없는 경우 삭제 가능)
		}
	}
}
