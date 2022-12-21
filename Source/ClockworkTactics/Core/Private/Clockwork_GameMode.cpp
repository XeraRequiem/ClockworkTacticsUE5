#include "Clockwork_GameMode.h"

// Engine
#include "UObject/ConstructorHelpers.h"

// Game
#include "Clockwork_Character.h"

AClockwork_GameMode::AClockwork_GameMode() :
	Super(),
	CharacterClass(AClockwork_Character::StaticClass())
{
	DefaultPawnClass = CharacterClass;
}
