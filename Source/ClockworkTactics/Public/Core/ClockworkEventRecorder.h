#pragma once

// Engine
#include "Containers/SortedSet.h"
#include "CoreMinimal.h"

// Game

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

	FClockworkAttackEventData(FString Source, FString Target, float Value) :
		SourceEntity{ Source },
		TargetEntity{ Target },
		Damage{ Value }
	{}


	~FClockworkAttackEventData() override {}

	//-------------------------
	// Const API
	//-------------------------

public:
	FString ToString() const override
	{
		return SourceEntity + " dealt " + FString::SanitizeFloat(Damage) + " to " + TargetEntity;
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
	FString Entity;
		
	//-------------------------
	// Constructors
	//-------------------------

public:
	FClockworkDeathEventData() :
		Entity{ "Entity" }
	{}

	FClockworkDeathEventData(FString InEntity) :
		Entity{ InEntity }
	{}


	~FClockworkDeathEventData() override {}

	//-------------------------
	// Const API
	//-------------------------

public:
	FString ToString() const override
	{
		return Entity + "Died";
	}
};

// To-Do : Create Event Data Struct For Each Event Type
//		X Create Sorted Array of Events in Event Recorder by Timestamp
//		- Add event handler to record events in the game and add them to the event recorder
//		- Add functionality to filter events by type and/or time range
// 	- Add functionality to export events to a file or external system
//		- Add UI to visualize events history and statistics
//		- Add functionality to replay events in the game for debugging or testing purposes


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
	FClockworkEventData EventData;


	//-------------------------
	// Constructors
	//-------------------------

public:
	FClockworkEvent() {}

	FClockworkEvent(FDateTime Time, EClockworkEventType Type, FClockworkEventData Data) :
		EventTime(Time),
		EventType(Type),
		EventData(Data)
	{}


	//-------------------------
	// Const API
	//-------------------------

	// Data Retrieval

	bool IsEventType(EClockworkEventType Type) const
	{
		return (EventType & Type) != EClockworkEventType::None;
	};
	
	template <typename T>
	T GetEventData() const
	{
		return static_cast<T>(EventData);
	}

	FDateTime GetEventTime() const
	{
		return EventTime;
	}

	// Comparison Operators

	bool operator<(const FClockworkEvent& Other) const
	{
		return EventTime < Other.EventTime;
	}
};


//-------------------------
// Classes
//-------------------------

UCLASS(minimalapi)
class UClockworkEventRecorder : public UObject
{
	GENERATED_BODY()


	//-------------------------
	// Static Variables
	//-------------------------

private:
	static UClockworkEventRecorder* EventRecorderInstance;


	//-------------------------
	// Static API
	//-------------------------

public:
	static UClockworkEventRecorder* GetEventRecorder()
	{
		if (EventRecorderInstance == nullptr)
		{
			EventRecorderInstance = NewObject<UClockworkEventRecorder>();
		}

		return EventRecorderInstance;
	}


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

private:
	UClockworkEventRecorder();


	//-------------------------
	// Const API
	//-------------------------

	UFUNCTION(BlueprintCallable, Category = "Event Recorder")
	TArray<FClockworkEvent> GetRecordedEvents() const;

	UFUNCTION(BlueprintCallable, Category = "Event Recorder")
	TArray<FClockworkEvent> GetRecordedEventsByType(EClockworkEventType EventType) const;

	UFUNCTION(BlueprintCallable, Category = "Event Recorder")
	TArray<FClockworkEvent> GetRecordedEventsWithinRange(FDateTime StartTime, FDateTime EndTime) const;

	UFUNCTION(BlueprintCallable, Category = "Event Recorder")
	TArray<FClockworkEvent> GetRecordedEventsByTypeWithinRange(EClockworkEventType EventType, FDateTime StartTime, FDateTime EndTime) const;


	//-------------------------
	// API
	//-------------------------

public:
	UFUNCTION(BlueprintCallable, Category = "Event Recorder")
	void GenerateEvent(EClockworkEventType EventType, FClockworkEventData Data);
};
