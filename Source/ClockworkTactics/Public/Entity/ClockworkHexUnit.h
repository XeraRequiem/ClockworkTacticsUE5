#pragma once

// Engine
#include "CoreMinimal.h"

// Game
#include "Entity/ClockworkHexEntity.h"

// Generated
#include "ClockworkHexUnit.generated.h"


// -------------------------
// --- Structs
// -------------------------

USTRUCT(BlueprintType)
struct FClockworkHexUnitData
{
	 GENERATED_BODY()

	 /* Hit Point Capacity */
	 UPROPERTY(BlueprintReadOnly)
	 float Health;

	 /* Physical Damage Resistance */
	 UPROPERTY(BlueprintReadOnly)
	 float Defense;

	 /* Magic Damage Resistance */
	 UPROPERTY(BlueprintReadOnly)
	 float Resistance;

	 /* Physical Damage Per Attack */
	 UPROPERTY(BlueprintReadOnly)
	 float AttackDamage;

	 /* Physical Attack Range (Hexes) */
	 UPROPERTY(BlueprintReadOnly)
	 uint8 AttackRange;
	 	 
	 /* Physical Attacks/Second */
	 UPROPERTY(BlueprintReadOnly)
	 float AttackSpeed;

	 /* Magical Damage Per Spell */
	 UPROPERTY(BlueprintReadOnly)
	 float MagicDamage;	
};


// -------------------------
// --- Classes
// -------------------------

UCLASS()
class AClockworkHexUnit : public AClockworkHexEntity
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

	// -------------------- Serialization -------------------- //

protected:
	/* Unmodified Unit Stats */
	FClockworkHexUnitData UnitData;

	/* Current Unit Stats (modified by buffs, debuffs, etc.) */
	FClockworkHexUnitData ModifiedUnitData;

	uint16 Level{ 1 };

	// -------------------- Hexes -------------------- //
		
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AClockworkTile> TargetedHex;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AClockworkTile> ReservedHex;

	// -------------------- Movement -------------------- //

	/* Tiles/Second */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MoveSpeed{ 0.5f };

	UPROPERTY(BlueprintReadOnly)
	bool bMoving{ false };

	UPROPERTY(BlueprintReadOnly)
	float MoveTimeTotal{ 0.0f };


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	 AClockworkHexUnit();


	// -------------------------
	// --- Inherited
	// -------------------------

public:
	virtual void Tick(float dt) override;

	virtual void ParseJsonData(const TSharedPtr<FJsonObject>& EntityJson) override;
	virtual void ParseTableData() override;

	bool OccupyHex(AClockworkTile* Hex) override;


	// -------------------------
	// --- Const API
	// -------------------------

public:
	UFUNCTION(BlueprintCallable)
	AClockworkTile* GetTargetHex() const;

	UFUNCTION(BlueprintCallable)
	AClockworkTile* GetReservedHex() const;


	// -------------------------
	// --- API
	// -------------------------

public:
	UFUNCTION(BlueprintCallable)
	bool TargetHex(AClockworkTile* Hex);
	
	UFUNCTION(BlueprintCallable)
	bool VacateHex(AClockworkTile* Hex);
	
	UFUNCTION(BlueprintCallable)
	bool ReserveHex(AClockworkTile* Hex);


	// -------------------------
	// --- Implementation
	// -------------------------

protected:
	void Move(float dt);
	void MoveToReservedHex(float dt);

	TArray<AClockworkTile*> GetPathToHexWithinTargetRange();
	TArray<AClockworkTile*> GetPathToHex(AClockworkTile* Hex);


	// -------------------------
	// --- Event Handlers
	// -------------------------

protected:
	UFUNCTION()
	void HandleHexTargeted(AClockworkTile* Hex);
	
	UFUNCTION()
	void HandleHexVacated(AClockworkTile* Hex);
	
	UFUNCTION()
	void HandleHexReserved(AClockworkTile* Hex);
	
	UFUNCTION()
	void HandleHexOccupied(AClockworkTile* Hex);


	// -------------------------
	// --- Blueprint Events
	// -------------------------

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnHexTargeted(AClockworkTile* Hex);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHexVacated(AClockworkTile* Hex);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHexReserved(AClockworkTile* Hex);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHexOccupied(AClockworkTile* Hex);
};
