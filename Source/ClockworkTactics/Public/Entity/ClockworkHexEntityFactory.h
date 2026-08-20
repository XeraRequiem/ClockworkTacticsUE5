#pragma once

// Engine
#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Engine/Texture2D.h"

// Plugin
#include "Core/HexMath.h"

// Generated
#include "ClockworkHexEntityFactory.generated.h"


class AClockworkHexEntity;


// -------------------------
// --- Enums
// -------------------------

UENUM()
enum class EClockworkHexEntityType : uint8
{
	Invalid,
	Entity,
	Unit,
	Structure,
	Character
};


// -------------------------
// --- Datatables
// -------------------------

USTRUCT(BlueprintType)
struct FClockworkHexEntityTableData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EntityId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FriendlyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IconPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPassable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTargetable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDestructible;
};

USTRUCT(BlueprintType)
struct FClockworkHexUnitTableData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EntityId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth{ 100.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicalResistance{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PhysicalDefense{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PhysicalAttackPower{ 10.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicalAttackPower{ 0.0f };
};

USTRUCT(BlueprintType)
struct FClockworkHexCharacterTableData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EntityId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDurability{ 100.0f };
};

USTRUCT(BlueprintType)
struct FClockworkHexStructureTableData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EntityId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDurability{ 100.0f };
};


// -------------------------
// --- Classes
// -------------------------

UCLASS(Config="Game", BlueprintType)
class UClockworkHexEntityFactory : public UObject
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDataTable> EntityDataTable;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDataTable> UnitDataTable;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDataTable> StructureDataTable;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDataTable> CharacterDataTable;


	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<AClockworkHexEntity>> Entities;

	UPROPERTY(Config)
	FString EntityDataPath{ FPaths::Combine(FPaths::ProjectDir(), TEXT("Resources"), TEXT("Data"), TEXT("Entity"), TEXT("Entities.json")) };


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	UClockworkHexEntityFactory();


	// -------------------------
	// --- Inherited
	// -------------------------

public:


	// -------------------------
	// --- API
	// -------------------------

public:
	void Initialize();


	// -------------------------
	// --- Const API
	// -------------------------

public:
	UDataTable* GetEntityDataTable() const;
	UDataTable* GetUnitDataTable() const;
	UDataTable* GetStructureDataTable() const;
	UDataTable* GetCharacterDataTable() const;


	// -------------------------
	// --- Implementation
	// -------------------------

protected:
	void CreateEntityDatatables();
	UDataTable* CreateEntityDatatable(const FString TableName);

	void GenerateEntities();
	void GenerateEntity(TSharedPtr<FJsonObject> JsonData);
	void GenerateEntityByType(TSubclassOf<AClockworkHexEntity> EntityClass, int32 EntityId, FOffsetCoordinate Coordinate, TSharedPtr<FJsonObject> JsonData);
};
