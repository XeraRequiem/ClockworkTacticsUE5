#pragma once

// Engine
#include "CoreMinimal.h"

// Game
#include "Entity/ClockworkHexEntity.h"
#include "Grid/ClockworkHex.h"

// Generated
#include "ClockworkEntitySpawnerComponent.generated.h"


// -------------------------
// --- Classes
// -------------------------

UCLASS(ClassGroup = (Clockwork), BlueprintType, Meta = (BlueprintSpawnableComponent, ShortTooltip = "An EntitySpawner Component must be added to a Hex to allow Entities to spawn on it."), MinimalAPI)
class UClockworkEntitySpawnerComponent : public USceneComponent
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Clockwork Hex Entity Spawner")
	TSubclassOf<AClockworkHexEntity> EntityClassToSpawn;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Clockwork Hex Entity Spawner")
	float SpawnInterval{ 5.0f };

private:
	float TimeSinceLastSpawn{ 0.0f };


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	UClockworkEntitySpawnerComponent();


	// -------------------------
	// --- Inherited
	// -------------------------

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
