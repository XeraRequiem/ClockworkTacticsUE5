// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ClockworkTacticsTarget : TargetRules
{
	 public ClockworkTacticsTarget( TargetInfo Target) : base(Target)
	 {
		  Type = TargetType.Game;
		  DefaultBuildSettings = BuildSettingsVersion.Latest;
		  IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		  ExtraModuleNames.AddRange( new string[] { "ClockworkTactics" } );
	 }
}
