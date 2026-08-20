// Required
#include "HexLibrary.h"


// -------------------------
// --- Log Category
// -------------------------

DEFINE_LOG_CATEGORY(LogHex);


// -------------------------
// --- Class
// -------------------------

void IHexLibraryModule::StartupModule()
{
	 // This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void IHexLibraryModule::ShutdownModule()
{
	 // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	 // we call this function before unloading the module.
}


// -------------------------
// --- Module Implementation
// -------------------------

IMPLEMENT_MODULE(IHexLibraryModule, HexLibrary);
