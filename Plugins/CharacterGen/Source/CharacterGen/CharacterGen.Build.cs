// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CharacterGen : ModuleRules
{
	public CharacterGen(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.Add(ModuleDirectory + "/Public");
		PublicIncludePaths.Add(ModuleDirectory + "/Public/AI");
		PublicIncludePaths.Add(ModuleDirectory + "/Public/Data");
		PublicIncludePaths.Add(ModuleDirectory + "/Public/Mesh");
		PublicIncludePaths.Add(ModuleDirectory + "/Public/MetaHuman");
		PublicIncludePaths.Add(ModuleDirectory + "/Public/Pipeline");
		
		PrivateIncludePaths.Add(ModuleDirectory + "/Private");
		PrivateIncludePaths.Add(ModuleDirectory + "/Private/AI");
		PrivateIncludePaths.Add(ModuleDirectory + "/Private/Pipeline");
		PrivateIncludePaths.Add(ModuleDirectory + "/Private/Mesh");
		PrivateIncludePaths.Add(ModuleDirectory + "/Private/MetaHuman");

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"HTTP",
				"Json",
				"JsonUtilities"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"RenderCore",
				"RHI",
				"AssetRegistry",
				"Slate",
				"SlateCore",
				"MeshDescription",
				"StaticMeshDescription",
				"SkeletalMeshDescription",
				"AnimationCore",
				"AnimGraphRuntime",
				"AnimationAsset",
				"GeometryFramework"
			}
		);

		if (Target.bBuildEditor == true)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"UnrealEd",
					"EditorStyle",
					"PropertyEditor"
				}
			);
		}

		bEnableExceptions = false;
		bUseRTTI = false;
	}
}
