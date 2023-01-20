// Parent
#include "HexGrid.h"

// Engine
#include "Net/UnrealNetwork.h"

// Game
#include "ClockworkTactics.h"


// -------------------------
// --- Constructors
// -------------------------

AHexGrid::AHexGrid() :
	HexClass(AHexTile::StaticClass()),
	GridHexWidth(0),
	GridHexDepth(0),
	GridHexSpacing(0.0f),
	Hexes(TArray<AHexTile*>()),
	bDebugMode(true)
{
	bReplicates = true;
}


// -------------------------
// --- Inherited
// -------------------------

void AHexGrid::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate Variables
	DOREPLIFETIME(AHexGrid, GridHexWidth);
	DOREPLIFETIME(AHexGrid, GridHexDepth);
	DOREPLIFETIME(AHexGrid, GridHexSpacing);
	DOREPLIFETIME(AHexGrid, Hexes);
}


// -------------------------
// --- API
// -------------------------

void AHexGrid::GenerateHexGrid_Implementation()
{
	// Clear Existing Hexes
	for (AHexTile* hex : Hexes)
	{
		hex->Destroy();
	}
	Hexes.Empty();

	for (int row = 0; row < GridHexDepth; row++)
	{
		for (int col = 0; col < GridHexWidth; col++)
		{
			AHexTile* tile = GetWorld()->SpawnActor<AHexTile>(HexClass);
			
			float posX = (tile->GetMaximalDiameter() * 0.75 + GridHexSpacing) * row;
			float posY = (tile->GetMinimalDiameter() + GridHexSpacing) * col;
			posY += (row % 2) * tile->GetMinimalDiameter() * 0.5;

			float height = FMath::RandRange(1, 20);
			
			tile->InitializeTile(this, FVector(posX, posY, 0), height, bDebugMode);
			tile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

			Hexes.Add(tile);
		}
	}

	if (bDebugMode)
	{
		Debug_SpawnUnits();
		Debug_DrawHexCoordinates();
	}
}


// -------------------------
// --- Const API
// -------------------------

AHexTile* AHexGrid::GetHexAtOffsetCoordinate(const FOffsetCoordinate& coordinate) const
{
	uint8 index = coordinate.Y * GridHexWidth + coordinate.X;
	if (Hexes.Num() > index)
	{
		return Hexes[index];
	}

	return nullptr;
}

AHexTile* AHexGrid::GetHexAtCubeCoordinate(const FCubeCoordinate& coordinate) const
{	
	return GetHexAtOffsetCoordinate(UClockworkTactics_HexLibrary::ConvertCubeCoordinateToOffset(coordinate));
}

AHexTile* AHexGrid::GetHexAtAxialCoordinate(const FAxialCoordinate& coordinate) const
{
	return GetHexAtOffsetCoordinate(UClockworkTactics_HexLibrary::ConvertAxialCoordinateToOffset(coordinate));
}

AHexTile* AHexGrid::GetHexAtDoubledCoordinate(const FDoubledCoordinate& coordinate) const
{
	return GetHexAtOffsetCoordinate(UClockworkTactics_HexLibrary::ConvertDoubledCoordinateToOffset(coordinate));
}


AHexTile* AHexGrid::GetRandomVacantHex() const
{
	if (Hexes.Num() > 0)
	{
		bool bVacantHex = false;
		while (!bVacantHex)
		{
			int32 index = FMath::RandRange(0, Hexes.Num() - 1);
			AHexTile* hex = Hexes[index];
			if (hex->GetOccupationStatus() == EOccupationStatus::Vacant)
			{
				return hex;
			}
		}
	}

	return nullptr;
}


AHexTile* AHexGrid::GetOccupiedHexClosestTo(const AHexTile* start) const
{
	// Breadth-First Search
	
	TArray<const AHexTile*> searchedHexes = TArray<const AHexTile*>();
	TQueue<AHexTile*> hexesToSearch;
	
	// Initial Search List
	for (AHexTile* hex : GetHexNeighbors(start))
	{
		hexesToSearch.Enqueue(hex);
		searchedHexes.Add(start);
	}

	while (!hexesToSearch.IsEmpty())
	{
		AHexTile* hex;
		hexesToSearch.Dequeue(hex);

		if (!searchedHexes.Contains(hex))
		{
			if (hex->IsOccupied())
			{
				return hex;
			}

			for (AHexTile* neighbor : GetHexNeighbors(hex))
			{
				hexesToSearch.Enqueue(neighbor);
			}

			searchedHexes.Add(hex);
		}
	}

	return nullptr;
}

AHexTile* AHexGrid::GetVacantHexClosestTo(const AHexTile* start) const
{
	// Breadth-First Search

	TArray<const AHexTile*> searchedHexes = TArray<const AHexTile*>();
	TQueue<AHexTile*> hexesToSearch;

	// Initial Search List
	for (AHexTile* hex : GetHexNeighbors(start))
	{
		hexesToSearch.Enqueue(hex);
		searchedHexes.Add(start);
	}

	while (!hexesToSearch.IsEmpty())
	{
		AHexTile* hex;
		hexesToSearch.Dequeue(hex);

		if (!searchedHexes.Contains(hex))
		{
			if (hex->IsVacant())
			{
				return hex;
			}

			for (AHexTile* neighbor : GetHexNeighbors(hex))
			{
				hexesToSearch.Enqueue(neighbor);
			}

			searchedHexes.Add(hex);
		}
	}

	return nullptr;
}


AClockworkUnit* AHexGrid::GetEnemyUnitClosestTo(const AClockworkUnit* unit) const
{
	// Breadth-First Search

	TArray<AHexTile*> searchedHexes = TArray<AHexTile*>();
	TQueue<AHexTile*> hexesToSearch;
	
	// Initial Search List
	AHexTile* startHex = unit->GetOccupiedHex();

	for (AHexTile* hex : GetHexNeighbors(startHex))
	{
		hexesToSearch.Enqueue(hex);
	}
	searchedHexes.Add(startHex);

	while (!hexesToSearch.IsEmpty())
	{
		AHexTile* hex;
		hexesToSearch.Dequeue(hex);

		if (!searchedHexes.Contains(hex))
		{
			if (hex != startHex && hex->GetOccupationStatus() == EOccupationStatus::Occupied)
			{
				return hex->GetOccupant();
			}

			for (AHexTile* neighbor : GetHexNeighbors(hex))
			{
				hexesToSearch.Enqueue(neighbor);
			}

			searchedHexes.Add(hex);
		}
	}

	return nullptr;
}


uint8 AHexGrid::HexDistanceBetween(const AHexTile* hex1, const AHexTile* hex2) const
{
	// Hex Distance = |dy| + max(0, (|dx|−|dy|)/2)

	FDoubledCoordinate hex1Coordinate = GetDoublesCoordinateOfHex(hex1);
	FDoubledCoordinate hex2Coordinate = GetDoublesCoordinateOfHex(hex2);

	uint8 dx = hex2Coordinate.X - hex1Coordinate.X;
	uint8 dy = hex2Coordinate.Y - hex2Coordinate.Y;

	return FMath::Abs(dy) + FMath::Max(0, (FMath::Abs(dx) - FMath::Abs(dy)) / 2);
}

TArray<AHexTile*> AHexGrid::GetPathFromTo(AHexTile* start, AHexTile* target) const
{
	// A* Pathing Algorithm

	// Initialization
	TMap<AHexTile*, uint8> distanceFromStartMap;
	TMap<AHexTile*, float> hexCostMap;

	for (AHexTile* hex : Hexes)
	{
		distanceFromStartMap.Add(hex, 255);
		hexCostMap.Add(hex, INFINITY);
	}

	distanceFromStartMap.Add(start, 0);
	hexCostMap.Add(start, 0);

	TQueue<const AHexTile*> hexesToProcess;
	hexesToProcess.Enqueue(start);

	// Pre-Process Hexes
		// Determine Hex Costs
		// Cost = Hex Distance From Start + Linear Distance To Target

	const AHexTile* hex;
	while (hexesToProcess.Dequeue(hex))
	{
		TArray<AHexTile*> neighbors = GetHexNeighbors(hex);
		for (AHexTile* neighbor : neighbors)
		{
			uint8 distanceFromStart = distanceFromStartMap[hex] + 1;

			float cost = INFINITY;
			if (DetermineHexPathCost(neighbor, target, distanceFromStart, cost, distanceFromStartMap))
			{
				distanceFromStartMap.Add(neighbor, distanceFromStart);
				hexCostMap.Add(neighbor, cost);

				hexesToProcess.Enqueue(neighbor);
			}
		}
	}

	// Determine Optimal Path From Start To Target
		// Ignores Occupation Status of Hexes (TODO: Fix to avoid Occupied/Reserved Hexes)
		// Greedily Select Neighboring Hex With Lowest Cost
		// Reverse Order - Target to Start
		// End Conditions
			// Reached Target
			// No Unprocessed Neighbors

	TArray<AHexTile*> path = TArray<AHexTile*>();
	path.Add(target);

	hex = target;
	while (hex != nullptr && hex != start)
	{
		AHexTile* nextHex = nullptr;
		float nextHexCost = INFINITY;

		TArray<AHexTile*> neighbors = GetHexNeighbors(hex);
		for (AHexTile* neighbor : neighbors)
		{
			if (!path.Contains(neighbor))
			{
				float neighborHexCost = hexCostMap[neighbor];
				if (neighborHexCost < nextHexCost)
				{
					nextHex = neighbor;
					nextHexCost = neighborHexCost;
				}
			}
		}

		if (nextHex != nullptr && nextHex != start)
		{
			path.Add(nextHex);
		}

		hex = nextHex;
	}

	// Reverse Path
	for (int i = 0; i < path.Num() / 2; i++)
	{
		int swapIndex = path.Num() - 1 - i;
		AHexTile* temp = path[i];

		path[i] = path[swapIndex];
		path[swapIndex] = temp;
	}

	return path;
}


// -------------------------
// --- Implementation
// -------------------------

void AHexGrid::SpawnUnitOnRandomHex(TSubclassOf<AClockworkUnit> clockworkClass)
{
	if (Hexes.Num() > 0)
	{
		bool bSpawned = false;
		while (!bSpawned)
		{
			int32 index = FMath::RandRange(0, Hexes.Num() - 1);
			AHexTile* spawnHex = Hexes[index];

			bSpawned = SpawnUnitOnHex(clockworkClass, spawnHex);
		}
	}
}

bool AHexGrid::SpawnUnitOnHex(TSubclassOf<AClockworkUnit> clockworkClass, AHexTile* hex)
{
	if (hex->GetOccupationStatus() == EOccupationStatus::Vacant)
	{
		AClockworkUnit* unit = GetWorld()->SpawnActor<AClockworkUnit>(clockworkClass);
		unit->InitializeUnit(hex, bDebugMode);

		return true;
	}

	return false;
}


// -------------------------
// --- Const Implementation
// -------------------------

TArray<AHexTile*> AHexGrid::GetHexNeighbors(const AHexTile* hex) const
{
	FOffsetCoordinate coordinate = GetOffsetCoordinateOfHex(hex);
	TArray<FOffsetCoordinate> neighborCoordinates = UClockworkTactics_HexLibrary::GetNeighborsOfOffsetCoordinate(coordinate, GridHexWidth - 1, GridHexDepth - 1);

	TArray<AHexTile*> neighbors = TArray<AHexTile*>();
	for (FOffsetCoordinate neighborCoordinate : neighborCoordinates)
	{
		AHexTile* neighbor = GetHexAtOffsetCoordinate(neighborCoordinate);
		if (neighbor != nullptr)
		{
			neighbors.Add(neighbor);
		}
	}

	return neighbors;
}


AHexTile* AHexGrid::GetHexAt(uint8 x, uint8 y) const
{
	uint8 index = x + y * GridHexWidth;

	if (x < GridHexWidth && y < GridHexDepth && index < Hexes.Num())
	{
		return Hexes[x + y * GridHexWidth];
	}

	return nullptr;
}

FOffsetCoordinate AHexGrid::GetOffsetCoordinateOfHex(const AHexTile* hex) const
{
	uint8 index = Hexes.IndexOfByKey(hex);

	uint8 x = index % GridHexWidth;
	uint8 y = index / GridHexWidth;

	return FOffsetCoordinate(x, y);
}

FDoubledCoordinate AHexGrid::GetDoublesCoordinateOfHex(const AHexTile* hex) const
{
	FOffsetCoordinate coordinate = GetOffsetCoordinateOfHex(hex);

	return UClockworkTactics_HexLibrary::ConvertOffsetCoordinateToDoubled(coordinate);
}


bool AHexGrid::DetermineHexPathCost(AHexTile* hex, const AHexTile* target, uint8 startDistance, float& cost, const TMap<AHexTile*, uint8>& hexDistanceMap) const
{
	if (hex != nullptr && target != nullptr && hex->IsVacant() && startDistance < hexDistanceMap[hex])
	{
		float targetDistance = FVector::Dist2D(hex->GetOccupationLocation(), target->GetOccupationLocation()) / 100.0;
		cost = startDistance + targetDistance;

		//if (bDebugMode)
		//{
		//	hex->Debug_SetText(FString::SanitizeFloat(cost));
		//}

		return true;
	}

	return false;
}


// -------------------------
// --- Debug
// -------------------------

void AHexGrid::Debug_DrawHexCoordinates()
{
	for (AHexTile* hex : Hexes)
	{
		hex->Debug_SetText(hex->GetGridCoordinate().ToString());
	}
}

void AHexGrid::Debug_UpdateHexDistanceFrom(AClockworkUnit* clockwork)
{
	AHexTile* clockworkHex = clockwork->GetOccupiedHex();
	if (clockworkHex != nullptr)
	{
		for (AHexTile* hex : Hexes)
		{
			hex->Debug_SetText(FString::FromInt(HexDistanceBetween(clockworkHex, hex)));
		}
	}
}

void AHexGrid::Debug_SpawnUnits()
{
	for (const TPair<TSubclassOf<AClockworkUnit>, uint8> spawnData : DebugClockworkClassesToSpawn)
	{
		for (int i = 0; i < spawnData.Value; i++)
		{
			SpawnUnitOnRandomHex(spawnData.Key);
		}
	}
}
