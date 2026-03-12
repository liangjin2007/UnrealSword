rmdir /s /q "Binaries" >nul 2>&1
rmdir /s /q "Intermediate" >nul 2>&1
rmdir /s /q "Saved" >nul 2>&1
rmdir /s /q "DerivedDataCache" >nul 2>&1

rmdir /s /q "Plugins/UnrealClaude/UnrealClaude/Binaries" >nul 2>&1
rmdir /s /q "Plugins/UnrealClaude/UnrealClaude/Intermediate" >nul 2>&1

rmdir /s /q "Plugins/UnrealGenAISupport/Binaries" >nul 2>&1
rmdir /s /q "Plugins/UnrealGenAISupport/Intermediate" >nul 2>&1

echo Delete Binaries and Intermediate finished
pause