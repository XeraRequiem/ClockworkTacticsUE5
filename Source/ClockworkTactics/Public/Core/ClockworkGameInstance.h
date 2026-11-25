#pragma once

// Engine
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

// Game
#include "Core/ClockworkEventRecorder.h"
#include "Entity/ClockworkHexEntityFactory.h"

// Generated
#include "ClockworkGameInstance.generated.h"


UCLASS()
class UClockworkGameInstance : public UGameInstance
{
	GENERATED_BODY()


	//-------------------------
	// Member Variables
	//-------------------------

private:
	UPROPERTY()
	UClockworkHexEntityFactory* ClockworkEntityFactory;

	UPROPERTY()
	UClockworkEventRecorder* ClockworkEventRecorder;


	//-------------------------
	// Constructor
	//-------------------------

public:
	UClockworkGameInstance();


	//-------------------------
	// Const API
	//-------------------------

public:
	UClockworkHexEntityFactory* GetClockworkHexEntityFactory() const { return ClockworkEntityFactory; }

	UClockworkEventRecorder* GetClockworkEventRecorder() const { return ClockworkEventRecorder; }
};



