#pragma once

// Engine
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Plugin
#include "Core/HexMath.h"

// Game
#include "Entity/ClockworkHexEntity.h"

// Generated
#include "ClockworkHex.generated.h"


// -------------------------
// --- Enums
// -------------------------

UENUM(BlueprintType, Blueprintable)
enum class EOccupationStatus : uint8
{
	Vacant,
	Reserved,
	Occupied
};


// -------------------------
// --- Classes
// -------------------------

/*
* Horizonal Row Oriented Hexagonal Tile
*/
UCLASS()
class  AClockworkHex : public AActor
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

protected:
	// Components
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> HexMesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<USceneComponent> OccupationLocation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Height{ 1.0 };

	// Occupation

	UPROPERTY(BlueprintReadOnly)
	EOccupationStatus OccupationStatus { EOccupationStatus::Vacant };

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AClockworkHexEntity> Occupant;

	// Grid

	UPROPERTY(BlueprintReadOnly)
	FOffsetCoordinate Coordinate;

	// Debug

	UPROPERTY(BlueprintReadOnly)
	FString FriendlyName;

	UPROPERTY(BlueprintReadOnly)
	bool bDebugMode;


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	AClockworkHex();


	// -------------------------
	// --- API
	// -------------------------

public:
	UFUNCTION()
	void InitializeTile(FOffsetCoordinate InCoordinate, bool bInDebugMode = false);


	UFUNCTION()
	bool Reserve(AClockworkHexEntity* Actor);

	UFUNCTION()
	bool Occupy(AClockworkHexEntity* Actor);

	UFUNCTION()
	bool Vacate(AClockworkHexEntity* Actor);


	// -------------------------
	// --- Const API
	// -------------------------

public:
	// Occupation Status Queries

	UFUNCTION(BlueprintPure)
	EOccupationStatus GetOccupationStatus() const;

	UFUNCTION(BlueprintPure)
	bool IsOccupied() const;

	UFUNCTION(BlueprintPure)
	bool IsReserved() const;

	UFUNCTION(BlueprintPure)
	bool IsVacant() const;

	// Occupant Queries
	
	UFUNCTION(BlueprintPure)
	AClockworkHexEntity* GetOccupant() const;

	UFUNCTION(BlueprintPure)
	FVector GetOccupationLocation() const;

	UFUNCTION()
	bool CanBeReservedBy(const AClockworkHexEntity* Actor) const;

	UFUNCTION()
	bool CanBeOccupiedBy(const AClockworkHexEntity* Actor) const;

	UFUNCTION()
	bool CanBeVacatedBy(const AClockworkHexEntity* Actor) const;

	// Grid Queries

	UFUNCTION(BlueprintPure)
	FOffsetCoordinate GetGridCoordinate() const;

	// Geometric Queries

	UFUNCTION(BlueprintPure)
	float GetCircumradius() const;

	UFUNCTION(BlueprintPure)
	float GetInradius() const;

	UFUNCTION(BlueprintPure)
	float GetMaximalDiameter() const;

	UFUNCTION(BlueprintPure)
	float GetMinimalDiameter() const;

	// Debug

	UFUNCTION(BlueprintPure)
	FString GetFriendlyName() const;

	// -------------------------
	// --- Blueprint Events
	// -------------------------

protected:
	UFUNCTION(BlueprintImplementableEvent)
	bool OnReserved(AClockworkHexEntity* Entity);

	UFUNCTION(BlueprintImplementableEvent)
	bool OnOccupied(AClockworkHexEntity* Entity);

	UFUNCTION(BlueprintImplementableEvent)
	bool OnVacated(AClockworkHexEntity* Entity);


	// -------------------------
	// --- Debugging
	// -------------------------

public:
	UFUNCTION()
	void Debug_DrawCoordinates();

	UFUNCTION()
	void Debug_SetText(const FString& DebugText);

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Debug_OnTextSet(const FString& DebugText);
};