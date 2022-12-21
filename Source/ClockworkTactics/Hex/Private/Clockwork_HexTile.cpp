#include "Clockwork_HexTile.h"

//-------------------------
// Static Variables
//-------------------------

uint8 AClockwork_HexTile::DiameterInner = 104;
uint8 AClockwork_HexTile::DiameterOuter = 120;

//-------------------------
// Constructors
//-------------------------

AClockwork_HexTile::AClockwork_HexTile() :
	TimeSinceColorChange(0.0f),
	TimeBetweenColorChange(3.0f)
{
	bReplicates = true;
}


//-------------------------
// Inherited
//-------------------------	

void AClockwork_HexTile::Tick(float delta)
{
	Super::Tick(delta);

	TimeSinceColorChange += delta;

	if (TimeSinceColorChange > TimeBetweenColorChange)
	{
		ChangeColor();

		TimeSinceColorChange = 0.0f;
	}
}

void AClockwork_HexTile::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate Data
	DOREPLIFETIME(AClockwork_HexTile, TimeSinceColorChange);
}


//-------------------------
// Implementation
//-------------------------

void AClockwork_HexTile::ChangeColor()
{
	float red = FMath::RandRange(0.0f, 1.0f);
	float green = FMath::RandRange(0.0f, 1.0f);
	float blue = FMath::RandRange(0.0f, 1.0f);

	SetColor(FLinearColor(red, green, blue));
}

void AClockwork_HexTile::SetColor_Implementation(FLinearColor inColor)
{
	OnColorSet(inColor);
}
