#pragma once

// Engine
#include "CoreMinimal.h"

// Game
#include "Entity/ClockworkHexUnit.h"

// Generated
#include "ClockworkHexCharacter.generated.h"


// -------------------------
// --- Classes
// -------------------------

UCLASS()
class AClockworkHexCharacter : public AClockworkHexUnit
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<AClockworkHexEntity*> TargetEntities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 MaxTargets{ 1 };

	UPROPERTY()
	uint16 CurrentExperience{ 0 };

private:
	float TimeSinceLastLog{ 0.0f };
	 

	// -------------------------
	// --- Constructors
	// -------------------------

public:
	 AClockworkHexCharacter();


	// -------------------------
	// --- Inherited
	// -------------------------

public:
	void Tick(float dt) override;


	// -------------------------
	// --- Implementation
	// -------------------------

protected:
	void UpdateTargetEntitiesInRange();

	TArray<FOffsetCoordinate> GetCoordinatesInRange();

	TArray<AClockworkHexEntity*> GetTargetableEntitiesWithinRange(uint8 Range);


	// -------------------------
	// --- Implementation
	// -------------------------

private:
	void Debug_PaintHexesInRange(TArray<AClockworkHex*> Hexes);
};
