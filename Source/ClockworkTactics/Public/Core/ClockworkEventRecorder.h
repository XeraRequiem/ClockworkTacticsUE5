#pragma once

// Engine
#include "Containers/SortedSet.h"
#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "StructUtils/InstancedStruct.h"

// Plugin
#include "Core/HexMath.h"

// Generated
#include "ClockworkEventRecorder.generated.h"


//-------------------------
// Enums
//-------------------------

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EClockworkEventType : uint8
{
	None       = 0			UMETA(DisplayName = "None", Hidden),
	Death      = 1 << 0	UMETA(DisplayName = "Death"),
	Spawn      = 1 << 1	UMETA(DisplayName = "Spawn"),
	Attack	  = 1 << 2	UMETA(DisplayName = "Attack"),
	AbilityUse = 1 << 3	UMETA(DisplayName = "Ability Use"),
};

ENUM_CLASS_FLAGS(EClockworkEventType);


//-------------------------
// Structs
//-------------------------

USTRUCT(BlueprintType)
struct FClockworkEventData
{
	GENERATED_BODY()


	//-------------------------
	// Constructors
	//-------------------------

public:
	FClockworkEventData() {}

	virtual ~FClockworkEventData() {}


	//-------------------------
	// Const API
	//-------------------------

public:
	virtual FString ToString() const
	{
		return FString("Base Event Data");
	}

	virtual FString ToJson() const
	{
		return TEXT("NOPE");
	}
};

USTRUCT(BlueprintType)
struct FClockworkAttackEventData : public FClockworkEventData
{
	GENERATED_BODY()


	//-------------------------
	// Member Variables
	//-------------------------

protected:
	UPROPERTY(BlueprintReadOnly)
	FString SourceEntity;

	UPROPERTY(BlueprintReadOnly)
	FString TargetEntity;

	UPROPERTY(BlueprintReadOnly)
	float Damage;


	//-------------------------
	// Constructors
	//-------------------------

public:
	FClockworkAttackEventData() :
		SourceEntity{ "Source" },
		TargetEntity{ "Target" },
		Damage{ 0.0f }
	{}

	FClockworkAttackEventData(const FClockworkEventData& Other)
	{}

	FClockworkAttackEventData(FString Source, FString Target, float Value) :
		SourceEntity{ Source },
		TargetEntity{ Target },
		Damage{ Value }
	{}


	virtual ~FClockworkAttackEventData() {}

	//-------------------------
	// Const API
	//-------------------------

public:
	FString ToString() const override
	{
		return SourceEntity + " dealt " + FString::SanitizeFloat(Damage) + " damage to " + TargetEntity;
	}

	FString ToJson() const override
	{
		FString Json;
		FJsonObjectConverter::UStructToJsonObjectString(StaticStruct(), this, Json);

		return Json;
	}
};

USTRUCT(BlueprintType)
struct FClockworkDeathEventData : public FClockworkEventData
{
	GENERATED_BODY()


	//-------------------------
	// Member Variables
	//-------------------------

protected:
	UPROPERTY(BlueprintReadOnly)
	FString DeadEntity;

	UPROPERTY(BlueprintReadOnly)
	FString KillerEntity;


	//-------------------------
	// Constructors
	//-------------------------

public:
	FClockworkDeathEventData() :
		DeadEntity{ TEXT("DeadEntity") },
		KillerEntity{ TEXT("KillerEntity") }
	{}

	FClockworkDeathEventData(const FString& InDeadEntity, const FString& InKillerEntity) :
		DeadEntity{ InDeadEntity },
		KillerEntity{ InKillerEntity }
	{}


	virtual ~FClockworkDeathEventData() {}

	//-------------------------
	// Const API
	//-------------------------

public:
	FString ToString() const override
	{
		return KillerEntity + " killed " + DeadEntity;
	}

	FString ToJson() const override
	{
		FString Json;
		FJsonObjectConverter::UStructToJsonObjectString(StaticStruct(), this, Json);

		return Json;
	}
};

USTRUCT(BlueprintType)
struct FClockworkSpawnEventData : public FClockworkEventData
{
	GENERATED_BODY()


	//-------------------------
	// Member Variables
	//-------------------------

protected:
	UPROPERTY(BlueprintReadOnly)
	FString Entity;

	UPROPERTY(BlueprintReadOnly)
	FOffsetCoordinate SpawnLocation;


	//-------------------------
	// Constructors
	//-------------------------

public:
	FClockworkSpawnEventData() :
		Entity{ TEXT("Entity") },
		SpawnLocation{ FOffsetCoordinate(0,0) }
	{}

	FClockworkSpawnEventData(const FString& InEntity, const FOffsetCoordinate& InSpawnLocation) :
		Entity{ InEntity },
		SpawnLocation{ InSpawnLocation }
	{}


	virtual ~FClockworkSpawnEventData() {}

	//-------------------------
	// Const API
	//-------------------------

public:
	FString ToString() const override
	{
		return Entity + " Spawned At " + SpawnLocation.ToString();
	}

	FString ToJson() const override
	{
		FString Json;
		FJsonObjectConverter::UStructToJsonObjectString(StaticStruct(), this, Json);

		return Json;
	}
};


USTRUCT(BlueprintType)
struct FClockworkEvent
{
	GENERATED_BODY()


	//-------------------------
	// Member Variables
	//-------------------------

protected:
	UPROPERTY()
	FGuid EventId;

	UPROPERTY(BlueprintReadOnly)
	FDateTime EventTime;

	UPROPERTY(BlueprintReadOnly)
	EClockworkEventType EventType;

	UPROPERTY(BlueprintReadOnly)
	FInstancedStruct EventData;


	//-------------------------
	// Constructors
	//-------------------------

public:
	FClockworkEvent() {}

	FClockworkEvent(FDateTime Time, EClockworkEventType Type, FInstancedStruct& Data) :
		EventId(FGuid::NewGuid()),
		EventTime(Time),
		EventType(Type),
		EventData(Data)
	{}


	//-------------------------
	// Operators
	//-------------------------

	bool operator<(const FClockworkEvent& Other) const
	{
		return EventTime < Other.EventTime;
	}


	//-------------------------
	// Const API
	//-------------------------

	// Data Retrieval

	bool IsEventType(EClockworkEventType Type) const
	{
		return (EventType & Type) != EClockworkEventType::None;
	};

	FDateTime GetEventTime() const
	{
		return EventTime;
	}
	
	
	FClockworkEventData GetEventData() const
	{
		return EventData.Get<FClockworkEventData>();
	}
	
	template <typename T>
	T GetEventDataAs() const
	{
		return EventData.Get<T>();
	}

	// Serialization

	FString ToJson() const
	{
		FString Json;
		FJsonObjectConverter::UStructToJsonObjectString(FClockworkEvent::StaticStruct(), this, Json);

		return Json;
	}


	//-------------------------
	// API
	//-------------------------

public:
	void FromJson(const FString& json)
	{
		// To-Do: Need to check that this works the way I think it does
		FJsonObjectConverter::JsonObjectStringToUStruct(json, this);
	}
};


//-------------------------
// Classes
//-------------------------

// To-Do : Create Event Data Struct For Each Event Type
//		X Create Sorted Array of Events in Event Recorder by Timestamp
//		X Add event handler to record events in the game and add them to the event recorder. I changed this to just allow the event recorder to be called directly.
//		X Add functionality to filter events by type and/or time range
// 	- Add functionality to export events to a file or external system
//		- Add UI to visualize events history and statistics
//		- Add functionality to replay events in the game for debugging or testing purposes
UCLASS(minimalapi)
class UClockworkEventRecorder : public UObject
{
	GENERATED_BODY()


	//-------------------------
	// Member Variables
	//-------------------------

protected:
	// Note: Assumed chronologically sorted based on when events are added to the array.
	UPROPERTY(BlueprintReadOnly)
	TArray<FClockworkEvent> RecordedEvents;


	//-------------------------
	// Constructor
	//-------------------------

public:
	UClockworkEventRecorder();


	//-------------------------
	// Const API
	//-------------------------

public:
	UFUNCTION(BlueprintCallable, Category = "Event Recorder")
	TArray<FClockworkEvent> GetRecordedEvents() const;

	UFUNCTION(BlueprintCallable, Category = "Event Recorder")
	TArray<FClockworkEvent> GetRecordedEventsByType(EClockworkEventType EventType) const;

	UFUNCTION(BlueprintCallable, Category = "Event Recorder")
	TArray<FClockworkEvent> GetRecordedEventsWithinRange(FDateTime StartTime, FDateTime EndTime) const;

	UFUNCTION(BlueprintCallable, Category = "Event Recorder")
	TArray<FClockworkEvent> GetRecordedEventsByTypeWithinRange(EClockworkEventType EventType, FDateTime StartTime, FDateTime EndTime) const;


	UFUNCTION(BlueprintCallable, Category = "Event Recorder", meta=(DisplayName = "SerializeAllEvents"))
	FString SerializeAllEvents() const;

	UFUNCTION(BlueprintCallable, Category = "Event Recorder", meta=(DisplayName = "SerializeEvents"))
	FString SerializeEvents(const TArray<FClockworkEvent>& Events) const;


	//-------------------------
	// API
	//-------------------------

public:
	UFUNCTION(BlueprintCallable, Category = "Event Recorder")
	void GenerateEvent(EClockworkEventType EventType, FInstancedStruct Data);
};
