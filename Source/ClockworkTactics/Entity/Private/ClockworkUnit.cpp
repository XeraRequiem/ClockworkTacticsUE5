// Required
#include "ClockworkUnit.h"

// Engine
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"

// Game
#include "HexGrid.h"
#include "ClockworkTactics.h"


// -------------------------
// --- Constructors
// -------------------------

AClockworkUnit::AClockworkUnit() :
	Super(),
	Mesh(nullptr),
	CurrentHex(nullptr),
	ReservedHex(nullptr),
	TargetedUnit(nullptr),
	bMoving(false),
	MoveSpeed(0.5f),
	AttackRange(1),
	AttackDamage(0.0f),
	MagicDamage(0.0f),
	AttackSpeed(0.0f),
	PhysicalDefense(0.0f),
	MagicResistance(0.0f),
	MoveTimeTotal(0.0f),
	MinJumpHeight(50.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetReplicates(true);
	SetReplicateMovement(true);

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}


// -------------------------
// --- Inherited
// -------------------------

void AClockworkUnit::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate Variables
	DOREPLIFETIME(AClockworkUnit, CurrentHex);
	DOREPLIFETIME(AClockworkUnit, ReservedHex);
	DOREPLIFETIME(AClockworkUnit, TargetedUnit);
	DOREPLIFETIME(AClockworkUnit, TargetedHex);
	DOREPLIFETIME(AClockworkUnit, bMoving);
}

void AClockworkUnit::Tick(float dt)
{
	// Target Hex-Based Movement
	if (TargetedHex == nullptr)
	{
		SelectTargetHex_Server();
	}
	
	if (CurrentHex != TargetedHex)
	{
		Move(dt);
	}
	else
	{
		TargetedHex = nullptr;
	}
}


// -------------------------
// --- API
// -------------------------

void AClockworkUnit::InitializeUnit(AHexTile* hex, bool inDebugMode)
{
	bDebugMode = inDebugMode;

	if (hex != nullptr)
	{
		ReserveHex(hex);
		OccupyHex(hex);

		SetActorLocation(hex->GetOccupationLocation());
	}

	OnInitialized(hex);
}


bool AClockworkUnit::TargetHex(AHexTile* hex)
{
	if (hex != nullptr)
	{
		TargetedHex = hex;
		HandleHexTargeted(hex);

		UE_LOG(LogClockwork, Verbose, TEXT("Selecting New Target Hex: %s"), *TargetedHex->GetName());
		return true;
	}

	return false;
}

bool AClockworkUnit::VacateHex(AHexTile* hex)
{
	UE_LOG(LogClockwork, Verbose, TEXT("Vacating Hex %s..."), *hex->GetName());
	if (hex != nullptr && hex->CanBeVacatedBy(this))
	{
		hex->Vacate(this);
		CurrentHex = nullptr;

		HandleHexVacated(hex);

		UE_LOG(LogClockwork, Verbose, TEXT("	Success"));
		return true;
	}

	UE_LOG(LogClockwork, Verbose, TEXT("	Failed"));
	return false;
}

bool AClockworkUnit::ReserveHex(AHexTile* hex)
{
	UE_LOG(LogClockwork, Verbose, TEXT("Reserving Hex %s..."), *hex->GetName());
	if (hex != nullptr && hex->CanBeReservedBy(this))
	{
		hex->Reserve(this);
		ReservedHex = hex;

		HandleHexReserved(hex);

		UE_LOG(LogClockwork, Verbose, TEXT("	Success"));
		return true;
	}

	UE_LOG(LogClockwork, Verbose, TEXT("	Failed"));
	return false;
}

bool AClockworkUnit::OccupyHex(AHexTile* hex)
{
	UE_LOG(LogClockwork, Verbose, TEXT("Occupying Hex %s..."), *hex->GetName());
	if (hex != nullptr && hex->CanBeOccupiedBy(this))
	{
		if (CurrentHex != nullptr)
		{
			VacateHex(CurrentHex);
		}

		hex->Occupy(this);
		ReservedHex = nullptr;
		CurrentHex = hex;

		HandleHexOccupied(hex);

		UE_LOG(LogClockwork, Verbose, TEXT("	Success"));
		return true;
	}

	UE_LOG(LogClockwork, Verbose, TEXT("	Failed"));
	return false;
}


// -------------------------
// --- Const API
// -------------------------

AHexTile* AClockworkUnit::GetOccupiedHex() const
{
	return CurrentHex;
}


// -------------------------
// --- Implementation
// -------------------------

void AClockworkUnit::Move_Implementation(float dt)
{
	if (ReservedHex == nullptr)
	{
		TArray<AHexTile*> path = GetPathToHex(TargetedHex);
		if (path.Num() > 0)
		{

			AHexTile* hex = path[0];
			ReserveHex(hex);

			if (bDebugMode)
			{
				hex->GetOwningGrid()->Debug_UpdateHexDistanceFrom(this);
			}
		}
	}

	MoveToReservedHex(dt);
}

void AClockworkUnit::MoveToReservedHex_Implementation(float dt)
{
	if (ReservedHex != nullptr)
	{
		MoveTimeTotal += dt;

		float percentToTarget = FMath::Clamp(MoveTimeTotal * MoveSpeed, 0.0f, 1.0f);
		UE_LOG(LogClockwork, Verbose, TEXT("Moving To Reserved Hex %f%"), percentToTarget);
		
		FVector curve = JumpCurve->GetVectorValue(percentToTarget);

		FVector start = CurrentHex->GetOccupationLocation();
		FVector end = ReservedHex->GetOccupationLocation();
		end.Z = FMath::Max(start.Z, end.Z) + MinJumpHeight;

		FVector delta = end - start;
		FVector location = start + delta * curve;

		SetActorLocation(location);

		if (percentToTarget == 1.0f)
		{
			MoveTimeTotal = 0.0f;
			OccupyHex(ReservedHex);
		}
	}
}


TArray<AHexTile*> AClockworkUnit::GetPathToHex(AHexTile* hex)
{
	if (CurrentHex != nullptr)
	{
		AHexGrid* grid = CurrentHex->GetOwningGrid();
		if (grid != nullptr)
		{
			return grid->GetPathFromTo(CurrentHex, hex);
		}
	}

	return TArray<AHexTile*>();
}

TArray<AHexTile*> AClockworkUnit::GetPathToHexWithinTargetRange()
{
	if (TargetedUnit != nullptr)
	{
		AHexTile* targetHex = TargetedUnit->GetOccupiedHex();
		if (targetHex != nullptr)
		{
			return GetPathToHex(targetHex);
		}
	}

	return TArray<AHexTile*>();
}


AClockworkUnit* AClockworkUnit::FindClosestEnemyUnit()
{
	if (CurrentHex != nullptr)
	{
		AHexGrid* grid = CurrentHex->GetOwningGrid();
		if (grid != nullptr)
		{
			AClockworkUnit* enemy = grid->GetEnemyUnitClosestTo(this);
			UE_LOG(LogClockwork, Verbose, TEXT("Closest Enemy of %s is %s"), *GetName(), *enemy->GetName());
			
			return enemy;
		}
	}

	UE_LOG(LogClockwork, Verbose, TEXT("%s Has No Available Enemy Target"), *GetName());
	return nullptr;
}


void AClockworkUnit::SelectTargetHex_Server_Implementation()
{
	AHexTile* hex = CurrentHex->GetOwningGrid()->GetRandomVacantHex();

	TargetHex(hex);
}


// -------------------------
// --- Event Handlers
// -------------------------

void AClockworkUnit::HandleHexTargeted(AHexTile* hex)
{
	OnHexTargeted(hex);
}

void AClockworkUnit::HandleHexVacated(AHexTile* hex)
{
	OnHexVacated(hex);
}

void AClockworkUnit::HandleHexReserved(AHexTile* hex)
{
	OnHexReserved(hex);
}

void AClockworkUnit::HandleHexOccupied(AHexTile* hex)
{
	OnHexOccupied(hex);
}
