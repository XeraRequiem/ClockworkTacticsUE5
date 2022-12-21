#include "Clockwork_HexGrid.h"


//-------------------------
// Constructors
//-------------------------

AClockwork_HexGrid::AClockwork_HexGrid() :
	TileWidth(5),
	TileLength(5),
	TileGap(5),
	TileClass(AClockwork_HexTile::StaticClass()),
	Tiles(TArray<AClockwork_HexTile*>())
{
	bReplicates = true;
}


//-------------------------
// Inherited
//-------------------------	

void AClockwork_HexGrid::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AClockwork_HexGrid, Tiles);
}

void AClockwork_HexGrid::BeginPlay()
{
	GenerateGrid();
}


//-------------------------
// Implementation
//-------------------------

void AClockwork_HexGrid::GenerateGrid_Implementation()
{
	float dx = 0.75f * AClockwork_HexTile::DiameterOuter + TileGap;
	float dy = 0.50f * AClockwork_HexTile::DiameterInner;
	
	for (int i = 0; i < TileLength; i++)
	{
		for (int j = 0; j < TileWidth; j++)
		{
			AClockwork_HexTile* tile = GetWorld()->SpawnActor<AClockwork_HexTile>(TileClass);

			float x = dx * j;
			float y = (dy + TileGap) * (j % 2) + (AClockwork_HexTile::DiameterInner + TileGap) * i;

			tile->SetActorLocation(GetActorLocation() + FVector(x, y, 0.0f));
			Tiles.Add(tile);
		}
	}
}