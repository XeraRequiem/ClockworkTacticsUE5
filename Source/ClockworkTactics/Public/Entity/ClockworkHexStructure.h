#pragma once

// Engine
#include "CoreMinimal.h"

// Game
#include "Entity/ClockworkHexEntity.h"

// Generated
#include "ClockworkHexStructure.generated.h"


// -------------------------
// --- Structs
// -------------------------

USTRUCT(BlueprintType)
struct FClockworkHexStructureData
{
	GENERATED_BODY()

	/* Hit Point Capacity */
	UPROPERTY(BlueprintReadOnly)
	float MaxDurability;

	/* Hit Point Capacity */
	UPROPERTY(BlueprintReadOnly)
	float CurrentDurability;
};


// -------------------------
// --- Classes
// -------------------------

UCLASS()
class AClockworkHexStructure : public AClockworkHexEntity
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

protected:
	FClockworkHexStructureData StructureData;


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	 AClockworkHexStructure();


	// -------------------------
	// --- Inherited
	// -------------------------

public:
	virtual void ParseJsonData(const TSharedPtr<FJsonObject>& EntityJson) override;
	virtual void ParseTableData() override;


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
