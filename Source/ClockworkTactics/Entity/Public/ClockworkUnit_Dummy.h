#pragma once

// Game
#include "ClockworkUnit.h"

#include "ClockworkUnit_Dummy.generated.h"


// -------------------------
// --- Classes
// -------------------------

UCLASS()
class AClockworkUnit_Dummy : public AClockworkUnit
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
	AClockworkUnit_Dummy();


	// -------------------------
	// --- Inherited
	// -------------------------

public:
	virtual void Tick(float dt) override;


	// -------------------------
	// --- API
	// -------------------------

public:


	// -------------------------
	// --- Implementation
	// -------------------------

protected:


	// -------------------------
	// --- Blueprint Events
	// -------------------------

protected:


	// -------------------------
	// --- Event Handlers
	// -------------------------
};
