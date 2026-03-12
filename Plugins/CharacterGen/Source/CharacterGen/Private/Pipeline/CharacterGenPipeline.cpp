// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterGenPipeline.h"
#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Skeleton.h"
#include "Rendering/SkeletalMeshModel.h"
#include "Rendering/SkeletalMeshLODModel.h"
#include "Animation/Skeleton.h"

UCharacterGenPipeline::UCharacterGenPipeline()
{
	bIsCancelled = false;
	bIsRunning = false;
	GeneratedRawMesh = nullptr;
	MetaHumanSkeleton = nullptr;
	HeadSkeletalMesh = nullptr;
	BodySkeletalMesh = nullptr;
}

void UCharacterGenPipeline::GenerateCharacter(
	const TArray<UTexture2D*>& InputImages,
	const FCharacterGenSettings& Settings,
	const FOnCharacterGenComplete& OnComplete,
	const FOnCharacterGenError& OnError)
{
	if (InputImages.IsEmpty())
	{
		OnError.ExecuteIfBound(TEXT("No input images provided"));
		return;
	}

	CurrentSettings = Settings;
	CompletionCallback = OnComplete;
	ErrorCallback = OnError;
	bIsCancelled = false;
	bIsRunning = true;

	UpdateProgress(ECharacterGenState::ImageTo3D, 0.1f, TEXT("Starting Image to 3D generation"));
	ExecuteImageTo3D(InputImages);
}

void UCharacterGenPipeline::GenerateCharacterAsync(
	const TArray<UTexture2D*>& InputImages,
	const FCharacterGenSettings& Settings)
{
	CurrentSettings = Settings;
	bIsCancelled = false;
	bIsRunning = true;

	UpdateProgress(ECharacterGenState::ImageTo3D, 0.1f, TEXT("Starting Image to 3D generation"));
	ExecuteImageTo3D(InputImages);
}

FCharacterGenProgress UCharacterGenPipeline::GetProgress() const
{
	return CurrentProgress;
}

void UCharacterGenPipeline::SetQualityLevel(ECharacterGenQuality Quality)
{
	CurrentSettings.Quality = Quality;
}

void UCharacterGenPipeline::Cancel()
{
	bIsCancelled = true;
	bIsRunning = false;
	UpdateProgress(ECharacterGenState::Idle, 0.0f, TEXT("Cancelled"));
}

bool UCharacterGenPipeline::IsRunning() const
{
	return bIsRunning;
}

void UCharacterGenPipeline::ExecuteImageTo3D(const TArray<UTexture2D*>& InputImages)
{
	UpdateProgress(ECharacterGenState::ImageTo3D, 0.3f, TEXT("Running TRELLIS2 inference"), TEXT("Processing images with AI model"));
	
	// TODO: Integrate with Python subprocess for TRELLIS2
	// This will call the Python inference script and load the resulting GLB
	
	// Placeholder: simulate completion
	if (bIsCancelled)
	{
		OnError(TEXT("Operation cancelled"));
		return;
	}

	// After receiving mesh from TRELLIS2:
	UStaticMesh* PlaceholderMesh = nullptr;
	OnImageTo3DComplete(PlaceholderMesh);
}

void UCharacterGenPipeline::ExecuteMeshPostProcess(UStaticMesh* RawMesh)
{
	UpdateProgress(ECharacterGenState::MeshPostProcess, 0.5f, TEXT("Mesh post-processing"), TEXT("Running Hunyuan3D-Part decomposition"));
	
	if (bIsCancelled)
	{
		OnError(TEXT("Operation cancelled"));
		return;
	}

	// TODO: Run Hunyuan3D-Part decomposition
	// TODO: Mesh topology correction
	// TODO: Mesh simplification
	
	TArray<FMeshPart> PlaceholderParts;
	OnMeshPostProcessComplete(PlaceholderParts);
}

void UCharacterGenPipeline::ExecuteMetaHumanFit(const TArray<FMeshPart>& MeshParts)
{
	UpdateProgress(ECharacterGenState::MetaHumanFit, 0.7f, TEXT("Fitting to MetaHuman topology"), TEXT("Matching mesh to MetaHuman template"));
	
	if (bIsCancelled)
	{
		OnError(TEXT("Operation cancelled"));
		return;
	}

	// TODO: Face/body landmark detection
	// TODO: Template matching
	// TODO: Texture transfer
	
	UStaticMesh* PlaceholderHead = nullptr;
	UStaticMesh* PlaceholderBody = nullptr;
	OnMetaHumanFitComplete(PlaceholderHead, PlaceholderBody);
}

void UCharacterGenPipeline::ExecuteSkeletonBinding(UStaticMesh* HeadMesh, UStaticMesh* BodyMesh)
{
	UpdateProgress(ECharacterGenState::SkeletonBinding, 0.9f, TEXT("Creating skeletal mesh"), TEXT("Applying bone weights and skeleton"));
	
	if (bIsCancelled)
	{
		OnError(TEXT("Operation cancelled"));
		return;
	}

	// TODO: Create USkeletalMesh
	// TODO: Apply automatic skinning
	// TODO: Assign skeleton
	
	USkeletalMesh* PlaceholderHeadSkeletal = nullptr;
	USkeletalMesh* PlaceholderBodySkeletal = nullptr;
	OnSkeletonBindingComplete(PlaceholderHeadSkeletal, PlaceholderBodySkeletal);
}

void UCharacterGenPipeline::OnImageTo3DComplete(UStaticMesh* GeneratedMesh)
{
	if (bIsCancelled) return;

	GeneratedRawMesh = GeneratedMesh;
	UpdateProgress(ECharacterGenState::MeshPostProcess, 0.4f, TEXT("Starting mesh post-processing"));
	ExecuteMeshPostProcess(GeneratedRawMesh);
}

void UCharacterGenPipeline::OnMeshPostProcessComplete(const TArray<FMeshPart>& Parts)
{
	if (bIsCancelled) return;

	DecomposedParts = Parts;
	UpdateProgress(ECharacterGenState::MetaHumanFit, 0.6f, TEXT("Starting MetaHuman fitting"));
	ExecuteMetaHumanFit(DecomposedParts);
}

void UCharacterGenPipeline::OnMetaHumanFitComplete(UStaticMesh* HeadMesh, UStaticMesh* BodyMesh)
{
	if (bIsCancelled) return;

	FittedHeadMesh = HeadMesh;
	FittedBodyMesh = BodyMesh;
	UpdateProgress(ECharacterGenState::SkeletonBinding, 0.8f, TEXT("Starting skeleton binding"));
	ExecuteSkeletonBinding(FittedHeadMesh, FittedBodyMesh);
}

void UCharacterGenPipeline::OnSkeletonBindingComplete(USkeletalMesh* HeadSkeletal, USkeletalMesh* BodySkeletal)
{
	if (bIsCancelled) return;

	HeadSkeletalMesh = HeadSkeletal;
	BodySkeletalMesh = BodySkeletal;

	FGeneratedCharacter Result;
	Result.HeadMesh = FittedHeadMesh;
	Result.BodyMesh = FittedBodyMesh;
	Result.HeadSkeletalMesh = HeadSkeletalMesh;
	Result.BodySkeletalMesh = BodySkeletalMesh;
	Result.Skeleton = MetaHumanSkeleton;

	UpdateProgress(ECharacterGenState::Completed, 1.0f, TEXT("Completed"), TEXT("Character generation complete"));
	bIsRunning = false;

	CompletionCallback.ExecuteIfBound(Result);
}

void UCharacterGenPipeline::OnError(const FString& ErrorMessage)
{
	bIsRunning = false;
	UpdateProgress(ECharacterGenState::Failed, 0.0f, TEXT("Failed"), ErrorMessage);
	ErrorCallback.ExecuteIfBound(ErrorMessage);
}

void UCharacterGenPipeline::UpdateProgress(ECharacterGenState State, float Progress, const FString& Step, const FString& Message)
{
	CurrentProgress.State = State;
	CurrentProgress.Progress = Progress;
	CurrentProgress.CurrentStep = Step;
	CurrentProgress.Message = Message;
}
