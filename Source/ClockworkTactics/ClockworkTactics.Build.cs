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
			"InputCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"OnlineSubsystem"
		});

		PublicIncludePaths.AddRange(new string[] {
			Path.Combine(ModuleDirectory, "Core", "Public"),
			Path.Combine(ModuleDirectory, "Entity", "Public"),
			Path.Combine(ModuleDirectory, "Grid", "Public")
		});
	}
}
