// Parent
#include "HexTile.h"

// Engine
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"

// Game
#include "ClockworkTactics.h"
#include "HexGrid.h"


// -------------------------
// --- Constructors
// -------------------------

AHexTile::AHexTile() :
	Mesh(nullptr),
	OccupationLocation(nullptr),
	Height(0.5f),
	OccupationStatus(EOccupationStatus::Vacant),
	Occupant(nullptr),
	OwningGrid(nullptr)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	OccupationLocation = CreateDefaultSubobject<USceneComponent>(FName("OccupationSpot"));
	OccupationLocation->SetupAttachment(Mesh);

	bReplicates = true;
}


// -------------------------
// --- Inherited
// -------------------------

void AHexTile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate Variables
	DOREPLIFETIME(AHexTile, OccupationStatus);
}


// -------------------------
// --- API
// -------------------------

void AHexTile::InitializeTile(AHexGrid* grid, bool inDebugMode)
{
	OwningGrid = grid;
	bDebugMode = inDebugMode;
}


bool AHexTile::Reserve(AClockworkUnit* clockwork)
{
	if (Occupant == nullptr && OccupationStatus == EOccupationStatus::Vacant)
	{
		Occupant = clockwork;
		OccupationStatus = EOccupationStatus::Reserved;
		OnReserved(clockwork);
	}
}

void AHexTile::Occupy_Implementation(AClockworkUnit* clockwork)
{
	if (Occupant == clockwork && OccupationStatus == EOccupationStatus::Reserved)
	{
		OccupationStatus = EOccupationStatus::Occupied;
		OnOccupied(clockwork);
	}
}

void AHexTile::Vacate_Implementation(AClockworkUnit* clockwork)
{
	if (Occupant == clockwork && OccupationStatus != EOccupationStatus::Vacant)
	{
		OccupationStatus = EOccupationStatus::Vacant;
		Occupant = nullptr;
		OnVacated(clockwork);
	}
}


// -------------------------
// --- Const API
// -------------------------

EOccupationStatus AHexTile::GetOccupationStatus() const
{
	return OccupationStatus;
}

AClockworkUnit* AHexTile::GetOccupant() const
{
	return Occupant;
}

FVector AHexTile::GetOccupationLocation() const
{
	return OccupationLocation->GetComponentLocation();
}


bool AHexTile::IsOccupied() const
{
	return OccupationStatus == EOccupationStatus::Occupied;
}

bool AHexTile::IsReserved() const
{
	return OccupationStatus == EOccupationStatus::Reserved;
}

bool AHexTile::IsVacant() const
{
	return OccupationStatus == EOccupationStatus::Vacant;
}


bool AHexTile::CanBeReservedBy(const AClockworkUnit* clockwork) const
{
	return OccupationStatus == EOccupationStatus::Vacant;
}

bool AHexTile::CanBeOccupiedBy(const AClockworkUnit* clockwork) const
{
	return OccupationStatus == EOccupationStatus::Reserved && Occupant == clockwork;
}

bool AHexTile::CanBeVacatedBy(const AClockworkUnit* clockwork) const
{
	return OccupationStatus == EOccupationStatus::Occupied && Occupant == clockwork;
}


AHexGrid* AHexTile::GetOwningGrid() const
{
	return OwningGrid;
}

FVector2D AHexTile::GetGridCoordinate() const
{
	return Coordinate;
}


float AHexTile::GetCircumradius() const
{
	if (Mesh != nullptr && Mesh->GetStaticMesh() != nullptr)
	{
		FBoxSphereBounds bounds = Mesh->GetStaticMesh()->GetBounds();

		return bounds.BoxExtent.X;
	}

	return 0.0f;
}

float AHexTile::GetInradius() const
{
	if (Mesh != nullptr && Mesh->GetStaticMesh() != nullptr)
	{
		FBoxSphereBounds bounds = Mesh->GetStaticMesh()->GetBounds();

		return bounds.BoxExtent.Y;
	}

	return 0.0f;
}

float AHexTile::GetMaximalDiameter() const
{
	return GetCircumradius() * 2.0f;
}

float AHexTile::GetMinimalDiameter() const
{
	return GetInradius() * 2.0f;
}


// -------------------------
// --- Debugging
// -------------------------

void AHexTile::Debug_SetText(const FString& text)
{
	Debug_OnTextSet(text);
}
