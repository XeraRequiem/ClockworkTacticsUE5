// Copyright Xera Requiem Development (2026). All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class HexLibrary : ModuleRules
{
	 public HexLibrary(ReadOnlyTargetRules Target) : base(Target)
	 {
		  PublicDependencyModuleNames.AddRange(
			  new string[] {
				"Core",
				"CoreUObject",
				"Engine"
			  }
		  );
	 }
}