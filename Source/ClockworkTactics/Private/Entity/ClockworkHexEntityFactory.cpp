// Required
#include "Entity/ClockworkHexEntityFactory.h"

// Engine
#include "Dom/JsonValue.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonSerializer.h"

// Game
#include "Core/ClockworkTactics.h"
#include "Entity/ClockworkHexUnit.h"
#include "Entity/ClockworkHexStructure.h"
#include "Entity/ClockworkHexCharacter.h"


// -------------------------
// --- Constructors
// -------------------------

UClockworkHexEntityFactory::UClockworkHexEntityFactory() : 
	 Super()
{}


// -------------------------
// --- API
// -------------------------

void UClockworkHexEntityFactory::Initialize()
{
	CreateEntityDatatables();
	GenerateEntities();
}


// -------------------------
// --- Const API
// -------------------------

UDataTable* UClockworkHexEntityFactory::GetEntityDataTable() const
{
	return EntityDataTable;
}

UDataTable* UClockworkHexEntityFactory::GetUnitDataTable() const
{
	return UnitDataTable;
}

UDataTable* UClockworkHexEntityFactory::GetStructureDataTable() const
{
	return StructureDataTable;
}

UDataTable* UClockworkHexEntityFactory::GetCharacterDataTable() const
{
	return CharacterDataTable;
}	


// -------------------------
// --- Implementation
// -------------------------

// Entity Databases (CSV Files) are loaded into DataTables at runtime. This allows for easy editing of entity data without needing to recompile the game.

void UClockworkHexEntityFactory::CreateEntityDatatables()
{
	EntityDataTable = CreateEntityDatatable(TEXT("EntityData"));
	UnitDataTable = CreateEntityDatatable(TEXT("UnitData"));
	StructureDataTable = CreateEntityDatatable(TEXT("StructureData"));
}

UDataTable* UClockworkHexEntityFactory::CreateEntityDatatable(const FString TableName)
{
	FString EntityDataCSV;
	FFileHelper::LoadFileToString(EntityDataCSV, *FPaths::Combine(EntityDataPath, TableName + TEXT(".csv")));

	UDataTable* Datatable = NewObject<UDataTable>(this, UDataTable::StaticClass(), *TableName);

	TArray<FString> TableCreationErrors;
	TableCreationErrors = Datatable->CreateTableFromCSVString(EntityDataCSV);

	if (TableCreationErrors.Num() > 0)
	{
		for (const FString& Error : TableCreationErrors)
		{
			UE_LOG(LogClockwork, Error, TEXT("Error creating Entity Data Table: %s"), *Error);
		}
	}

	return Datatable;
}

// 

void UClockworkHexEntityFactory::GenerateEntities()
{
	TSharedPtr<FJsonObject> EntitiesJson = MakeShared<FJsonObject>();
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(EntityDataPath);

	if (FJsonSerializer::Deserialize(Reader, EntitiesJson))
	{
		TArray<TSharedPtr<FJsonValue>> EntitiesJsonArray = EntitiesJson->GetArrayField(TEXT("Entities"));

		for (const TSharedPtr<FJsonValue>& EntityJsonValue : EntitiesJsonArray)
		{
			GenerateEntity(EntityJsonValue->AsObject());
		}
	}
}

void UClockworkHexEntityFactory::GenerateEntity(TSharedPtr<FJsonObject> JsonData)
{
	int EntityId = JsonData->GetIntegerField(TEXT("EntityId"));
	FOffsetCoordinate EntityLocation = FOffsetCoordinate::FromString(JsonData->GetStringField(TEXT("Location")));
	FString EntityType = JsonData->GetStringField(TEXT("Type"));

	UEnum* StaticEntityTypeEnum = StaticEnum<EClockworkHexEntityType>();
	int64 EntityTypeEnumIndex = StaticEntityTypeEnum->GetValueByName(FName(EntityType));

	if (EntityTypeEnumIndex != INDEX_NONE)
	{
		TSharedPtr<FJsonObject> EntityDataJson = JsonData->GetObjectField(TEXT("Data"));
			
		EClockworkHexEntityType EntityTypeEnum = static_cast<EClockworkHexEntityType>(EntityTypeEnumIndex);
		switch (EntityTypeEnum)
		{
			case EClockworkHexEntityType::Unit:
				GenerateEntityByType(AClockworkHexUnit::StaticClass(), EntityId, EntityLocation, EntityDataJson);
				break;
			case EClockworkHexEntityType::Structure:
				GenerateEntityByType(AClockworkHexStructure::StaticClass(), EntityId, EntityLocation, EntityDataJson);
				break;
			case EClockworkHexEntityType::Character:
				GenerateEntityByType(AClockworkHexCharacter::StaticClass(), EntityId, EntityLocation, EntityDataJson);
				break;
			default:
				// Handle invalid or unsupported entity types.
				break;
		}
	}
}

void UClockworkHexEntityFactory::GenerateEntityByType(TSubclassOf<AClockworkHexEntity> EntityClass, int32 EntityId, FOffsetCoordinate Coordinate, TSharedPtr<FJsonObject> JsonData)
{
	AClockworkHexEntity* Entity = GetWorld()->SpawnActor<AClockworkHexEntity>(EntityClass);
	Entity->InitializeFromData(EntityId, Coordinate, JsonData);
	Entities.Add(Entity);
}

//FClockworkHexUnitData UClockworkHexEntityFactory::ConstructUnitData(TSharedPtr<FJsonObject> UnitJson, int32 EntityId, const FString& EntityLocation)
//{
//	// Parse the unit data from the JSON object and populate the FClockworkHexUnitData struct.
//
//	FClockworkHexUnitData UnitData = ParseUnitData(UnitJson);
//	UnitData.EntityId = EntityId;
//	UnitData.Location = FOffsetCoordinate::FromString(EntityLocation);
//
//	// Retrieve data from the EntityDataTable based on the EntityId.
//
//	FClockworkHexEntityTableData* EntityDataEntry = EntityDataTable->FindRow<FClockworkHexEntityTableData>(FName(*FString::FromInt(EntityId)), TEXT(""), true);
//	if (EntityDataEntry)
//	{
//		UnitData.FriendlyName = EntityDataEntry->FriendlyName;
//		UnitData.bPassable = EntityDataEntry->bPassable;
//		UnitData.bTargetable = EntityDataEntry->bTargetable;
//		UnitData.bDestructible = EntityDataEntry->bDestructible;
//	}
//
//	// Retrieve data from the UnitDataTable based on the EntityId.
//
//	FClockworkHexUnitTableData* UnitDataEntry = UnitDataTable->FindRow<FClockworkHexUnitTableData>(FName(*FString::FromInt(EntityId)), TEXT(""), true);
//	if (UnitDataEntry)
//	{
//		UnitData.MaxHealth = UnitDataEntry->MaxHealth;
//		UnitData.MagicalResistance = UnitDataEntry->MagicalResistance;
//		UnitData.PhysicalDefense = UnitDataEntry->PhysicalDefense;
//		UnitData.PhysicalAttackPower = UnitDataEntry->PhysicalAttackPower;
//		UnitData.AttackSpeed = UnitDataEntry->AttackSpeed;
//		UnitData.AttackRange = UnitDataEntry->AttackRange;
//		UnitData.MagicalAttackPower = UnitDataEntry->MagicalAttackPower;
//	}
//
//	return UnitData;
//}
//
//FClockworkHexCharacterData UClockworkHexEntityFactory::ConstructCharacterData(TSharedPtr<FJsonObject> CharacterJson, int32 EntityId, const FString& EntityLocation)
//{
//	// Parse the character data from the JSON object and populate the FClockworkHexCharacterData struct.
//
//	FClockworkHexCharacterData CharacterData = ParseCharacterData(CharacterJson);
//	CharacterData.EntityId = EntityId;
//	CharacterData.Location = FOffsetCoordinate::FromString(EntityLocation);
//
//	// Retrieve data from the EntityDataTable based on the EntityId.
//
//	FClockworkHexEntityTableData* EntityDataEntry = EntityDataTable->FindRow<FClockworkHexEntityTableData>(FName(*FString::FromInt(EntityId)), TEXT(""), true);
//	if (EntityDataEntry)
//	{
//		CharacterData.FriendlyName = EntityDataEntry->FriendlyName;
//		CharacterData.bPassable = EntityDataEntry->bPassable;
//		CharacterData.bTargetable = EntityDataEntry->bTargetable;
//		CharacterData.bDestructible = EntityDataEntry->bDestructible;
//	}
//
//	// Retrieve data from the UnitDataTable based on the EntityId.
//
//	FClockworkHexUnitTableData* UnitDataEntry = UnitDataTable->FindRow<FClockworkHexUnitTableData>(FName(*FString::FromInt(EntityId)), TEXT(""), true);
//	if (UnitDataEntry)
//	{
//		CharacterData.MaxHealth = UnitDataEntry->MaxHealth;
//		CharacterData.MagicalResistance = UnitDataEntry->MagicalResistance;
//		CharacterData.PhysicalDefense = UnitDataEntry->PhysicalDefense;
//		CharacterData.PhysicalAttackPower = UnitDataEntry->PhysicalAttackPower;
//		CharacterData.AttackSpeed = UnitDataEntry->AttackSpeed;
//		CharacterData.AttackRange = UnitDataEntry->AttackRange;
//		CharacterData.MagicalAttackPower = UnitDataEntry->MagicalAttackPower;
//	}
//
//	return CharacterData;
//}
//
//FClockworkHexStructureData UClockworkHexEntityFactory::ConstructStructureData(TSharedPtr<FJsonObject> StructureJson, int32 EntityId, const FString& EntityLocation)
//{
//	// Parse the character data from the JSON object and populate the FClockworkHexCharacterData struct.
//
//	FClockworkHexStructureData StructureData = ParseStructureData(StructureJson);
//	StructureData.EntityId = EntityId;
//	StructureData.Location = FOffsetCoordinate::FromString(EntityLocation);
//
//	// Retrieve data from the EntityDataTable based on the EntityId.
//
//	FClockworkHexEntityTableData* EntityDataEntry = EntityDataTable->FindRow<FClockworkHexEntityTableData>(FName(*FString::FromInt(EntityId)), TEXT(""), true);
//	if (EntityDataEntry)
//	{
//		StructureData.FriendlyName = EntityDataEntry->FriendlyName;
//		StructureData.bPassable = EntityDataEntry->bPassable;
//		StructureData.bTargetable = EntityDataEntry->bTargetable;
//		StructureData.bDestructible = EntityDataEntry->bDestructible;
//	}
//
//	// Retrieve data from the StructureDataTable based on the EntityId.
//
//	FClockworkHexStructureTableData* StructureDataEntry = StructureDataTable->FindRow<FClockworkHexStructureTableData>(FName(*FString::FromInt(EntityId)), TEXT(""), true);
//	if (StructureDataEntry)
//	{
//		StructureData.MaxDurability = StructureDataEntry->MaxDurability;
//	}
//
//	return StructureData;
//}
//
////
//
//FClockworkHexUnitData UClockworkHexEntityFactory::ParseUnitData(TSharedPtr<FJsonObject> UnitJson)
//{
//	FClockworkHexUnitData UnitData = FClockworkHexUnitData();
//
//	UnitJson->TryGetNumberField(TEXT("Level"), UnitData.Level);
//
//	return UnitData;
//}
//
//FClockworkHexCharacterData UClockworkHexEntityFactory::ParseCharacterData(TSharedPtr<FJsonObject> CharacterJson)
//{
//	FClockworkHexCharacterData CharacterData = FClockworkHexCharacterData();
//
//	CharacterJson->TryGetNumberField(TEXT("Level"), CharacterData.Level);
//	CharacterJson->TryGetNumberField(TEXT("Experience"), CharacterData.Experience);
//	CharacterJson->TryGetNumberField(TEXT("WeaponId"), CharacterData.WeaponId);
//	CharacterJson->TryGetNumberField(TEXT("ArmorId"), CharacterData.ArmorId);
//	CharacterJson->TryGetNumberField(TEXT("AccessoryId"), CharacterData.AccessoryId);
//
//	return CharacterData;
//}
//
//FClockworkHexStructureData UClockworkHexEntityFactory::ParseStructureData(TSharedPtr<FJsonObject> StructureJson)
//{
//	FClockworkHexStructureData StructureData = FClockworkHexStructureData();
//
//	StructureJson->TryGetNumberField(TEXT("Durability"), StructureData.MaxDurability);
//
//	return StructureData;
//}
