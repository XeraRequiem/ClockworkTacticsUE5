// Required
#include "Entity/ClockworkHexEntity.h"

// Game
#include "Core/ClockworkGameInstance.h"
#include "Core/ClockworkTactics.h"
#include "Core/ClockworkWorldSubsystem.h"
#include "Entity/ClockworkHexEntityFactory.h"
#include "Grid/ClockworkTile.h"


// -------------------------
// --- Constructors
// -------------------------

AClockworkHexEntity::AClockworkHexEntity() : 
	 Super()
{
	 FriendlyName = TEXT("Clockwork Hex Entity");

	 PrimaryActorTick.bCanEverTick = true;
	 PrimaryActorTick.bTickEvenWhenPaused = true;
	 PrimaryActorTick.TickGroup = TG_PrePhysics;
}


// -------------------------
// --- API
// -------------------------

void AClockworkHexEntity::Initialize(AClockworkTile* Hex)
{
	EntityGuid = FGuid::NewGuid();

	if (Hex != nullptr)
	{
		OccupyHex(Hex);
	}
}

void AClockworkHexEntity::InitializeFromData(int32 InEntityId, const FOffsetCoordinate& InLocation, const TSharedPtr<FJsonObject>& InEntityJson)
{
	EntityGuid = FGuid::NewGuid();
	EntityId = InEntityId;
	
	UClockworkWorldSubsystem* WorldSubsystem = GetWorld()->GetSubsystem<UClockworkWorldSubsystem>();
	if (WorldSubsystem != nullptr)
	{
		AClockworkTile* Hex = WorldSubsystem->GetHexGrid()->GetHexAtCoordinate(InLocation);
		if (Hex != nullptr)
		{
			HexToOccupy = Hex;
		}
	}

	// Retrieve data from Entity Json "Data" block
	ParseJsonData(InEntityJson);

	// Retrieve data from the EntityDataTable based on the EntityId.
	ParseTableData();
}


void AClockworkHexEntity::OccupyPendingTile()
{
	if (HexToOccupy != nullptr && HexToOccupy->Occupy(this))
	{
		OccupiedHex = HexToOccupy;
		HexToOccupy = nullptr;
	}
}

bool AClockworkHexEntity::OccupyHex(AClockworkTile* Hex)
{
	if (Hex->CanBeOccupiedBy(this))
	{
		Hex->Occupy(this);
		OccupiedHex = Hex;
		return true;
	}

	return false;
}


// -------------------------
// --- Const API
// -------------------------

FString AClockworkHexEntity::SerializeEntityData() const
{
	 FString SerializedData = TEXT("{");
	 SerializedData += FString::Printf(TEXT("\"EntityId\": %d, "), EntityId);
	 SerializedData += FString::Printf(TEXT("\"Location\": \"%s\", "), *OccupiedHex->GetGridCoordinate().ToString());
	 SerializedData += FString::Printf(TEXT("\"Type\": \"%s\""), *GetName());
	 SerializedData += TEXT("}");

	 return SerializedData;
}

AClockworkTile* AClockworkHexEntity::GetOccupiedHex() const
{
	return OccupiedHex;
}


// -------------------------
// --- Implementation
// -------------------------

bool AClockworkHexEntity::SpawnEntityAtOccupiedTile()
{
	 if (OccupiedHex)
	 {
		  SetActorLocation(OccupiedHex->GetOccupationLocation());
		  return true;
	 }

	 return false;
}


void AClockworkHexEntity::ParseJsonData(const TSharedPtr<FJsonObject>& EntityJson) {}

void AClockworkHexEntity::ParseTableData()
{
	UClockworkGameInstance* GameInstance = Cast<UClockworkGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance != nullptr)
	{
		UDataTable* EntityDataTable = GameInstance->GetClockworkHexEntityFactory()->GetEntityDataTable();
		FClockworkHexEntityTableData* EntityDataEntry = EntityDataTable->FindRow<FClockworkHexEntityTableData>(FName(*FString::FromInt(EntityId)), TEXT(""), true);
		if (EntityDataEntry)
		{
			FriendlyName = EntityDataEntry->FriendlyName;
			EntityData.bPassable = EntityDataEntry->bPassable;
			EntityData.bTargetable = EntityDataEntry->bTargetable;
			EntityData.bDestructible = EntityDataEntry->bDestructible;
		}
	}
}
