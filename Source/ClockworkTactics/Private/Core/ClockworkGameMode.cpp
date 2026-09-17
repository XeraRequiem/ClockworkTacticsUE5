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


//-------------------------
// Public API
//-------------------------

void AClockworkGameMode::NotifyEntityDestruction(const FClockworkEntityDeathData& DeathData)
{
	UE_LOG(LogGameMode, Verbose, TEXT("%s Killed %s"), *DeathData.KillerEntity, *DeathData.DeadEntity);

	TicketData.AddTickets();
}
