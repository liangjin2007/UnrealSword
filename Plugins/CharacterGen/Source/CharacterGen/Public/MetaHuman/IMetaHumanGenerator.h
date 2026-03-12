// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Skeleton.h"
#include "Materials/MaterialInterface.h"
#include "CharacterGenData.h"
#include "IMetaHumanGenerator.generated.h"

UINTERFACE(BlueprintType, NotBlueprintable)
class CHARACTERGEN_API UMetaHumanGenerator : public UInterface
{
	GENERATED_BODY()
};

class IMetaHumanGenerator
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "CharacterGen|MetaHuman")
	virtual void Initialize() = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|MetaHuman")
	virtual UStaticMesh* FitToMetaHumanTopology(
		UStaticMesh* InputMesh,
		EMetaHumanPart PartType,
		const TArray<FLandmark3D>& Landmarks
	) = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|MetaHuman")
	virtual USkeletalMesh* CreateSkeletalMesh(
		UStaticMesh* FittedMesh,
		USkeleton* TargetSkeleton
	) = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|MetaHuman")
	virtual void ApplySkinning(
		USkeletalMesh* SkeletalMesh,
		const TMap<FString, FBoneWeightArray>& BoneWeights
	) = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|MetaHuman")
	virtual USkeleton* GetMetaHumanSkeleton(EMetaHumanPart PartType) = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|MetaHuman")
	virtual TArray<FLandmark3D> DetectFaceLandmarks(UTexture2D* FaceImage) = 0;

	UFUNCTION(BlueprintCallable, Category = "CharacterGen|MetaHuman")
	virtual void TransferTexture(
		UStaticMesh* SourceMesh,
		UStaticMesh* TargetMesh,
		UMaterialInterface*& OutMaterial
	) = 0;
};
