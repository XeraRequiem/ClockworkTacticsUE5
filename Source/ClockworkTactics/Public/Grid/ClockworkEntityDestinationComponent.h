#pragma once

// Engine
#include "CoreMinimal.h"

// Generated
#include "ClockworkEntityDestinationComponent.generated.h"


// -------------------------
// --- Classes
// -------------------------

UCLASS(ClassGroup = (Clockwork), BlueprintType, Meta = (BlueprintSpawnableComponent, ShortTooltip = "An EntityDestination Component must be added to a Hex to allow Entities to move to it."), MinimalAPI)
class UClockworkEntityDestinationComponent : public USceneComponent
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

protected:


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	UClockworkEntityDestinationComponent();


	// -------------------------
	// --- API
	// -------------------------


};
