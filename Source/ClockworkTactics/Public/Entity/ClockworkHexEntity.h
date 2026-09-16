#pragma once

// Engine
#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Engine/Texture2D.h"
#include "GameFramework/Actor.h"

// Plugin
#include "Core/HexMath.h"

// Generated
#include "ClockworkHexEntity.generated.h"


class AClockworkHex;

// -------------------------
// --- Structs
// -------------------------

/*
* 
*/
USTRUCT(BlueprintType)
struct FClockworkHexEntityData
{
	 GENERATED_BODY()

	 UPROPERTY(BlueprintReadOnly)
	 FString IconPath;

	 UPROPERTY(BlueprintReadOnly)
	 FOffsetCoordinate SpawnCoordinate;

	 UPROPERTY(BlueprintReadOnly)
	 bool bPassable{ false };

	 UPROPERTY(BlueprintReadOnly)
	 bool bTargetable{ false };

	 UPROPERTY(BlueprintReadOnly)
	 bool bDestructible{ false };

	 UPROPERTY(BlueprintReadOnly)
	 bool bDebugMode{ false };
};

/*
* 
*/
USTRUCT(BlueprintType)
struct FClockworkDamage
{
	GENERATED_BODY()

	// -------------------------
	// --- Member Variables
	// -------------------------

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	const AClockworkHexEntity* Source;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage{ 0.0f };


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	FClockworkDamage()
	{	}

	FClockworkDamage(const AClockworkHexEntity* InSource, float InDamage) :
		Source(InSource),
		Damage(InDamage)
	{}

};


// -------------------------
// --- Classes
// -------------------------

/*
* 
*/
UCLASS(BlueprintType)
class AClockworkHexEntity : public AActor
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

protected:
	 UPROPERTY(BlueprintReadOnly)
	 FGuid EntityGuid;

	 UPROPERTY(BlueprintReadOnly)
	 int32 EntityId;

	 UPROPERTY(BlueprintReadOnly)
	 FString FriendlyName;


	 UPROPERTY(BlueprintReadOnly)
	 TObjectPtr<UTexture2D> Icon;

	 UPROPERTY(BlueprintReadOnly)
	 TObjectPtr<AClockworkHex> OccupiedHex;

	 UPROPERTY(BlueprintReadOnly)
	 TObjectPtr<AClockworkHex> HexToOccupy;


	 UPROPERTY(BlueprintReadOnly)
	 FClockworkHexEntityData EntityData;


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	 AClockworkHexEntity();


	// -------------------------
	// --- API
	// -------------------------

public:
	
	// ---------- Initialization ---------- //

	UFUNCTION()
	virtual void Initialize(AClockworkHex* Hex);
	
	virtual void InitializeFromData(int32 InEntityId, const FOffsetCoordinate& InLocation, const TSharedPtr<FJsonObject>& InEntityJson);

	// ---------- Occupation ---------- //

	UFUNCTION()
	virtual void OccupyPendingTile();

	UFUNCTION()
	virtual bool OccupyHex(AClockworkHex* Hex);

	// ---------- Damage ---------- //

	UFUNCTION()
	virtual void ApplyDamage(const FClockworkDamage& Damage);

	UFUNCTION()
	virtual void DestroyEntity();

	// -------------------------
	// --- Const API
	// -------------------------

public:
	 virtual FString SerializeEntityData() const;

	 AClockworkHex* GetOccupiedHex() const;

	 FString GetFriendlyName() const;


	// -------------------------
	// --- Implementation
	// -------------------------

protected:
	 bool SpawnEntityAtOccupiedTile();

	 // -------------------- Serialization -------------------- //

	 virtual void ParseJsonData(const TSharedPtr<FJsonObject>& EntityJson);
	 virtual void ParseTableData();
};
