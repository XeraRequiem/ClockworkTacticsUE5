#pragma once

// Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

// Generated
#include "HexTile.generated.h"

class AHexGrid;


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
class AHexTile : public AActor
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	USceneComponent* OccupationLocation;

	UPROPERTY(BLueprintReadOnly, EditAnywhere)
	float Height;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float DefaultDepth;

	UPROPERTY(BlueprintReadOnly, Replicated)
	EOccupationStatus OccupationStatus;

	UPROPERTY(BlueprintReadOnly)
	AClockworkUnit* Occupant;

	UPROPERTY(BlueprintReadOnly)
	AHexGrid* OwningGrid;

private:
	FVector2D Coordinate;

	bool bDebugMode;


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	AHexTile();


	// -------------------------
	// --- Inherited
	// -------------------------

public:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


	// -------------------------
	// --- API
	// -------------------------

public:
	UFUNCTION(NetMulticast, Reliable)
	void InitializeTile(AHexGrid* grid);


	UFUNCTION()
	bool Reserve(AClockworkUnit* clockwork);

	UFUNCTION()
	bool Occupy(AClockworkUnit* clockwork);

	UFUNCTION()
	bool Vacate(AClockworkUnit* clockwork);


	// -------------------------
	// --- Const API
	// -------------------------

public:
	UFUNCTION(BlueprintPure)
	EOccupationStatus GetOccupationStatus() const;


	UFUNCTION(BlueprintPure)
	AClockworkUnit* GetOccupant() const;

	UFUNCTION(BlueprintPure)
	FVector GetOccupationLocation() const;


	UFUNCTION(BlueprintPure)
	bool IsOccupied() const;

	UFUNCTION(BlueprintPure)
	bool IsReserved() const;

	UFUNCTION(BlueprintPure)
	bool IsVacant() const;


	UFUNCTION()
	bool CanBeReservedBy(const AClockworkUnit* clockwork) const;

	UFUNCTION()
	bool CanBeOccupiedBy(const AClockworkUnit* clockwork) const;

	UFUNCTION()
	bool CanBeVacatedBy(const AClockworkUnit* clockwork) const;


	UFUNCTION(BlueprintPure)
	AHexGrid* GetOwningGrid() const;

	UFUNCTION(BlueprintPure)
	FVector2D GetGridCoordinate() const;


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
	bool OnReserved(AClockworkUnit* clockwork);

	UFUNCTION(BlueprintImplementableEvent)
	bool OnOccupied(AClockworkUnit* clockwork);

	UFUNCTION(BlueprintImplementableEvent)
	bool OnVacated(AClockworkUnit* clockwork);


	// -------------------------
	// --- Debugging
	// -------------------------

public:
	UFUNCTION()
	void Debug_SetText(const FString& text);

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Debug_OnTextSet(const FString& text);
};
