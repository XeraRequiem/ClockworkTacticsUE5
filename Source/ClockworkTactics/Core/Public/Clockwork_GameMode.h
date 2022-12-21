#pragma once

// Engine
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

// Game
#include "Clockwork_Character.h"

#include "Clockwork_GameMode.generated.h"

UCLASS(minimalapi)
class AClockwork_GameMode : public AGameModeBase
{
	GENERATED_BODY()


	//-------------------------
	// Member Variables
	//-------------------------
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AClockwork_Character> CharacterClass;


	//-------------------------
	// Constructors
	//-------------------------

public:
	AClockwork_GameMode();
};



