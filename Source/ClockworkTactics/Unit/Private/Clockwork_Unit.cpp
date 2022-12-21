#include "Clockwork_Unit.h"


//-------------------------
// Constructors
//-------------------------

AClockwork_HexTile::AClockwork_HexTile() :
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
