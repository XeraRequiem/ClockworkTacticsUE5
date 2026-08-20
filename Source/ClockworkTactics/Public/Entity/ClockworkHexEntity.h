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


class AClockworkTile;

// -------------------------
// --- Structs
// -------------------------

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


// -------------------------
// --- Classes
// -------------------------

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
	 TObjectPtr<AClockworkTile> OccupiedHex;

	 UPROPERTY(BlueprintReadOnly)
	 TObjectPtr<AClockworkTile> HexToOccupy;


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
	UFUNCTION()
	virtual void Initialize(AClockworkTile* Hex);
	
	virtual void InitializeFromData(int32 InEntityId, const FOffsetCoordinate& InLocation, const TSharedPtr<FJsonObject>& InEntityJson);


	UFUNCTION()
	virtual void OccupyPendingTile();

	UFUNCTION()
	virtual bool OccupyHex(AClockworkTile* Hex);


	// -------------------------
	// --- Const API
	// -------------------------

public:
	 virtual FString SerializeEntityData() const;

	 AClockworkTile* GetOccupiedHex() const;


	// -------------------------
	// --- Implementation
	// -------------------------

protected:
	 bool SpawnEntityAtOccupiedTile();

	 // -------------------- Serialization -------------------- //

	 virtual void ParseJsonData(const TSharedPtr<FJsonObject>& EntityJson);
	 virtual void ParseTableData();
};
