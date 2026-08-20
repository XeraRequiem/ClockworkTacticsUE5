#pragma once

// Engine
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

// Game
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
public:
	UPROPERTY()
	UClockworkHexEntityFactory* ClockworkEntityFactory;


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
};



