// Required
#include "Core/ClockworkGameInstance.h"

// Game
#include "Entity/ClockworkHexEntityFactory.h"


//-------------------------
// Constructor
//-------------------------

UClockworkGameInstance::UClockworkGameInstance()
{
	ClockworkEntityFactory = NewObject<UClockworkHexEntityFactory>(this, UClockworkHexEntityFactory::StaticClass(), TEXT("ClockworkEntityFactory"));
	ClockworkEntityFactory->Initialize();
}
