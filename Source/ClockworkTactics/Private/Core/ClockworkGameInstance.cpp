// Required
#include "Core/ClockworkGameInstance.h"

// Game
#include "Entity/ClockworkHexEntityFactory.h"


//-------------------------
// Constructor
//-------------------------

UClockworkGameInstance::UClockworkGameInstance()
{
	// Entity Factory
	ClockworkEntityFactory = NewObject<UClockworkHexEntityFactory>(this, UClockworkHexEntityFactory::StaticClass(), TEXT("ClockworkEntityFactory"));
	ClockworkEntityFactory->Initialize();

	// Event Recorder
	ClockworkEventRecorder = NewObject<UClockworkEventRecorder>(this, UClockworkEventRecorder::StaticClass(), TEXT("ClockworkEventRecorder"));
}
