#pragma once

// Engine
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Plugin
#include "Core/HexMath.h"
#include "Entity/ClockworkHexEntity.h"
#include "Grid/ClockworkGrid.h"

// Generated
#include "ClockworkTile.generated.h"

class AClockworkGrid;


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
class  AClockworkTile : public AActor
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<USceneComponent> OccupationLocation;

	UPROPERTY(BLueprintReadOnly, EditAnywhere)
	float Height{ 1.0 };

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float DefaultDepth{ 1.0 };

	UPROPERTY(BlueprintReadOnly)
	EOccupationStatus OccupationStatus;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AClockworkHexEntity> Occupant;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AClockworkGrid> OwningGrid;

	UPROPERTY(BlueprintReadOnly)
	FOffsetCoordinate Coordinate;

	UPROPERTY(BlueprintReadOnly)
	bool bDebugMode;


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	AClockworkTile();


	// -------------------------
	// --- Inherited
	// -------------------------

public:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


	// -------------------------
	// --- API
	// -------------------------

public:
	UFUNCTION()
	void InitializeTile(AClockworkGrid* InGrid, FOffsetCoordinate InCoordinate, bool bInDebugMode);


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
	UFUNCTION(BlueprintPure)
	EOccupationStatus GetOccupationStatus() const;

	UFUNCTION(BlueprintPure)
	AClockworkHexEntity* GetOccupant() const;

	UFUNCTION(BlueprintPure)
	FVector GetOccupationLocation() const;


	UFUNCTION(BlueprintPure)
	FOffsetCoordinate GetCoordinate() const;


	UFUNCTION(BlueprintPure)
	bool IsOccupied() const;

	UFUNCTION(BlueprintPure)
	bool IsReserved() const;

	UFUNCTION(BlueprintPure)
	bool IsVacant() const;

		
	UFUNCTION()
	bool CanBeReservedBy(const AClockworkHexEntity* Actor) const;

	UFUNCTION()
	bool CanBeOccupiedBy(const AClockworkHexEntity* Actor) const;

	UFUNCTION()
	bool CanBeVacatedBy(const AClockworkHexEntity* Actor) const;


	UFUNCTION(BlueprintPure)
	AClockworkGrid* GetOwningGrid() const;

	UFUNCTION(BlueprintPure)
	FOffsetCoordinate GetGridCoordinate() const;


	UFUNCTION(BlueprintPure)
	float GetCircumradius() const;

	UFUNCTION(BlueprintPure)
	float GetInradius() const;

	UFUNCTION(BlueprintPure)
	float GetMaximalDiameter() const;

	UFUNCTION(BlueprintPure)
	float GetMinimalDiameter() const;


	// -------------------------
	// --- Blueprint Events
	// -------------------------

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetColorByOccupationStatus();

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