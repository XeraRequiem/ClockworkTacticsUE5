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
			"InputCore",
			"HeadMountedDisplay"
		});

		PublicIncludePaths.AddRange(new string[] {
			Path.Combine(ModuleDirectory, "Core", "Public"),
			Path.Combine(ModuleDirectory, "Hex", "Public")
		});
	}
}
