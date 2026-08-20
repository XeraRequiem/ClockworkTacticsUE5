#pragma once

// Engine
#include "GameFramework/Actor.h"

// Plugin
#include "Core/HexMath.h"
#include "Entity/ClockworkHexEntity.h"

// Generated
#include "ClockworkGrid.generated.h"


class AClockworkTile;


// -------------------------
// --- Enum
// -------------------------

UENUM(BlueprintType)
enum CoordinateType : uint8
{
	Offset,
	Doubled,
	Cube,
	Axial
};


// -------------------------
// --- Class
// -------------------------


/*
* "Odd-R" Horizontal Layout Grid Consisting of Horizontally-Oriented Hex Tiles. Coordinates Start From Bottom-Left
*/
UCLASS(Config = Game)
class AClockworkGrid : public AActor
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AClockworkTile> DefaultHexClass;

	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AClockworkHexEntity>, uint8> DebugClockworkHexEntityClasses;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSubclassOf<AClockworkHexEntity>> LayoutEntityClassMap;


	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<AClockworkTile>> Hexes;


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 GridHexWidth{ 0 };

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 GridHexDepth{ 0 };

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float GridHexSpacing{ 1.0f };

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bVariableHeight{ false };
		

	UPROPERTY(Config)
	bool bDebugMode{ true };

	UPROPERTY(Config)
	FString LayoutDataPath{ FPaths::Combine(FPaths::ProjectDir(), TEXT("Resources"), TEXT("Data"), TEXT("Grid"), TEXT("Layouts")) };


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	AClockworkGrid();


	// -------------------------
	// --- Const API
	// -------------------------

	// To-Do: Replace these with a single method that takes a CoordinateType enum and a coordinate struct, and returns the hex at that coordinate.
public:
	UFUNCTION(BlueprintPure)
	AClockworkTile* GetHexAtCoordinate(const FOffsetCoordinate& Coordinate) const;

	UFUNCTION(BlueprintPure)
	AClockworkTile* GetRandomVacantHex() const;


	UFUNCTION(BlueprintPure)
	uint8 HexDistanceBetween(const AClockworkTile* Hex1, const AClockworkTile* Hex2) const;


	// -------------------------	
	// --- API
	// -------------------------

public:
	UFUNCTION(BlueprintCallable)
	void Initialize(int32 InWidth, int32 InDepth, bool bInVariableHeight);

	UFUNCTION(BlueprintCallable)
	void InitializeWithLayout(const FString& LayoutFile);

	UFUNCTION(BlueprintCallable)
	void GenerateGrid();


	UFUNCTION(BlueprintCallable)
	TArray<AClockworkTile*> GetPathFromTo(AClockworkTile* Start, AClockworkTile* Target);

	// To-Do
	UFUNCTION(BlueprintCallable)
	TArray<AClockworkTile*> BidirectionalPathSearch(AClockworkTile* StartHex, AClockworkTile* TargetHex);


	// -------------------------
	// --- Implementation
	// -------------------------

protected:
	UFUNCTION(BlueprintCallable)
	void SpawnEntityOnRandomHex(TSubclassOf<AClockworkHexEntity> ClockworkHexEntityClass);

	UFUNCTION(BlueprintCallable)
	bool SpawnEntityOnHex(TSubclassOf<AClockworkHexEntity> ClockworkHexEntityClass, AClockworkTile* Hex);


	// -------------------------
	// --- Const Implementation
	// -------------------------

protected:
	AClockworkTile* GetHexAt(const FOffsetCoordinate& Coordinate) const;
	TArray<AClockworkTile*> GetHexNeighbors(const AClockworkTile* Tile) const;

	FVector CalcualteHexLocation(const AClockworkTile* Tile, int Column, int Row) const;
	bool DetermineHexPathCost(AClockworkTile* Hex, const AClockworkTile* Target, uint8 StartDistance, float& Cost, const TMap<const AClockworkTile*, uint8>& HexDistanceMap) const;


	// -------------------------
	// --- Blueprint Events
	// -------------------------

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnProcessPathHex(AClockworkTile* Hex);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPathCheckingHexCost(AClockworkTile* Hex);


	// -------------------------
	// --- Debug
	// -------------------------

public:
	UFUNCTION(BlueprintCallable)
	void Debug_DrawHexCoordinates();

	UFUNCTION(BlueprintCallable)
	void Debug_UpdateHexDistanceFrom(AClockworkHexEntity* ClockworkHexEntity);

	UFUNCTION(BlueprintCallable)
	void Debug_SpawnUnits();
};
