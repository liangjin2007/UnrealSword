// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Skeleton.h"
#include "CharacterGenData.h"
#include "Pipeline/ICharacterGenPipeline.h"
#include "CharacterGenPipeline.generated.h"

UCLASS(BlueprintType)
class CHARACTERGEN_API UCharacterGenPipeline : public UObject, public ICharacterGenPipeline
{
	GENERATED_BODY()

public:
	UCharacterGenPipeline();

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|Pipeline")
	void GenerateCharacter(
		const TArray<UTexture2D*>& InputImages,
		const FCharacterGenSettings& Settings,
		const FOnCharacterGenComplete& OnComplete,
		const FOnCharacterGenError& OnError
	) override;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|Pipeline")
	void GenerateCharacterAsync(
		const TArray<UTexture2D*>& InputImages,
		const FCharacterGenSettings& Settings
	) override;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|Pipeline")
	FCharacterGenProgress GetProgress() const override;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|Pipeline")
	void SetQualityLevel(ECharacterGenQuality Quality) override;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|Pipeline")
	void Cancel() override;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|Pipeline")
	bool IsRunning() const override;

protected:
	void ExecuteImageTo3D(const TArray<UTexture2D*>& InputImages);
	void ExecuteMeshPostProcess(UStaticMesh* RawMesh);
	void ExecuteMetaHumanFit(const TArray<FMeshPart>& MeshParts);
	void ExecuteSkeletonBinding(UStaticMesh* HeadMesh, UStaticMesh* BodyMesh);

	void OnImageTo3DComplete(UStaticMesh* GeneratedMesh);
	void OnMeshPostProcessComplete(const TArray<FMeshPart>& Parts);
	void OnMetaHumanFitComplete(UStaticMesh* HeadMesh, UStaticMesh* BodyMesh);
	void OnSkeletonBindingComplete(USkeletalMesh* HeadSkeletal, USkeletalMesh* BodySkeletal);

	void OnError(const FString& ErrorMessage);

	void UpdateProgress(ECharacterGenState State, float Progress, const FString& Step, const FString& Message = FString());

private:
	UPROPERTY()
	FCharacterGenSettings CurrentSettings;

	UPROPERTY()
	FCharacterGenProgress CurrentProgress;

	FOnCharacterGenComplete CompletionCallback;
	FOnCharacterGenError ErrorCallback;

	UPROPERTY()
	UStaticMesh* GeneratedRawMesh;

	UPROPERTY()
	TArray<FMeshPart> DecomposedParts;

	UPROPERTY()
	UStaticMesh* FittedHeadMesh;

	UPROPERTY()
	UStaticMesh* FittedBodyMesh;

	UPROPERTY()
	USkeleton* MetaHumanSkeleton;

	UPROPERTY()
	USkeletalMesh* HeadSkeletalMesh;

	UPROPERTY()
	USkeletalMesh* BodySkeletalMesh;

	bool bIsCancelled;
	bool bIsRunning;
};
