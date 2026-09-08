#pragma once

// Engine
#include "GameFramework/Actor.h"

// Plugin
#include "Core/HexMath.h"
#include "Entity/ClockworkHexEntity.h"

// Generated
#include "ClockworkGrid.generated.h"


class AClockworkHex;


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
	TSubclassOf<AClockworkHex> DefaultHexClass;

	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AClockworkHexEntity>, uint8> DebugClockworkHexEntityClasses;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSubclassOf<AClockworkHexEntity>> LayoutEntityClassMap;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSubclassOf<AClockworkHex>> LayoutHexClassMap;


	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<AClockworkHex>> Hexes;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<AClockworkHex>> DestinationHexes;


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

public:
	UFUNCTION(BlueprintPure)
	AClockworkHex* GetHexAtCoordinate(const FOffsetCoordinate& Coordinate) const;

	UFUNCTION(BlueprintPure)
	AClockworkHex* GetRandomVacantHex() const;

	UFUNCTION(BlueprintPure)
	TArray<AClockworkHex*> GetDestinationHexes() const;


	UFUNCTION(BlueprintPure)
	uint8 HexDistanceBetween(const AClockworkHex* Hex1, const AClockworkHex* Hex2) const;


	// -------------------------	
	// --- API
	// -------------------------

public:
	UFUNCTION(BlueprintCallable)
	void Initialize(int32 InWidth, int32 InDepth, bool bInVariableHeight);

	UFUNCTION(BlueprintCallable)
	void InitializeWithLayout(const FString& HexLayoutFile, const FString& EntityLayoutFile);

	UFUNCTION(BlueprintCallable)
	void GenerateGrid();


	UFUNCTION(BlueprintCallable)
	bool SpawnEntityOnHex(TSubclassOf<AClockworkHexEntity> ClockworkHexEntityClass, AClockworkHex* Hex);


	UFUNCTION(BlueprintCallable)
	TArray<AClockworkHex*> GetPathFromTo(AClockworkHex* Start, AClockworkHex* Target);

	// To-Do
	UFUNCTION(BlueprintCallable)
	TArray<AClockworkHex*> BidirectionalPathSearch(AClockworkHex* StartHex, AClockworkHex* TargetHex);


	// -------------------------
	// --- Implementation
	// -------------------------

protected:
	UFUNCTION(BlueprintCallable)
	void SpawnEntityOnRandomHex(TSubclassOf<AClockworkHexEntity> ClockworkHexEntityClass);



	// -------------------------
	// --- Const Implementation
	// -------------------------

protected:
	AClockworkHex* GetHexAt(const FOffsetCoordinate& Coordinate) const;
	TArray<AClockworkHex*> GetHexNeighbors(const AClockworkHex* Tile) const;

	FVector CalcualteHexLocation(const AClockworkHex* Tile, int Column, int Row) const;
	bool DetermineHexPathCost(AClockworkHex* Hex, const AClockworkHex* Target, uint8 StartDistance, float& Cost, const TMap<const AClockworkHex*, uint8>& HexDistanceMap) const;


	// -------------------------
	// --- Blueprint Events
	// -------------------------

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnProcessPathHex(AClockworkHex* Hex);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPathCheckingHexCost(AClockworkHex* Hex);


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
