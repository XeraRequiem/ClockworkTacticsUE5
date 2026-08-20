// Required
#include "Grid/ClockworkTile.h"

// Engine
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"

// Plugin
#include "Grid/ClockworkGrid.h"
#include "HexLibrary.h"


// -------------------------
// --- Constructors
// -------------------------

AClockworkTile::AClockworkTile() :
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

void AClockworkTile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


// -------------------------
// --- API
// -------------------------

void AClockworkTile::InitializeTile(AClockworkGrid* InGrid, FOffsetCoordinate InCoordinate, bool bInDebugMode)
{
	OwningGrid = InGrid;
	Coordinate = InCoordinate;
	bDebugMode = bInDebugMode;
}


bool AClockworkTile::Reserve(AClockworkHexEntity* Actor)
{
	if (Occupant == nullptr && OccupationStatus == EOccupationStatus::Vacant)
	{
		Occupant = Actor;
		OccupationStatus = EOccupationStatus::Reserved;

		return OnReserved(Actor);
	}

	return true;
}

bool AClockworkTile::Occupy(AClockworkHexEntity* Actor)
{
	if (OccupationStatus == EOccupationStatus::Vacant || (OccupationStatus == EOccupationStatus::Reserved && Occupant == Actor))
	{
		OccupationStatus = EOccupationStatus::Occupied;

		return OnOccupied(Actor);
	}

	return true;
}

bool AClockworkTile::Vacate(AClockworkHexEntity* Actor)
{
	if (Occupant == Actor && OccupationStatus != EOccupationStatus::Vacant)
	{
		OccupationStatus = EOccupationStatus::Vacant;
		Occupant = nullptr;

		return OnVacated(Actor);
	}

	return true;
}


// -------------------------
// --- Const API
// -------------------------

EOccupationStatus AClockworkTile::GetOccupationStatus() const
{
	return OccupationStatus;
}

AClockworkHexEntity* AClockworkTile::GetOccupant() const
{
	return Occupant;
}

FVector AClockworkTile::GetOccupationLocation() const
{
	return OccupationLocation->GetComponentLocation();
}


FOffsetCoordinate AClockworkTile::GetCoordinate() const
{
	return Coordinate;
}



bool AClockworkTile::IsOccupied() const
{
	return OccupationStatus == EOccupationStatus::Occupied;
}

bool AClockworkTile::IsReserved() const
{
	return OccupationStatus == EOccupationStatus::Reserved;
}

bool AClockworkTile::IsVacant() const
{
	return OccupationStatus == EOccupationStatus::Vacant;
}


bool AClockworkTile::CanBeReservedBy(const AClockworkHexEntity* Actor) const
{
	return OccupationStatus == EOccupationStatus::Vacant;
}

bool AClockworkTile::CanBeOccupiedBy(const AClockworkHexEntity* Actor) const
{
	return OccupationStatus == EOccupationStatus::Vacant || (OccupationStatus == EOccupationStatus::Reserved && Occupant == Actor);
}

bool AClockworkTile::CanBeVacatedBy(const AClockworkHexEntity* Actor) const
{
	return OccupationStatus == EOccupationStatus::Occupied && Occupant == Actor;
}


AClockworkGrid* AClockworkTile::GetOwningGrid() const
{
	return OwningGrid;
}

FOffsetCoordinate AClockworkTile::GetGridCoordinate() const
{
	return Coordinate;
}


float AClockworkTile::GetCircumradius() const
{
	if (Mesh != nullptr && Mesh->GetStaticMesh() != nullptr)
	{
		FBoxSphereBounds bounds = Mesh->GetStaticMesh()->GetBounds();

		return bounds.BoxExtent.X;
	}

	return 0.0f;
}

float AClockworkTile::GetInradius() const
{
	if (Mesh != nullptr && Mesh->GetStaticMesh() != nullptr)
	{
		FBoxSphereBounds bounds = Mesh->GetStaticMesh()->GetBounds();

		return bounds.BoxExtent.Y;
	}

	return 0.0f;
}

float AClockworkTile::GetMaximalDiameter() const
{
	return GetCircumradius() * 2.0f;
}

float AClockworkTile::GetMinimalDiameter() const
{
	return GetInradius() * 2.0f;
}


// -------------------------
// --- Debugging
// -------------------------

void AClockworkTile::Debug_DrawCoordinates()
{
	Debug_SetText(Coordinate.ToString());
}

void AClockworkTile::Debug_SetText(const FString& DebugText)
{
	Debug_OnTextSet(DebugText);
}
