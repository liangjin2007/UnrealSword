// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CharacterGenLoader : ModuleRules
{
	public CharacterGenLoader(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"CharacterGenLoader/Public"
			}
		);
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"CharacterGenLoader/Private",
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"HTTP",
				"Json"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
			}
		);
	}
}
