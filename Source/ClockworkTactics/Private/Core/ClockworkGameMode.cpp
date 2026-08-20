// Required
#include "Core/ClockworkGameMode.h"


//-------------------------
// Constructor
//-------------------------

AClockworkGameMode::AClockworkGameMode() :
	Super(),
	CharacterClass(AClockworkCharacter::StaticClass())
{
	DefaultPawnClass = CharacterClass;
}

