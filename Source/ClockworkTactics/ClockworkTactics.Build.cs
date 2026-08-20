using UnrealBuildTool;
using System.IO;

public class ClockworkTactics : ModuleRules
{
	 public ClockworkTactics(ReadOnlyTargetRules Target) : base(Target)
	 {
		  PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		  PublicDependencyModuleNames.AddRange(new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"Json",
				"JsonUtilities",
				"InputCore"
		  });

		  PrivateDependencyModuleNames.AddRange(new string[] {
				"HexLibrary",
				"OnlineSubsystem"
		  });
	 }
}
