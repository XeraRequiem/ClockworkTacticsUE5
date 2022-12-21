#pragma once

// Engine
#include "GameFramework/Actor.h"

// Game
#include "ClockworkTactics_HexLibrary.h"
#include "ClockworkUnit.h"
#include "HexTile.h"

// Generated
#include "HexGrid.generated.h"


/*
* "Odd-R" Horizontal Layout Grid Consisting of Horizontally-Oriented Hex Tiles. Coordinates Start From Bottom-Left
*/
UCLASS(Config=Game)
class AHexGrid : public AActor
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

protected:
	UPROPERTY(EditDefaultsOnly, Replicated)
	TSubclassOf<AHexTile> HexClass;

	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AClockworkUnit>, uint8> DebugClockworkClassesToSpawn;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated)
	uint8 GridHexWidth;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated)
	uint8 GridHexDepth;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Replicated)
	float GridHexSpacing;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Replicated)
	TArray<AHexTile*> Hexes;

private:
	UPROPERTY(Config)
	bool bDebugMode;


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	AHexGrid();


	// -------------------------
	// --- Inherited
	// -------------------------

public:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


	// -------------------------
	// --- API
	// -------------------------

public:
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void GenerateHexGrid();


	// -------------------------
	// --- Const API
	// -------------------------

public:
	UFUNCTION(BlueprintCallable)
	AHexTile* GetHexAtOffsetCoordinate(const FOffsetCoordinate& coordinate) const;

	UFUNCTION(BlueprintCallable)
	AHexTile* GetHexAtCubeCoordinate(const FCubeCoordinate& coordinate) const;

	UFUNCTION(BlueprintCallable)
	AHexTile* GetHexAtAxialCoordinate(const FAxialCoordinate& coordinate) const;

	UFUNCTION(BlueprintCallable)
	AHexTile* GetHexAtDoubledCoordinate(const FDoubledCoordinate& coordinate) const;


	UFUNCTION(BlueprintCallable)
	AHexTile* GetRandomVacantHex() const;


	UFUNCTION(BlueprintCallable)
	AHexTile* GetOccupiedHexClosestTo(const AHexTile* start) const;

	UFUNCTION(BlueprintCallable)
	AHexTile* GetVacantHexClosestTo(const AHexTile* start) const;


	UFUNCTION(BlueprintCallable)
	AClockworkUnit* GetEnemyUnitClosestTo(const AClockworkUnit* unit) const;

	uint8 HexDistanceBetween(const AHexTile* hex1, const AHexTile* hex2) const;

	UFUNCTION(BlueprintCallable)
	TArray<AHexTile*> GetPathFromTo(AHexTile* start, const AHexTile* target) const;


	// -------------------------
	// --- Implementation
	// -------------------------

protected:
	UFUNCTION(BlueprintCallable)
	void SpawnUnitOnRandomHex(TSubclassOf<AClockworkUnit> clockworkClass);

	UFUNCTION(BlueprintCallable)
	bool SpawnUnitOnHex(TSubclassOf<AClockworkUnit> clockworkClass, AHexTile* hex);


	// -------------------------
	// --- Const Implementation
	// -------------------------

protected:
	UFUNCTION(BlueprintCallable)
	TArray<AHexTile*> GetHexNeighbors(const AHexTile* tile) const;

private:
	AHexTile* GetHexAt(uint8 x, uint8 y) const;


	FOffsetCoordinate GetOffsetCoordinateOfHex(const AHexTile* hex) const;

	FDoubledCoordinate GetDoublesCoordinateOfHex(const AHexTile* hex) const;

	
	bool DetermineHexPathCost(AHexTile* hex, const AHexTile* target, uint8 startDistance, float& cost, const TMap<AHexTile*, uint8>& hexDistanceMap) const;


	// -------------------------
	// --- Blueprint Events
	// -------------------------

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnProcessPathHex(AHexTile* hex);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPathCheckingHexCost(AHexTile* hex);


	// -------------------------
	// --- Debug
	// -------------------------

public:
	UFUNCTION(BLueprintCallable)
	void Debug_DrawHexCoordinates();

	UFUNCTION(BLueprintCallable)
	void Debug_UpdateHexDistanceFrom(AClockworkUnit* clockwork);

	UFUNCTION(BlueprintCallable)
	void Debug_SpawnUnits();
};
