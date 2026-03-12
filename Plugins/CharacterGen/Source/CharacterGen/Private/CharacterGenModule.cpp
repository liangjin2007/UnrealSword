// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterGenModule.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "FCharacterGenModule"

DEFINE_LOG_CATEGORY(LogCharacterGen);

void FCharacterGenModule::StartupModule()
{
	UE_LOG(LogCharacterGen, Log, TEXT("CharacterGen Module Starting..."));

	FString PluginDir = IPluginManager::Get().FindPlugin(TEXT("CharacterGen"))->GetBaseDir();
	UE_LOG(LogCharacterGen, Log, TEXT("CharacterGen Plugin Directory: %s"), *PluginDir);

	RegisterSettings();
	
	UE_LOG(LogCharacterGen, Log, TEXT("CharacterGen Module Started Successfully"));
}

void FCharacterGenModule::ShutdownModule()
{
	UE_LOG(LogCharacterGen, Log, TEXT("CharacterGen Module Shutting Down..."));
	
	UnregisterSettings();
	
	UE_LOG(LogCharacterGen, Log, TEXT("CharacterGen Module Shutdown Complete"));
}

void FCharacterGenModule::RegisterSettings()
{
}

void FCharacterGenModule::UnregisterSettings()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCharacterGenModule, CharacterGen)
