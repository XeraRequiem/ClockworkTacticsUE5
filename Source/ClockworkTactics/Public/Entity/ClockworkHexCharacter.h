#pragma once

// Engine
#include "CoreMinimal.h"

// Game
#include "Entity/ClockworkHexUnit.h"

// Generated
#include "ClockworkHexCharacter.generated.h"


// -------------------------
// --- Structs
// -------------------------


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
	 UPROPERTY()
	 TObjectPtr<AClockworkWeapon> EquippedWeapon;
	 */

	 /*
	 UPROPERTY()
	 TObjectPtr<AClockworkArmor> EquippedArmor;
	 */

	 /*
	 UPROPERTY()
	 TObjectPtr<AClockworkAccessory> EquippedAccessory;
	 */

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


	// -------------------------
	// --- API
	// -------------------------

public:


	// -------------------------
	// --- Const API
	// -------------------------

public:


	// -------------------------
	// --- Implementation
	// -------------------------

protected:


	// -------------------------
	// --- Blueprint Events
	// -------------------------

protected:


	// -------------------------
	// --- Event Handlers
	// -------------------------

};
