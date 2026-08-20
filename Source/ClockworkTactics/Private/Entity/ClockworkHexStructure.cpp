// Required
#include "Entity/ClockworkHexStructure.h"

// Game
#include "Core/ClockworkGameInstance.h"
#include "Core/ClockworkTactics.h"


// -------------------------
// --- Constructors
// -------------------------

AClockworkHexStructure::AClockworkHexStructure() :
	 Super()
{
	 FriendlyName = TEXT("Clockwork Hex Structure");
}


// -------------------------
// --- Inherited
// -------------------------

void AClockworkHexStructure::ParseJsonData(const TSharedPtr<FJsonObject>& EntityJson)
{
	Super::ParseJsonData(EntityJson);

	EntityJson->TryGetNumberField(TEXT("Durability"), StructureData.CurrentDurability);

}

void AClockworkHexStructure::ParseTableData()
{
	Super::ParseTableData();

	UClockworkGameInstance* GameInstance = Cast<UClockworkGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		UDataTable* StructureDataTable = GameInstance->GetClockworkHexEntityFactory()->GetStructureDataTable();
		FClockworkHexStructureTableData* StructureDataEntry = StructureDataTable->FindRow<FClockworkHexStructureTableData>(FName(*FString::FromInt(EntityId)), TEXT(""), true);
		if (StructureDataEntry)
		{
			StructureData.MaxDurability = StructureDataEntry->MaxDurability;
		}
	}
}


// -------------------------
// --- API
// -------------------------


// -------------------------
// --- Const API
// -------------------------


// -------------------------
// --- Implementation
// -------------------------


// -------------------------
// --- Event Handlers
// -------------------------
