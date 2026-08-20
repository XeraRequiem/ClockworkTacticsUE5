// Required
#include "Entity/ClockworkHexUnit.h"

// Game
#include "Core/ClockworkGameInstance.h"
#include "Core/ClockworkTactics.h"
#include "Entity/ClockworkHexEntityFactory.h"
#include "Grid/ClockworkGrid.h"
#include "Grid/ClockworkTile.h"


// -------------------------
// --- Constructors
// -------------------------

AClockworkHexUnit::AClockworkHexUnit() :
	 Super()
{
	 FriendlyName = TEXT("Clockwork Hex Unit");
}


// -------------------------
// --- Inherited
// -------------------------

void AClockworkHexUnit::Tick(float dt)
{
	// Target Hex-Based Movement
	if (TargetedHex == nullptr && OccupiedHex != nullptr)
	{
		AClockworkGrid* Grid = OccupiedHex->GetOwningGrid();
		if (Grid != nullptr)
		{
			TargetedHex = Grid->GetRandomVacantHex();
			UE_LOG(LogHex, Verbose, TEXT("%s Targeted Hex %s"), *FriendlyName, *TargetedHex->GetCoordinate().ToString());
		}
	}

	if (OccupiedHex != TargetedHex)
	{
		Move(dt);
	}
	else
	{
		TargetedHex = nullptr;
	}

	Super::Tick(dt);
}


void AClockworkHexUnit::ParseJsonData(const TSharedPtr<FJsonObject>& EntityJson)
{
	Super::ParseJsonData(EntityJson);

	EntityJson->TryGetNumberField(TEXT("Level"), Level);
}

void AClockworkHexUnit::ParseTableData()
{
	Super::ParseTableData();

	UClockworkGameInstance* GameInstance = Cast<UClockworkGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		UDataTable* UnitDataTable = GameInstance->GetClockworkHexEntityFactory()->GetUnitDataTable();
		FClockworkHexUnitTableData* UnitDataEntry = UnitDataTable->FindRow<FClockworkHexUnitTableData>(FName(*FString::FromInt(EntityId)), TEXT(""), true);
		if (UnitDataEntry)
		{
			UnitData.Health = UnitDataEntry->MaxHealth;
			UnitData.Defense = UnitDataEntry->PhysicalDefense;
			UnitData.Resistance = UnitDataEntry->MagicalResistance;
			UnitData.AttackDamage = UnitDataEntry->PhysicalAttackPower;
			UnitData.AttackRange = UnitDataEntry->AttackRange;
			UnitData.AttackSpeed = UnitDataEntry->AttackSpeed;
			UnitData.MagicDamage = UnitDataEntry->MagicalAttackPower;
		}
	}
}


bool AClockworkHexUnit::OccupyHex(AClockworkTile* Hex)
{
	if (Hex != nullptr)
	{
		if (Hex->CanBeOccupiedBy(this))
		{
			if (OccupiedHex != nullptr)
			{
				VacateHex(OccupiedHex);
			}

			Hex->Occupy(this);
			ReservedHex = nullptr;
			OccupiedHex = Hex;

			HandleHexOccupied(Hex);

			UE_LOG(LogHex, Verbose, TEXT("%s Occupied Hex %s"), *FriendlyName, *Hex->GetCoordinate().ToString());
			return true;
		}

		UE_LOG(LogHex, Verbose, TEXT("%s Failed To Occupied Hex %s: Hex Can't be Occupied By This Unit"), *FriendlyName, *Hex->GetCoordinate().ToString());
		return false;
	}

	UE_LOG(LogHex, Verbose, TEXT("%s Failed to Occupy Hex: Invalid Hex"), *FriendlyName);
	return false;
}


// -------------------------
// --- Const API
// -------------------------

AClockworkTile* AClockworkHexUnit::GetTargetHex() const
{
	return TargetedHex;
}

AClockworkTile* AClockworkHexUnit::GetReservedHex() const
{
	return ReservedHex;
}


// -------------------------
// --- API
// -------------------------

bool AClockworkHexUnit::TargetHex(AClockworkTile* Hex)
{
	UE_LOG(LogHex, Verbose, TEXT("%s Selecting New Target Hex..."), *FriendlyName);
	if (Hex != nullptr)
	{
		UE_LOG(LogHex, Verbose, TEXT("%s Targeted %s"), *FriendlyName, *Hex->GetName());

		TargetedHex = Hex;
		HandleHexTargeted(Hex);

		return true;
	}

	UE_LOG(LogHex, Verbose, TEXT("%s Failed To Target Hex: Invalid Hex"), *FriendlyName);
	return false;
}

bool AClockworkHexUnit::VacateHex(AClockworkTile* Hex)
{
	UE_LOG(LogHex, Verbose, TEXT("%s Vacating Hex..."), *FriendlyName);
	if (Hex != nullptr && Hex->CanBeVacatedBy(this))
	{
		Hex->Vacate(this);
		OccupiedHex = nullptr;

		HandleHexVacated(Hex);

		UE_LOG(LogHex, Verbose, TEXT("%s Vacated By %s"), *Hex->GetName(), *FriendlyName);
		return true;
	}

	UE_LOG(LogHex, Verbose, TEXT("%s Failed to Vacate Hex: Invalid Hex"), *FriendlyName);
	return false;
}

bool AClockworkHexUnit::ReserveHex(AClockworkTile* Hex)
{
	UE_LOG(LogHex, Verbose, TEXT("%s Reserving Hex..."), *FriendlyName);
	if (Hex != nullptr && Hex->CanBeReservedBy(this))
	{
		Hex->Reserve(this);
		ReservedHex = Hex;

		HandleHexReserved(Hex);

		UE_LOG(LogHex, Verbose, TEXT("%s Reserved By %s"), *Hex->GetName(), *FriendlyName);

		return true;
	}

	UE_LOG(LogHex, Verbose, TEXT("%s Failed to Reserve Hex: Invalid Hex"), *FriendlyName);
	return false;
}


// -------------------------
// --- Implementation
// -------------------------

void AClockworkHexUnit::Move(float dt)
{
	if (ReservedHex == nullptr)
	{
		TArray<AClockworkTile*> Path = GetPathToHex(TargetedHex);
		if (Path.Num() > 0)
		{
			AClockworkTile* Hex = Path[0];
			ReserveHex(Hex);

			if (EntityData.bDebugMode)
			{
				Hex->GetOwningGrid()->Debug_UpdateHexDistanceFrom(this);
			}
		}
	}

	MoveToReservedHex(dt);
}

void AClockworkHexUnit::MoveToReservedHex(float dt)
{
	if (ReservedHex != nullptr)
	{
		MoveTimeTotal += dt;

		float PercentToTarget = FMath::Clamp(MoveTimeTotal * MoveSpeed, 0.0f, 1.0f);
		UE_LOG(LogHex, Verbose, TEXT("Moving To Reserved Hex %f"), (PercentToTarget * 100.0f));

		FVector WorldLocation = FMath::Lerp(OccupiedHex->GetOccupationLocation(), ReservedHex->GetOccupationLocation(), PercentToTarget);
		SetActorLocation(WorldLocation);

		if (PercentToTarget >= 1.0f)
		{
			MoveTimeTotal = 0.0f;
			OccupyHex(ReservedHex);
		}
	}
}


TArray<AClockworkTile*> AClockworkHexUnit::GetPathToHexWithinTargetRange()
{
	// To-Do

	return TArray<AClockworkTile*>();
}

TArray<AClockworkTile*> AClockworkHexUnit::GetPathToHex(AClockworkTile* Hex)
{
	if (OccupiedHex != nullptr && Hex != nullptr)
	{
		AClockworkGrid* Grid = OccupiedHex->GetOwningGrid();
		if (Grid != nullptr)
		{
			return Grid->GetPathFromTo(OccupiedHex, Hex);
		}
	}

	return TArray<AClockworkTile*>();
}


// -------------------------
// --- Event Handlers
// -------------------------

void AClockworkHexUnit::HandleHexTargeted(AClockworkTile* hex)
{
	OnHexTargeted(hex);
}

void AClockworkHexUnit::HandleHexVacated(AClockworkTile* hex)
{
	OnHexVacated(hex);
}

void AClockworkHexUnit::HandleHexReserved(AClockworkTile* hex)
{
	OnHexReserved(hex);
}

void AClockworkHexUnit::HandleHexOccupied(AClockworkTile* Hex)
{
	OnHexOccupied(Hex);
}
