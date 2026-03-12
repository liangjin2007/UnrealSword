// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterGenData.h"
#include "ICharacterGenPipeline.generated.h"

class ICharacterGenPipeline
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "CharacterGen|Pipeline")
	virtual void GenerateCharacter(
		const TArray<UTexture2D*>& InputImages,
		const FCharacterGenSettings& Settings,
		const FOnCharacterGenComplete& OnComplete,
		const FOnCharacterGenError& OnError
	) = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|Pipeline")
	virtual void GenerateCharacterAsync(
		const TArray<UTexture2D*>& InputImages,
		const FCharacterGenSettings& Settings
	) = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|Pipeline")
	virtual FCharacterGenProgress GetProgress() const = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|Pipeline")
	virtual void SetQualityLevel(ECharacterGenQuality Quality) = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|Pipeline")
	virtual void Cancel() = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|Pipeline")
	virtual bool IsRunning() const = 0;
};
