// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CharacterGenData.h"
#include "Pipeline/CharacterGenPipeline.h"
#include "CharacterGenSubsystem.generated.h"

UCLASS()
class CHARACTERGEN_API UCharacterGenSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UCharacterGenSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen", meta = (WorldContext = "WorldContextObject"))
	static UCharacterGenSubsystem* Get(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "CharacterGen")
	UCharacterGenPipeline* CreatePipeline();

	UFUNCTION(BlueprintCallable, Category = "CharacterGen")
	void GenerateCharacter(
		UPARAM(Ref) UCharacterGenPipeline* Pipeline,
		const TArray<UTexture2D*>& InputImages,
		const FCharacterGenSettings& Settings,
		const FOnCharacterGenComplete& OnComplete,
		const FOnCharacterGenError& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "CharacterGen")
	FCharacterGenProgress GetProgress(UCharacterGenPipeline* Pipeline);

	UFUNCTION(BlueprintCallable, Category = "CharacterGen")
	void CancelGeneration(UPARAM(Ref) UCharacterGenPipeline* Pipeline);

	UFUNCTION(BlueprintCallable, Category = "CharacterGen")
	bool IsGenerationRunning(UCharacterGenPipeline* Pipeline) const;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen")
	void SetPythonPath(const FString& Path);

	UFUNCTION(BlueprintCallable, Category = "CharacterGen")
	FString GetPythonPath() const;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen")
	bool ValidatePythonEnvironment() const;

private:
	FString PythonPath;
	TArray<UCharacterGenPipeline*> ActivePipelines;
};
