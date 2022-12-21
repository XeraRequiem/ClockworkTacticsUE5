#pragma once

// Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

#include "Clockwork_Unit.generated.h"

UCLASS()
class AClockwork_Unit : public AActor
{
	GENERATED_BODY()


	//-------------------------
	// Member Variables
	//-------------------------

protected:
	/* Current Hex */
	UPROPERTY(Replicated, BlueprintReadOnly)
	AClockwork_Hex* CurrentHex;

	/* Movement Target Hex */
	UPROPERTY(Replicated, BlueprintReadOnly)
	AClockwork_Hex* ReservedHex;

	/* Hexes */
	UPROPERTY(BlueprintReadOnly)
	uint8 AttackRange;

	/* Tiles/Second */
	UPROPERTY(BlueprintReadOnly)
	uint8 MovementSpeed


	//-------------------------
	// Constructors
	//-------------------------

public:
	AClockwork_Unit();


	//-------------------------
	// Inherited
	//-------------------------

public:
	void Tick(float delta) override;

	void GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const override;


	//-------------------------
	// Implementation
	//-------------------------

protected:


	//-------------------------
	// Blueprint Events
	//-------------------------

protected:

};

