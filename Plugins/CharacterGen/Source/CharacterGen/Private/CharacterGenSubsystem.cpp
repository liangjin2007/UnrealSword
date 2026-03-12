// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterGenSubsystem.h"
#include "Engine/Engine.h"
#include "Misc/Paths.h"

UCharacterGenSubsystem::UCharacterGenSubsystem()
{
	PythonPath = TEXT("python");
}

void UCharacterGenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FString PluginDir = FPaths::Combine(
		FPaths::ProjectPluginsDir(),
		TEXT("CharacterGen"),
		TEXT("Source"),
		TEXT("CharacterGenLoader")
	);

	if (FPaths::FileExists(FPaths::Combine(PluginDir, TEXT("character_gen_launcher.py"))))
	{
		UE_LOG(LogCharacterGen, Log, TEXT("CharacterGen Python launcher found at: %s"), *PluginDir);
	}
}

void UCharacterGenSubsystem::Deinitialize()
{
	for (UCharacterGenPipeline* Pipeline : ActivePipelines)
	{
		if (Pipeline)
		{
			Pipeline->Cancel();
		}
	}
	ActivePipelines.Empty();

	Super::Deinitialize();
}

UCharacterGenSubsystem* UCharacterGenSubsystem::Get(UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		return World->GetSubsystem<UCharacterGenSubsystem>();
	}
	return nullptr;
}

UCharacterGenPipeline* UCharacterGenSubsystem::CreatePipeline()
{
	UCharacterGenPipeline* Pipeline = NewObject<UCharacterGenPipeline>(this);
	ActivePipelines.Add(Pipeline);
	return Pipeline;
}

void UCharacterGenSubsystem::GenerateCharacter(
	UCharacterGenPipeline* Pipeline,
	const TArray<UTexture2D*>& InputImages,
	const FCharacterGenSettings& Settings,
	const FOnCharacterGenComplete& OnComplete,
	const FOnCharacterGenError& OnError)
{
	if (Pipeline)
	{
		Pipeline->GenerateCharacter(InputImages, Settings, OnComplete, OnError);
	}
}

FCharacterGenProgress UCharacterGenSubsystem::GetProgress(UCharacterGenPipeline* Pipeline)
{
	if (Pipeline)
	{
		return Pipeline->GetProgress();
	}
	return FCharacterGenProgress();
}

void UCharacterGenSubsystem::CancelGeneration(UCharacterGenPipeline* Pipeline)
{
	if (Pipeline)
	{
		Pipeline->Cancel();
		ActivePipelines.Remove(Pipeline);
	}
}

bool UCharacterGenSubsystem::IsGenerationRunning(UCharacterGenPipeline* Pipeline) const
{
	return Pipeline && Pipeline->IsRunning();
}

void UCharacterGenSubsystem::SetPythonPath(const FString& Path)
{
	PythonPath = Path;
}

FString UCharacterGenSubsystem::GetPythonPath() const
{
	return PythonPath;
}

bool UCharacterGenSubsystem::ValidatePythonEnvironment() const
{
	FString Result;
	FString ErrResult;

	// Check if python is available
	FPlatformProcess::ExecProcess(
		*PythonPath,
		TEXT("--version"),
		&Result,
		&ErrResult,
		nullptr,
		nullptr
	);

	if (Result.Contains(TEXT("Python")))
	{
		UE_LOG(LogCharacterGen, Log, TEXT("Python found: %s"), *Result.TrimStartAndEnd());
		return true;
	}

	UE_LOG(LogCharacterGen, Error, TEXT("Python not found. Please install Python 3.8+"));
	return false;
}
