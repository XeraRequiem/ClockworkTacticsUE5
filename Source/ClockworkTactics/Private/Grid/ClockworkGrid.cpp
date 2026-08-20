// Required
#include "Grid/ClockworkGrid.h"

// Game
#include "Core/ClockworkTactics.h"
#include "Grid/ClockworkTile.h"


// -------------------------
// --- Constructors
// -------------------------

AClockworkGrid::AClockworkGrid() :
	DefaultHexClass(AClockworkTile::StaticClass()),
	Hexes(TArray<AClockworkTile*>())
{
}


// -------------------------
// --- Const API
// -------------------------

AClockworkTile* AClockworkGrid::GetHexAtCoordinate(const FOffsetCoordinate& Coordinate) const
{
	uint8 index = Coordinate.Y * GridHexWidth + Coordinate.X;
	if (Hexes.Num() > index)
	{
		return Hexes[index];
	}

	return nullptr;
}

AClockworkTile* AClockworkGrid::GetRandomVacantHex() const
{
	if (Hexes.Num() > 0)
	{
		bool bVacantHex = false;
		while (!bVacantHex)
		{
			int32 index = FMath::RandRange(0, Hexes.Num() - 1);
			AClockworkTile* Hex = Hexes[index];
			if (Hex->GetOccupationStatus() == EOccupationStatus::Vacant)
			{
				return Hex;
			}
		}
	}

	return nullptr;
}

uint8 AClockworkGrid::HexDistanceBetween(const AClockworkTile* Hex1, const AClockworkTile* Hex2) const
{
	// Hex Distance = |dy| + max(0, (|dx|−|dy|)/2)

	FOffsetCoordinate Hex1Coordinate = Hex1->GetCoordinate();
	FOffsetCoordinate Hex2Coordinate = Hex2->GetCoordinate();

	uint8 dx = Hex2Coordinate.X - Hex1Coordinate.X;
	uint8 dy = Hex2Coordinate.Y - Hex2Coordinate.Y;

	return FMath::Abs(dy) + FMath::Max(0, (FMath::Abs(dx) - FMath::Abs(dy)) / 2);
}


// -------------------------
// --- API
// -------------------------

void AClockworkGrid::Initialize(int32 InWidth, int32 InDepth, bool bInVariableHeight)
{
	GridHexWidth = InWidth;
	GridHexDepth = InDepth;
	bVariableHeight = bInVariableHeight;
}

void AClockworkGrid::InitializeWithLayout(const FString& LayoutFile)
{
	FString Layout;
	FFileHelper::LoadFileToString(Layout, *FPaths::Combine(LayoutDataPath, LayoutFile));
	
	if (Layout.IsEmpty())
	{
		return;
	}


	// Clear Existing Hexes
	for (AClockworkTile* Hex : Hexes)
	{
		Hex->Destroy();
	}

	Hexes.Empty();

	// Parse Layout into Rows
	TArray<FString> Rows;
	Layout.ParseIntoArrayLines(Rows);

	GridHexDepth = Rows.Num();

	for (int Row = 0; Row < Rows.Num(); Row++)
	{
		FString RowLayout = Rows[Row];
		for (int Col = 0; Col < RowLayout.Len(); Col++)
		{
			GridHexWidth= RowLayout.Len();

			FString HexChar = FString(1, &RowLayout[Col]);

			if (LayoutEntityClassMap.Contains(HexChar))
			{
				// Create Tile
				AClockworkTile* Hex = GetWorld()->SpawnActor<AClockworkTile>(DefaultHexClass);
				Hex->InitializeTile(this, FOffsetCoordinate(Col, Row), bDebugMode);
				Hex->SetActorLocation(CalcualteHexLocation(Hex, Col, Row));
				Hex->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

				Hexes.Add(Hex);

				// Create Entity
				TSubclassOf<AClockworkHexEntity> HexClass = LayoutEntityClassMap[HexChar];

				if (HexClass != nullptr)
				{
					SpawnEntityOnHex(HexClass, Hex);
				}
			}
		}
	}

	if (bDebugMode)
	{
		Debug_DrawHexCoordinates();
	}
}


void AClockworkGrid::GenerateGrid()
{
	UE_LOG(LogHex, Log, TEXT("GenerateClockworkGrid"));

	GridHexWidth = (GridHexWidth != 0) ? GridHexWidth : FMath::Rand() % 6 + 1;
	GridHexDepth = (GridHexDepth != 0) ? GridHexDepth : FMath::Rand() % 6 + 1;

	// Clear Existing Hexes
	for (AClockworkTile* Hex : Hexes)
	{
		Hex->Destroy();
	}

	Hexes.Empty();

	for (int Row = 0; Row < GridHexDepth; Row++)
	{
		for (int Col = 0; Col < GridHexWidth; Col++)
		{
			AClockworkTile* Tile = GetWorld()->SpawnActor<AClockworkTile>(DefaultHexClass);
			Tile->InitializeTile(this, FOffsetCoordinate(Col, Row), bDebugMode);
			Tile->SetActorLocation(CalcualteHexLocation(Tile, Col, Row));
			Tile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

			Hexes.Add(Tile);
		}
	}

	if (bDebugMode)
	{
		Debug_SpawnUnits();
		Debug_DrawHexCoordinates();
	}
}


TArray<AClockworkTile*> AClockworkGrid::GetPathFromTo(AClockworkTile* Start, AClockworkTile* Target)
{
	// A* Pathing Algorithm

	// Initialization
	TMap<const AClockworkTile*, uint8> DistanceFromStartMap;
	TMap<const AClockworkTile*, float> HexCostMap;

	for (AClockworkTile* Hex : Hexes)
	{
		DistanceFromStartMap.Add(Hex, 255);
		HexCostMap.Add(Hex, INFINITY);
	}

	DistanceFromStartMap.Add(Start, 0);
	HexCostMap.Add(Start, 0);

	TQueue<const AClockworkTile*> HexesToProcess;
	HexesToProcess.Enqueue(Start);

	// Pre-Process Hexes
		// Determine Hex Costs
		// Cost = Hex Distance From Start + Linear Distance To Target

	const AClockworkTile* Hex;
	while (HexesToProcess.Dequeue(Hex))
	{
		TArray<AClockworkTile*> neighbors = GetHexNeighbors(Hex);
		for (AClockworkTile* neighbor : neighbors)
		{
			uint8 DistanceFromStart = (DistanceFromStartMap.Contains(Hex)) ? DistanceFromStartMap[Hex] + 1 : 255;

			float Cost = INFINITY;
			if (DetermineHexPathCost(neighbor, Target, DistanceFromStart, Cost, DistanceFromStartMap))
			{
				DistanceFromStartMap.Add(neighbor, DistanceFromStart);
				HexCostMap.Add(neighbor, Cost);

				HexesToProcess.Enqueue(neighbor);
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

	TArray<AClockworkTile*> path = TArray<AClockworkTile*>();
	path.EmplaceAt(0, Target);

	Hex = Target;
	while (Hex != nullptr && Hex != Start)
	{
		AClockworkTile* nextHex = nullptr;
		float nextHexCost = INFINITY;

		TArray<AClockworkTile*> neighbors = GetHexNeighbors(Hex);
		for (AClockworkTile* neighbor : neighbors)
		{
			if (!path.Contains(neighbor))
			{
				float neighborHexCost = HexCostMap[neighbor];
				if (neighborHexCost < nextHexCost)
				{
					nextHex = neighbor;
					nextHexCost = neighborHexCost;
				}
			}
		}

		if (nextHex != nullptr && nextHex != Start)
		{
			path.EmplaceAt(0, nextHex);
		}

		Hex = nextHex;
	}

	return path;
}

TArray<AClockworkTile*> AClockworkGrid::BidirectionalPathSearch(AClockworkTile* StartHex, AClockworkTile* TargetHex)
{
	//To-Do
	return TArray<AClockworkTile*>();
}


// -------------------------
// --- Implementation
// -------------------------

void AClockworkGrid::SpawnEntityOnRandomHex(TSubclassOf<AClockworkHexEntity> ClockworkHexEntityClass)
{
	if (Hexes.Num() > 0)
	{
		bool bSpawned = false;
		while (!bSpawned)
		{
			int32 index = FMath::RandRange(0, Hexes.Num() - 1);
			AClockworkTile* spawnHex = Hexes[index];

			bSpawned = SpawnEntityOnHex(ClockworkHexEntityClass, spawnHex);
		}
	}
}

bool AClockworkGrid::SpawnEntityOnHex(TSubclassOf<AClockworkHexEntity> ClockworkHexEntityClass, AClockworkTile* Hex)
{
	if (Hex->GetOccupationStatus() == EOccupationStatus::Vacant)
	{
		AClockworkHexEntity* HexEntity = GetWorld()->SpawnActor<AClockworkHexEntity>(ClockworkHexEntityClass);
		HexEntity->SetActorLocation(Hex->GetOccupationLocation());
		HexEntity->Initialize(Hex);

		return true;
	}

	return false;
}


// -------------------------
// --- Const Implementation
// -------------------------

AClockworkTile* AClockworkGrid::GetHexAt(const FOffsetCoordinate& Coordinate) const
{
	uint8 index = Coordinate.X + Coordinate.Y * GridHexWidth;

	if (Coordinate.X < GridHexWidth && Coordinate.Y < GridHexDepth && index < Hexes.Num())
	{
		return Hexes[index];
	}

	return nullptr;
}

TArray<AClockworkTile*> AClockworkGrid::GetHexNeighbors(const AClockworkTile* Hex) const
{
	FOffsetCoordinate Coordinate = Hex->GetCoordinate();
	TArray<FOffsetCoordinate> NeighborCoordinates = UHexMath::GetNeighborsOfOffsetCoordinate(Coordinate);

	TArray<AClockworkTile*> Neighbors = TArray<AClockworkTile*>();
	for (FOffsetCoordinate NeighborCoordinate : NeighborCoordinates)
	{
		AClockworkTile* Neighbor = GetHexAtCoordinate(NeighborCoordinate);
		if (Neighbor != nullptr)
		{
			Neighbors.Add(Neighbor);
		}
	}

	return Neighbors;
}


FVector AClockworkGrid::CalcualteHexLocation(const AClockworkTile* Hex, int Column, int Row) const
{
	if (Hex != nullptr)
	{
		float X = (Hex->GetMaximalDiameter() * 0.75 + GridHexSpacing) * Row;
		float Y = (Hex->GetMinimalDiameter() + GridHexSpacing) * Column;
		Y += (Row % 2) * Hex->GetMinimalDiameter() * 0.5;
		
		return FVector(X, Y, 0);
	}

	return FVector();
}

bool AClockworkGrid::DetermineHexPathCost(AClockworkTile* Hex, const AClockworkTile* Target, uint8 StartDistance, float& OutCost, const TMap<const AClockworkTile*, uint8>& HexDistanceMap) const
{
	if (Hex != nullptr && Target != nullptr && Hex->IsVacant() && StartDistance < HexDistanceMap[Hex])
	{
		float TargetDistance = FVector::Dist2D(Hex->GetOccupationLocation(), Target->GetOccupationLocation()) / 100.0;
		OutCost = StartDistance + TargetDistance;

		return true;
	}

	return false;
}


// -------------------------
// --- Debug
// -------------------------

void AClockworkGrid::Debug_DrawHexCoordinates()
{
	UE_LOG(LogHex, Log, TEXT("Debug_DrawHexCoordinates"));

	for (AClockworkTile* Hex : Hexes)
	{
		Hex->Debug_DrawCoordinates();
	}
}

void AClockworkGrid::Debug_UpdateHexDistanceFrom(AClockworkHexEntity* ClockworkHexEntity)
{
	//AClockworkTile* Hex = ClockworkHexEntity->GetOccupiedHex();
	//if (clockworkHex != nullptr)
	//{
	//	for (AClockworkTile* Hex : Hexes)
	//	{
	//		Hex->Debug_SetText(FString::FromInt(HexDistanceBetween(clockworkHex, Hex)));
	//	}
	//}
}

void AClockworkGrid::Debug_SpawnUnits()
{
	for (const TPair<TSubclassOf<AClockworkHexEntity>, uint8> EntitySpawnData : DebugClockworkHexEntityClasses)
	{
		for (int i = 0; i < EntitySpawnData.Value; i++)
		{
			SpawnEntityOnRandomHex(EntitySpawnData.Key);
		}
	}
}
