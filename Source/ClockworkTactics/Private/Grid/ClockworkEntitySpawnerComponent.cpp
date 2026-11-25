// Required
#include "Grid/ClockworkEntitySpawnerComponent.h"

// Game
#include "Core/ClockworkEventRecorder.h"
#include "Core/ClockworkGameInstance.h"
#include "Core/ClockworkTactics.h"
#include "Core/ClockworkWorldSubsystem.h"
#include "Grid/ClockworkGrid.h"
#include "Grid/ClockworkHex.h"


// -------------------------
// --- Constructors
// -------------------------

UClockworkEntitySpawnerComponent::UClockworkEntitySpawnerComponent() :
	 Super()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
}


// -------------------------
// --- Inherited
// -------------------------

void UClockworkEntitySpawnerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastSpawn += DeltaTime;

	if (EntityClassToSpawn != nullptr && TimeSinceLastSpawn >= SpawnInterval)
	{
		// Spawn entity logic here
		TimeSinceLastSpawn = 0.0f;

		UClockworkWorldSubsystem* WorldSubsystem = GetWorld()->GetSubsystem<UClockworkWorldSubsystem>();
		if (WorldSubsystem != nullptr)
		{
			AClockworkGrid* HexGrid = WorldSubsystem->GetHexGrid();
			if (HexGrid != nullptr)
			{
				AClockworkHex* OwningHex = StaticCast<AClockworkHex*>(GetOwner());

				if (OwningHex != nullptr)
				{
					FInstancedStruct EventData = FInstancedStruct::Make<FClockworkSpawnEventData>(EntityClassToSpawn->GetDefaultObject<AClockworkHexEntity>()->GetFriendlyName(), OwningHex->GetGridCoordinate());
					OwningHex->GetGameInstance<UClockworkGameInstance>()->GetClockworkEventRecorder()->GenerateEvent(EClockworkEventType::Spawn, EventData);

					HexGrid->SpawnEntityOnHex(EntityClassToSpawn, OwningHex);
				}
			}
		}
	}
}
