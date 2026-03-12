// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/StaticMesh.h"
#include "CharacterGenData.h"
#include "IAIInferenceService.generated.h"

UINTERFACE(BlueprintType, NotBlueprintable)
class CHARACTERGEN_API UAIInferenceService : public UInterface
{
	GENERATED_BODY()
};

class IAIInferenceService
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "CharacterGen|AI")
	virtual void Initialize(const FString& ModelPath) = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|AI")
	virtual bool IsInitialized() const = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|AI")
	virtual void Generate3DFromImage(
		UTexture2D* InputImage,
		const FOnCharacterGenComplete& OnComplete,
		const FOnCharacterGenError& OnError
	) = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|AI")
	virtual void Generate3DFromImages(
		const TArray<UTexture2D*>& InputImages,
		const FOnCharacterGenComplete& OnComplete,
		const FOnCharacterGenError& OnError
	) = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|AI")
	virtual void DecomposeMesh(
		UStaticMesh* InputMesh,
		const FOnCharacterGenComplete& OnComplete,
		const FOnCharacterGenError& OnError
	) = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|AI")
	virtual void Cancel() = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|AI")
	virtual float GetProgress() const = 0;
};
