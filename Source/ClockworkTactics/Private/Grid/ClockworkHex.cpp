// Required
#include "Grid/ClockworkHex.h"

// Engine
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"

// Plugin
#include "HexLibrary.h"

// Game
#include "Grid/ClockworkGrid.h"

// -------------------------
// --- Constructors
// -------------------------

AClockworkHex::AClockworkHex() :
	HexMesh(nullptr),
	OccupationLocation(nullptr),
	Occupant(nullptr),
	FriendlyName(TEXT("Clockwork Hex"))
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	HexMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("HexMesh"));
	HexMesh->SetupAttachment(RootComponent);

	OccupationLocation = CreateDefaultSubobject<USceneComponent>(FName("OccupationSpot"));
	OccupationLocation->SetupAttachment(HexMesh);
}


// -------------------------
// --- API
// -------------------------

void AClockworkHex::InitializeTile(FOffsetCoordinate InCoordinate, bool bInDebugMode)
{
	Coordinate = InCoordinate;
	bDebugMode = bInDebugMode;

	FriendlyName = Coordinate.ToString();
}


bool AClockworkHex::Reserve(AClockworkHexEntity* Actor)
{
	if (Occupant == nullptr && OccupationStatus == EOccupationStatus::Vacant)
	{
		Occupant = Actor;
		OccupationStatus = EOccupationStatus::Reserved;

		return OnReserved(Actor);
	}

	return true;
}

bool AClockworkHex::Occupy(AClockworkHexEntity* Actor)
{
	if (OccupationStatus == EOccupationStatus::Vacant || (OccupationStatus == EOccupationStatus::Reserved && Occupant == Actor))
	{
		OccupationStatus = EOccupationStatus::Occupied;
		Occupant = Actor;

		return OnOccupied(Actor);
	}

	return true;
}

bool AClockworkHex::Vacate(AClockworkHexEntity* Actor)
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

EOccupationStatus AClockworkHex::GetOccupationStatus() const
{
	return OccupationStatus;
}

bool AClockworkHex::IsOccupied() const
{
	return OccupationStatus == EOccupationStatus::Occupied;
}

bool AClockworkHex::IsReserved() const
{
	return OccupationStatus == EOccupationStatus::Reserved;
}

bool AClockworkHex::IsVacant() const
{
	return OccupationStatus == EOccupationStatus::Vacant;
}


AClockworkHexEntity* AClockworkHex::GetOccupant() const
{
	return Occupant;
}

FVector AClockworkHex::GetOccupationLocation() const
{
	return OccupationLocation->GetComponentLocation();
}

bool AClockworkHex::CanBeReservedBy(const AClockworkHexEntity* Entity) const
{
	return OccupationStatus == EOccupationStatus::Vacant;
}

bool AClockworkHex::CanBeOccupiedBy(const AClockworkHexEntity* Entity) const
{
	return OccupationStatus == EOccupationStatus::Vacant || (OccupationStatus == EOccupationStatus::Reserved && Occupant == Entity);
}

bool AClockworkHex::CanBeVacatedBy(const AClockworkHexEntity* Entity) const
{
	return (OccupationStatus == EOccupationStatus::Occupied || OccupationStatus == EOccupationStatus::Reserved) && Occupant == Entity;
}


FOffsetCoordinate AClockworkHex::GetGridCoordinate() const
{
	return Coordinate;
}


float AClockworkHex::GetCircumradius() const
{
	if (HexMesh != nullptr && HexMesh->GetStaticMesh() != nullptr)
	{
		FBoxSphereBounds bounds = HexMesh->GetStaticMesh()->GetBounds();

		return bounds.BoxExtent.X;
	}

	return 0.0f;
}

float AClockworkHex::GetInradius() const
{
	if (HexMesh != nullptr && HexMesh->GetStaticMesh() != nullptr)
	{
		FBoxSphereBounds bounds = HexMesh->GetStaticMesh()->GetBounds();

		return bounds.BoxExtent.Y;
	}

	return 0.0f;
}

float AClockworkHex::GetMaximalDiameter() const
{
	return GetCircumradius() * 2.0f;
}

float AClockworkHex::GetMinimalDiameter() const
{
	return GetInradius() * 2.0f;
}


FString AClockworkHex::GetFriendlyName() const
{
	return FriendlyName;
}


// -------------------------
// --- Debugging
// -------------------------

void AClockworkHex::Debug_DrawCoordinates()
{
	Debug_SetText(Coordinate.ToString());
}

void AClockworkHex::Debug_SetText(const FString& DebugText)
{
	Debug_OnTextSet(DebugText);
}
