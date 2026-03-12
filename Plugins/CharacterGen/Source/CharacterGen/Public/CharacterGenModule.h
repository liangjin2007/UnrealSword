// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * CharacterGen Module
 * AI-powered character generation from images to Metahuman skeletal meshes
 */
class FCharacterGenModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterSettings();
	void UnregisterSettings();
};

DECLARE_LOG_CATEGORY_EXTERN(LogCharacterGen, Log, All);
