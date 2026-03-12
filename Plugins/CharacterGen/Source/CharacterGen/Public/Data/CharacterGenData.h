// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "Engine/EngineTypes.h"
#include "UObject/Object.h"
#include "UObject/Class.h"
#include "CharacterGenData.generated.h"

UENUM(BlueprintType)
enum class ECharacterGenState : uint8
{
	Idle           UMETA(DisplayName = "Idle"),
	Processing     UMETA(DisplayName = "Processing"),
	ImageTo3D      UMETA(DisplayName = "Image to 3D"),
	MeshPostProcess UMETA(DisplayName = "Mesh Post Processing"),
	MetaHumanFit  UMETA(DisplayName = "MetaHuman Fitting"),
	SkeletonBinding UMETA(DisplayName = "Skeleton Binding"),
	Completed      UMETA(DisplayName = "Completed"),
	Failed         UMETA(DisplayName = "Failed")
};

UENUM(BlueprintType)
enum class ECharacterGenQuality : uint8
{
	Preview      UMETA(DisplayName = "Preview"),
	Standard     UMETA(DisplayName = "Standard"),
	High         UMETA(DisplayName = "High"),
	Ultra        UMETA(DisplayName = "Ultra")
};

UENUM(BlueprintType)
enum class EMetaHumanPart : uint8
{
	Head         UMETA(DisplayName = "Head"),
	Body         UMETA(DisplayName = "Body"),
	FullBody     UMETA(DisplayName = "Full Body")
};

UENUM(BlueprintType)
enum class EAIModelType : uint8
{
	TRELLIS2     UMETA(DisplayName = "TRELLIS2"),
	TRELLIS      UMETA(DisplayName = "TRELLIS"),
	Hunyuan3D    UMETA(DisplayName = "Hunyuan3D")
};

UCLASS()
class UCharacterGenCallbacks : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Message;

	UPROPERTY()
	float Progress = 0.0f;
};

USTRUCT(BlueprintType)
struct FCharacterGenSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Settings")
	ECharacterGenQuality Quality = ECharacterGenQuality::Standard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Settings")
	EAIModelType ModelType = EAIModelType::TRELLIS2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Settings")
	FString ModelCachePath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Settings")
	int32 TargetTriangleCount = 15000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Settings")
	bool bGenerateHead = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Settings")
	bool bGenerateBody = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Settings")
	bool bUseCloudAPI = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Settings")
	FString CloudAPIEndpoint;
};

USTRUCT(BlueprintType)
struct FCharacterGenProgress
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Progress")
	ECharacterGenState State = ECharacterGenState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Progress")
	float Progress = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Progress")
	FString CurrentStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Progress")
	FString Message;
};

USTRUCT(BlueprintType)
struct FGeneratedCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Result")
	UObject* HeadMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Result")
	UObject* BodyMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Result")
	UObject* HeadSkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Result")
	UObject* BodySkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Result")
	UObject* Skeleton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Result")
	FString AssetPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Result")
	TMap<FString, UObject*> AdditionalAssets;
};

USTRUCT(BlueprintType)
struct FMeshPart
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Mesh")
	FString PartName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Mesh")
	UObject* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Mesh")
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Mesh")
	int32 VertexCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Mesh")
	int32 TriangleCount = 0;
};

USTRUCT(BlueprintType)
struct FLandmark2D
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Landmark")
	FVector2D Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Landmark")
	float Confidence = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Landmark")
	int32 PointIndex = 0;
};

USTRUCT(BlueprintType)
struct FLandmark3D
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Landmark")
	FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Landmark")
	FVector Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Landmark")
	float Confidence = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Landmark")
	int32 PointIndex = 0;
};

USTRUCT(BlueprintType)
struct FBoneWeightArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterGen|Skeleton")
	TArray<float> Weights;

	FBoneWeightArray()
	{
		Weights.SetNum(4);
	}
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCharacterGenComplete, const FGeneratedCharacter&, Result);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCharacterGenError, const FString&, ErrorMessage);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCharacterGenProgressUpdate, const FCharacterGenProgress&, Progress);
