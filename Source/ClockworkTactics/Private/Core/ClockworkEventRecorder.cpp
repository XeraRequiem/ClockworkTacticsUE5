// Required
#include "Core/ClockworkEventRecorder.h"

// Game
#include "Core/ClockworkTactics.h"


//-------------------------
// Constructor
//-------------------------

UClockworkEventRecorder::UClockworkEventRecorder() {}


//-------------------------
// Const API
//-------------------------

TArray<FClockworkEvent> UClockworkEventRecorder::GetRecordedEvents() const
{
	return RecordedEvents;
}

TArray<FClockworkEvent> UClockworkEventRecorder::GetRecordedEventsByType(EClockworkEventType EventType) const
{
	TArray<FClockworkEvent> FilteredEvents;
	for (const FClockworkEvent& Event : RecordedEvents)
	{
		if (Event.IsEventType(EventType))
		{
			FilteredEvents.Add(Event);
		}
	}

	return FilteredEvents;
}

TArray<FClockworkEvent> UClockworkEventRecorder::GetRecordedEventsWithinRange(FDateTime StartTime, FDateTime EndTime) const
{
	// Handle edge cass where there are no events
	if (RecordedEvents.Num() == 0)
	{
		return TArray<FClockworkEvent>();
	}

	// Handle edge case where the requested time range encompasses all recorded events
	if (StartTime < RecordedEvents[0].GetEventTime() && EndTime > RecordedEvents[RecordedEvents.Num() - 1].GetEventTime())
	{
		return RecordedEvents;
	}

	// Handle edge case where the requested time range is invalid
	if (StartTime > EndTime)
	{
		return TArray<FClockworkEvent>();
	}

	// Handle edge case where the requested time range is outside the bounds of the recorded events
	if (StartTime > RecordedEvents[RecordedEvents.Num() - 1].GetEventTime() || EndTime < RecordedEvents[0].GetEventTime())
	{
		return TArray<FClockworkEvent>();
	}

	TArray<FClockworkEvent> FilteredEvents;
	for (const FClockworkEvent& Event : RecordedEvents)
	{
		if (Event.GetEventTime() < StartTime)
		{
			continue;
		}

		if (Event.GetEventTime() > EndTime)
		{
			break;
		}

		FilteredEvents.Add(Event);
	}

	return FilteredEvents;
}

TArray<FClockworkEvent> UClockworkEventRecorder::GetRecordedEventsByTypeWithinRange(EClockworkEventType EventType, FDateTime StartTime, FDateTime EndTime) const
{
	// Handle edge cass where there are no events
	if (RecordedEvents.Num() == 0)
	{
		return TArray<FClockworkEvent>();
	}

	// Handle edge case where the requested time range encompasses all recorded events
	if (StartTime < RecordedEvents[0].GetEventTime() && EndTime > RecordedEvents[RecordedEvents.Num() - 1].GetEventTime())
	{
		return RecordedEvents;
	}

	// Handle edge case where the requested time range is invalid
	if (StartTime > EndTime)
	{
		return TArray<FClockworkEvent>();
	}

	// Handle edge case where the requested time range is outside the bounds of the recorded events
	if (StartTime > RecordedEvents[RecordedEvents.Num() - 1].GetEventTime() || EndTime < RecordedEvents[0].GetEventTime())
	{
		return TArray<FClockworkEvent>();
	}

	TArray<FClockworkEvent> FilteredEvents;
	for (const FClockworkEvent& Event : RecordedEvents)
	{
		if (Event.GetEventTime() < StartTime)
		{
			continue;
		}

		if (Event.GetEventTime() > EndTime)
		{
			break;
		}

		if (Event.IsEventType(EventType))
		{
			FilteredEvents.Add(Event);
		}
	}

	return FilteredEvents;
}


FString UClockworkEventRecorder::SerializeAllEvents() const
{

	FString Json = TEXT("{ Events: [");

	for (int i = 0; i < RecordedEvents.Num(); ++i)
	{
		// Serialize each event to JSON and append it to the output string
		Json += RecordedEvents[i].ToJson();

		// Add a comma between events, but not after the last one
		if (i < RecordedEvents.Num() - 1)
		{
			Json += TEXT(",");
		}
	}

	Json += TEXT("]}");

	return Json;
}

FString UClockworkEventRecorder::SerializeEvents(const TArray<FClockworkEvent>& Events) const
{
	FString Json = TEXT("{");
	for (const FClockworkEvent& Event : Events)
	{
		Json += Event.ToJson();
		Json += TEXT(",");
	}

	Json += TEXT("}");

	return Json;
}


//-------------------------
// API
//-------------------------

void UClockworkEventRecorder::GenerateEvent(EClockworkEventType EventType, FInstancedStruct Data)
{
	FClockworkEvent Event(FDateTime::Now(), EventType, Data);

	UE_LOG(LogRecorder, VeryVerbose, TEXT("Generated Event: %s"), *Event.GetEventData().ToString());

	RecordedEvents.Add(Event);
}

void UClockworkEventRecorder::GenerateEventsFromFile(const FString& FileName)
{
	FString JsonPath = FPaths::Combine(BaseFilePath, FileName);

	FString EventsJson;
	if (FFileHelper::LoadFileToString(EventsJson, *JsonPath))
	{
		FClockworkEvents Events = FClockworkEvents::FromJson(EventsJson);
		if (Events.Events.Num() > 0)
		{
			ClearEvents();
			RecordedEvents = Events.Events;
		}
		else
		{
			UE_LOG(LogRecorder, Warning, TEXT("No events found in file: %s"), *FileName);
		}
	}
	else
	{
		UE_LOG(LogRecorder, Error, TEXT("Failed to load events from file: %s"), *FileName);
	}
}

void UClockworkEventRecorder::ClearEvents()
{
	RecordedEvents.Empty();
}


void UClockworkEventRecorder::OutputToFile(bool bClearEvents)
{
	FString SerializedEvents = SerializeAllEvents();

	// Note: The file name should be changed later to indicate the start time of the recording and/or the game mode type.
	FString FilePath = FPaths::Combine(BaseFilePath, FDateTime::Now().ToString() + TEXT(".json"));

	if (FFileHelper::SaveStringToFile(SerializedEvents, *FilePath))
	{
		UE_LOG(LogRecorder, Log, TEXT("Successfully saved events to file: %s"), *FilePath);
	}
	else
	{
		UE_LOG(LogRecorder, Error, TEXT("Failed to save events to file: %s"), *FilePath);
	}

	if (bClearEvents)
	{
		ClearEvents();
	}
}