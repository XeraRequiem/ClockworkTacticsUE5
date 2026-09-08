// Required
#include "Grid/ClockworkGrid.h"

// Game
#include "Core/ClockworkTactics.h"
#include "Grid/ClockworkEntityDestinationComponent.h"
#include "Grid/ClockworkHex.h"


// -------------------------
// --- Constructors
// -------------------------

AClockworkGrid::AClockworkGrid() :
	DefaultHexClass(AClockworkHex::StaticClass()),
	Hexes(TArray<AClockworkHex*>())
{
}


// -------------------------
// --- Const API
// -------------------------

AClockworkHex* AClockworkGrid::GetHexAtCoordinate(const FOffsetCoordinate& Coordinate) const
{
	uint8 index = Coordinate.Y * GridHexWidth + Coordinate.X;
	if (Hexes.Num() > index)
	{
		return Hexes[index];
	}

	return nullptr;
}

AClockworkHex* AClockworkGrid::GetRandomVacantHex() const
{
	if (Hexes.Num() > 0)
	{
		bool bVacantHex = false;
		while (!bVacantHex)
		{
			int32 index = FMath::RandRange(0, Hexes.Num() - 1);
			AClockworkHex* Hex = Hexes[index];
			if (Hex->GetOccupationStatus() == EOccupationStatus::Vacant)
			{
				return Hex;
			}
		}
	}

	return nullptr;
}

TArray<AClockworkHex*> AClockworkGrid::GetDestinationHexes() const
{
	return DestinationHexes;
}


uint8 AClockworkGrid::HexDistanceBetween(const AClockworkHex* Hex1, const AClockworkHex* Hex2) const
{
	// Hex Distance = |dy| + max(0, (|dx|−|dy|)/2)

	FOffsetCoordinate Hex1Coordinate = Hex1->GetGridCoordinate();
	FOffsetCoordinate Hex2Coordinate = Hex2->GetGridCoordinate();

	uint8 dx = Hex2Coordinate.X - Hex1Coordinate.X;
	uint8 dy = Hex2Coordinate.Y - Hex1Coordinate.Y;

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

void AClockworkGrid::InitializeWithLayout(const FString& HexLayoutFile, const FString& EntityLayoutFile)
{
	// Get the layout data for hexes
	FString HexLayout;
	FFileHelper::LoadFileToString(HexLayout, *FPaths::Combine(LayoutDataPath, HexLayoutFile));

	if (HexLayout.IsEmpty())
	{
		UE_LOG(LogGrid, Warning, TEXT("Failed to load hex layout file: %s"), *HexLayoutFile);
		return;
	}

	// Get the layout data for entities
	FString EntityLayout;
	FFileHelper::LoadFileToString(EntityLayout, *FPaths::Combine(LayoutDataPath, EntityLayoutFile));
	
	if (EntityLayout.IsEmpty())
	{
		UE_LOG(LogGrid, Warning, TEXT("Failed to load entity layout file: %s"), *EntityLayoutFile);
		return;
	}

	// Clear Existing Hexes
	for (AClockworkHex* Hex : Hexes)
	{
		Hex->Destroy();
	}

	Hexes.Empty();

	// Parse Layout into Rows
	TArray<FString> HexRows;
	HexLayout.ParseIntoArrayLines(HexRows);

	TArray<FString> EntityRows;
	EntityLayout.ParseIntoArrayLines(EntityRows);

	if (HexRows.Num() != EntityRows.Num())
	{
		UE_LOG(LogGrid, Warning, TEXT("Hex layout and entity layout files have different number of rows: %s, %s"), *HexLayoutFile, *EntityLayoutFile);
		return;
	}

	GridHexDepth = HexRows.Num();

	for (int Row = 0; Row < HexRows.Num(); Row++)
	{
		FString HexRowLayout = HexRows[Row];
		FString EntityRowLayout = EntityRows[Row];

		if (HexRowLayout.Len() != EntityRowLayout.Len())
		{
			UE_LOG(LogGrid, Warning, TEXT("Hex layout and entity layout files have different number of columns: %s, %s"), *HexLayoutFile, *EntityLayoutFile);
			return;
		}

		for (int Col = 0; Col < HexRowLayout.Len(); Col++)
		{
			GridHexWidth= HexRowLayout.Len();

			FString HexChar = FString(1, &HexRowLayout[Col]);
			FString EntityChar = FString(1, &EntityRowLayout[Col]);

			// Create Tile
			if (LayoutHexClassMap.Contains(HexChar))
			{
				TSubclassOf<AClockworkHex> HexClass = LayoutHexClassMap[HexChar];
				AClockworkHex* Hex = GetWorld()->SpawnActor<AClockworkHex>(HexClass);
				Hex->InitializeTile(FOffsetCoordinate(Col, Row), bDebugMode);
				Hex->SetActorLocation(CalcualteHexLocation(Hex, Col, Row));
				Hex->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				Hexes.Add(Hex);

				if (Hex->GetComponentByClass<UClockworkEntityDestinationComponent>() != nullptr)
				{
					DestinationHexes.Add(Hex);
				}

				// Create Entity
				if (LayoutEntityClassMap.Contains(EntityChar))
				{
					TSubclassOf<AClockworkHexEntity> EntityClass = LayoutEntityClassMap[EntityChar];

					if (EntityClass != nullptr)
					{
						SpawnEntityOnHex(EntityClass, Hex);
					}
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
	for (AClockworkHex* Hex : Hexes)
	{
		Hex->Destroy();
	}

	Hexes.Empty();

	for (int Row = 0; Row < GridHexDepth; Row++)
	{
		for (int Col = 0; Col < GridHexWidth; Col++)
		{
			AClockworkHex* Tile = GetWorld()->SpawnActor<AClockworkHex>(DefaultHexClass);
			Tile->InitializeTile(FOffsetCoordinate(Col, Row), bDebugMode);
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

bool AClockworkGrid::SpawnEntityOnHex(TSubclassOf<AClockworkHexEntity> ClockworkHexEntityClass, AClockworkHex* Hex)
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


TArray<AClockworkHex*> AClockworkGrid::GetPathFromTo(AClockworkHex* Start, AClockworkHex* Target)
{
	// A* Pathing Algorithm

	// Initialization
	TMap<const AClockworkHex*, uint8> DistanceFromStartMap;
	TMap<const AClockworkHex*, float> HexCostMap;

	for (AClockworkHex* Hex : Hexes)
	{
		DistanceFromStartMap.Add(Hex, 255);
		HexCostMap.Add(Hex, INFINITY);
	}

	DistanceFromStartMap.Add(Start, 0);
	HexCostMap.Add(Start, 0);

	TQueue<const AClockworkHex*> HexesToProcess;
	HexesToProcess.Enqueue(Start);

	// Pre-Process Hexes
		// Determine Hex Costs
		// Cost = Hex Distance From Start + Linear Distance To Target

	const AClockworkHex* Hex;
	while (HexesToProcess.Dequeue(Hex))
	{
		TArray<AClockworkHex*> neighbors = GetHexNeighbors(Hex);
		for (AClockworkHex* neighbor : neighbors)
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

	TArray<AClockworkHex*> path = TArray<AClockworkHex*>();
	path.EmplaceAt(0, Target);

	Hex = Target;
	while (Hex != nullptr && Hex != Start)
	{
		AClockworkHex* nextHex = nullptr;
		float nextHexCost = INFINITY;

		TArray<AClockworkHex*> neighbors = GetHexNeighbors(Hex);
		for (AClockworkHex* neighbor : neighbors)
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

TArray<AClockworkHex*> AClockworkGrid::BidirectionalPathSearch(AClockworkHex* StartHex, AClockworkHex* TargetHex)
{
	//To-Do
	return TArray<AClockworkHex*>();
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
			AClockworkHex* spawnHex = Hexes[index];

			bSpawned = SpawnEntityOnHex(ClockworkHexEntityClass, spawnHex);
		}
	}
}


// -------------------------
// --- Const Implementation
// -------------------------

AClockworkHex* AClockworkGrid::GetHexAt(const FOffsetCoordinate& Coordinate) const
{
	uint8 index = Coordinate.X + Coordinate.Y * GridHexWidth;

	if (Coordinate.X < GridHexWidth && Coordinate.Y < GridHexDepth && index < Hexes.Num())
	{
		return Hexes[index];
	}

	return nullptr;
}

TArray<AClockworkHex*> AClockworkGrid::GetHexNeighbors(const AClockworkHex* Hex) const
{
	FOffsetCoordinate Coordinate = Hex->GetGridCoordinate();
	TArray<FOffsetCoordinate> NeighborCoordinates = UHexMath::GetNeighborsOfOffsetCoordinate(Coordinate);

	TArray<AClockworkHex*> Neighbors = TArray<AClockworkHex*>();
	for (FOffsetCoordinate NeighborCoordinate : NeighborCoordinates)
	{
		AClockworkHex* Neighbor = GetHexAtCoordinate(NeighborCoordinate);
		if (Neighbor != nullptr)
		{
			Neighbors.Add(Neighbor);
		}
	}

	return Neighbors;
}


FVector AClockworkGrid::CalcualteHexLocation(const AClockworkHex* Hex, int Column, int Row) const
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

bool AClockworkGrid::DetermineHexPathCost(AClockworkHex* Hex, const AClockworkHex* Target, uint8 StartDistance, float& OutCost, const TMap<const AClockworkHex*, uint8>& HexDistanceMap) const
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

	for (AClockworkHex* Hex : Hexes)
	{
		Hex->Debug_DrawCoordinates();
	}
}

void AClockworkGrid::Debug_UpdateHexDistanceFrom(AClockworkHexEntity* ClockworkHexEntity)
{
	//AClockworkHex* Hex = ClockworkHexEntity->GetOccupiedHex();
	//if (clockworkHex != nullptr)
	//{
	//	for (AClockworkHex* Hex : Hexes)
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
