#pragma once

// Engine
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


// -------------------------
// --- Log Category
// -------------------------

DECLARE_LOG_CATEGORY_EXTERN(LogHex, Log, All);


// -------------------------
// --- Class
// -------------------------

/** IModuleInterface implementation */
class HEXLIBRARY_API IHexLibraryModule : public IModuleInterface
{
public:
	 virtual void StartupModule() override;
	 virtual void ShutdownModule() override;
};
