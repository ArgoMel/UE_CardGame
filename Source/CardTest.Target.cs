using UnrealBuildTool;
using System.Collections.Generic;

public class CardTestTarget : TargetRules
{
	public CardTestTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.AddRange( new string[] { "CardTest" } );
	}
}
