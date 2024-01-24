using UnrealBuildTool;

public class CardTest : ModuleRules
{
	public CardTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core"
            ,"CoreUObject"
            ,"Engine"
            ,"InputCore"
            ,"EnhancedInput"
        });

        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}
