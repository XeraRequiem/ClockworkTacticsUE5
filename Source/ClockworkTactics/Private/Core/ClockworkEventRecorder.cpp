// Required
#include "Core/ClockworkEventRecorder.h"


//-------------------------
// Static API
//-------------------------

UClockworkEventRecorder* UClockworkEventRecorder::GetEventRecorder()
{
	if (EventRecorderInstance == nullptr)
	{
		EventRecorderInstance = NewObject<UClockworkEventRecorder>();
	}

	return EventRecorderInstance;
}


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


//-------------------------
// API
//-------------------------

void UClockworkEventRecorder::GenerateEvent(EClockworkEventType EventType, FClockworkEventData Data)
{
	RecordedEvents.Add(FClockworkEvent(FDateTime::Now(), EventType, Data));
}
