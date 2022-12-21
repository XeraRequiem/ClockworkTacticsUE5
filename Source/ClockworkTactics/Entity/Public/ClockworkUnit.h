#pragma once

// Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

// Game
#include "HexTile.h"

#include "ClockworkUnit.generated.h"


// -------------------------
// --- Enums
// -------------------------

enum class EClockworkState : uint8
{
	IDLE,
	MOVING,
	ATTACKINGs
};


// -------------------------
// --- Classes
// -------------------------

UCLASS()
class AClockworkUnit : public AActor
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly)
	AHexTile* CurrentHex;

	UPROPERTY(BlueprintReadOnly)
	AHexTile* ReservedHex;

	UPROPERTY(BlueprintReadOnly)
	AClockworkUnit* TargetedUnit;

	/* Temp */
	UPROPERTY(BlueprintReadOnly)
	AHexTile* TargetedHex;

	UPROPERTY(BlueprintReadOnly)
	bool bMoving;

	/* --------------- Stats --------------- */

	/* Tiles/Second */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MoveSpeed;

	/* Measured in Hexes */
	UPROPERTY(BlueprintReadOnly)
	uint8 AttackRange;

	UPROPERTY(BlueprintReadOnly)
	float AttackDamage;

	UPROPERTY(BlueprintReadOnly)
	float MagicDamage;

	/* Attacks/Second */
	UPROPERTY(BlueprintReadOnly)
	float AttackSpeed;

	UPROPERTY(BlueprintReadOnly)
	float PhysicalDefense;

	UPROPERTY(BlueprintReadOnly)
	float MagicResistance;

private:
	float MoveTimeTotal;

	bool bDebugMode;


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	AClockworkUnit();


	// -------------------------
	// --- Inherited
	// -------------------------

public:
	void Tick(float dt) override;


	// -------------------------
	// --- API
	// -------------------------

public:
	UFUNCTION(BlueprintCallable)
	void InitializeUnit(AHexTile* hex, bool bDebugMode = false);


	UFUNCTION(BlueprintCallable)
	bool TargetHex(AHexTile* hex);

	UFUNCTION(BlueprintCallable)
	bool VacateHex(AHexTile* hex);
	
	UFUNCTION(BlueprintCallable)
	bool ReserveHex(AHexTile* hex);
	
	UFUNCTION(BlueprintCallable)
	bool OccupyHex(AHexTile* hex);


	// -------------------------
	// --- Const API
	// -------------------------

public:
	UFUNCTION(BlueprintCallable)
	AHexTile* GetOccupiedHex() const;


	// -------------------------
	// --- Implementation
	// -------------------------

protected:
	UFUNCTION()
	void Move(float dt);

	UFUNCTION(BlueprintCallable)
	void MoveToReservedHex(float dt);

	UFUNCTION(BlueprintCallable)
	TArray<AHexTile*> GetPathToHex(const AHexTile* hex);

	UFUNCTION(BlueprintCallable)
	TArray<AHexTile*> GetPathToHexWithinTargetRange();

	UFUNCTION(BlueprintCallable)
	AClockworkUnit* FindClosestEnemyUnit();


	// -------------------------
	// --- Blueprint Events
	// -------------------------

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnInitialized(AHexTile* hex);


	UFUNCTION(BlueprintImplementableEvent)
	void OnHexTargeted(AHexTile* hex);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHexVacated(AHexTile* hex);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHexReserved(AHexTile* hex);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHexOccupied(AHexTile* hex);


	// -------------------------
	// --- Event Handlers
	// -------------------------

	UFUNCTION()
	void HandleHexTargeted(AHexTile* hex);
	
	UFUNCTION()
	void HandleHexVacated(AHexTile* hex);
	
	UFUNCTION()
	void HandleHexReserved(AHexTile* hex);
	
	UFUNCTION()
	void HandleHexOccupied(AHexTile* hex);
};
