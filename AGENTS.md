# AGENTS.md - UnrealSword Development Guide

## Project Overview
- **Engine**: Unreal Engine 5.7
- **Language**: C++ (with potential for Blueprint)
- **Type**: Game/Runtime Application
- **Module**: UnrealSword (Runtime + Editor)

---

## Build Commands

### Using Unreal Build Tool (UBT)
All builds use the Unreal Build Tool via command line or IDE:

```bash
# Build Project
set ENGINE_PATH="C:/Program Files/Epic Games/UE_5.7"
set PROJECT_FILE="D:/UnrealSword/UnrealSword.uproject"
"%ENGINE_PATH%/Engine/Build/BatchFiles/RunUAT.bat" BuildCookRun -nop4 -utf8output -project=%PROJECT_FILE% -target=UnrealSword -unrealexe="%ENGINE_PATH%/Engine/Binaries/Win64/UnrealEditor.exe" -platform=WIN64 -build -clientconfig=Development -specifiedarchitecture=x64
```

### Using UE Editor
1. Open `UnrealSword.uproject` in Unreal Editor 5.7
2. Click **Compile** button in toolbar (or press Ctrl+Alt+F11)
3. Or: File > Package Project > [Platform]

### Clean Build
```bash
# Delete intermediate and saved folders
rmdir /s /q D:\UnrealSword\Intermediate
rmdir /s /q D:\UnrealSword\Saved
```

---

## Testing

### Running Tests
```bash
# Run via Unreal Automation System (UAS)
# In editor: Window > Test > Automation

# Command line (from Engine/Binaries/Win64)
UE4Editor.exe UnrealSword -RunTests -TestLoop

# Specific test category
UE4Editor.exe UnrealSword -RunTests -TestSuite="Functional Tests"
```

### Test Structure
- Place tests in: `Source/UnrealSword/Tests/` or use `WITH_AUTOMATION_TESTS` guards
- Tests inherit from `FAutomationTestBase` or use `IMPLEMENT_SIMPLE_AUTOMATION_TEST`
- Run from **Session Frontend** in Editor: Window > Developer Tools > Session Frontend

---

## Linting & Code Analysis

### Built-in
- **Live Coding**: Editor provides real-time compilation errors
- **Error List**: Shows warnings/errors in Visual Studio or Rider

### External Tools
- **Unreal Engine Header Tool**: `UnrealBuildTool.exe -SkipBuild -HeaderTool`
- **clang-tidy**: Add to build pipeline for additional checks
- **Visual Studio**: Enable Analysis on Solution

---

## Code Style Guidelines

### File Headers
```cpp
// Copyright Epic Games, Inc. All Rights Reserved.
```

### Includes (Unreal C++ Standard)
```cpp
// Core minimal - always first
#include "CoreMinimal.h"

// Module includes
#include "Modules/ModuleManager.h"

// Your project headers
#include "MyActor.h"
#include "MyGameMode.generated.h"
```

### Naming Conventions
- **Classes**: `F` prefix (e.g., `FMyClass`, `AMyActor`, `UMyComponent`)
- **Structs**: `F` prefix (e.g., `FVector`, `FTransform`)
- **Interfaces**: `I` prefix (e.g., `IMyInterface`)
- **Enums**: `E` prefix (e.g., `EMyEnum`)
- **Functions**: PascalCase (e.g., `GetHealth()`, `TakeDamage()`)
- **Variables**: `b` prefix for booleans (e.g., `bIsDead`), camelCase otherwise
- **Constants**: `k` prefix for enums/ints, otherwise PascalCase

### Types
```cpp
// Unreal types (not STL primitives)
int32, int64, uint8, float, double, bool
FString (not std::string)
TArray<T> (not std::vector)
TMap<K, V> (not std::unordered_map)
TSet<T> (not std::set)

// Use UPROPERTY, UFUNCTION, UCLASS, etc.
UCLASS()
class AMyActor : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Health;

    UFUNCTION(BlueprintCallable)
    void TakeDamage(int32 Damage);
};
```

### Error Handling
- Use `UE_LOG()` with appropriate category (LogTemp, LogGameMode, etc.)
- Use `check()`, `ensure()`, or `verify()` for assertions
- Use `TryAcceptablePattern` or return bool for recoverable errors

```cpp
UE_LOG(LogTemp, Warning, TEXT("Failed to load: %s"), *FileName);
checkf(IsValid(MyActor), TEXT("MyActor must be valid"));
ensure(MyCondition); // Non-fatal assertion
```

### Memory & GC
- Use `UPROPERTY()` for objects that need garbage collection
- Prefer `TWeakObjectPtr` for weak references
- Use `NewObject<T>()` or `NewNamedObject<T>()` for creation
- Avoid raw pointers unless absolutely necessary

### Blueprint Integration
```cpp
// Expose to Blueprint
UFUNCTION(BlueprintCallable, BlueprintPure, Category="MyCategory")
int32 GetHealth() const { return Health; }

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MyCategory")
float MyFloat;
```

---

## Project Structure
```
UnrealSword/
├── Config/              # DefaultEngine.ini, etc.
├── Content/             # Blueprints, assets
├── Plugins/             # Plugin modules
├── Source/
│   └── UnrealSword/
│       ├── UnrealSword.h
│       ├── UnrealSword.cpp
│       ├── *.h           # Header files (classes)
│       └── *.cpp         # Implementation
├── UnrealSword.uproject
└── README.md
```

---

## Common Tasks

### Adding New C++ Class
1. In Editor: File > New C++ Class
2. Choose parent (Actor, Pawn, GameModeBase, etc.)
3. Edit generated `.h` and `.cpp`

### Hot Reload
- In Editor: Click Compile button or press Ctrl+Alt+F11
- Or: File > Refresh Visual Studio Project

### Generating Project Files
```bash
# Right-click .uproject > Generate Visual Studio project files
# Or via command line
UnrealBuildTool.exe -projectfiles -project="D:\UnrealSword\UnrealSword.uproject"
```

---

## Editor Integration
- Open `.uproject` in Unreal Engine 5.7
- C++ changes require compile (Ctrl+Alt+F11 or toolbar Compile button)
- Blueprints are in Content/ folder
- Use **Blueprint** for rapid iteration, **C++** for complex logic, performance

---

## Key Unreal Concepts
- **GameInstance**: Persists across level transitions
- **GameMode**: Server-side rules
- **GameState**: Replicated game state
- **PlayerController**: Input/control
- **Pawn**: Physical representation
- **Actor**: Base object (placed in world)

---

## Notes
- Always use `GENERATED_BODY()` (UE5)
- Mark GC-protected objects with `UPROPERTY()`
- Use `BlueprintCallable` sparingly; prefer `BlueprintImplementableEvent`
- Keep C++ classes minimal; use Blueprints for customization
