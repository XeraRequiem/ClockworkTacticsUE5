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
	/*
	* An array of entities that are within the character's attack range. Sorted by Priority: Units closest to their target first, then other entities closest to the character.
	*/
	UPROPERTY(BlueprintReadOnly)
	TArray<AClockworkHexEntity*> EntitiesWithinRange;

	/*
	* The maximum number of targets this character can attack at once.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 MaxTargets{ 1 };

	UPROPERTY()
	uint16 CurrentExperience{ 0 };


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


	TArray<AClockworkHexEntity*> GetTargetableEntitiesWithinRange(uint8 Range);

	TArray<FOffsetCoordinate> GetCoordinatesInRange();


	// -------------------------
	// --- Implementation
	// -------------------------

private:
	void Debug_PaintHexesInRange(TArray<AClockworkHex*> Hexes);
};
