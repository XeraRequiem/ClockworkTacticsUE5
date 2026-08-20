#pragma once

// Engine
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

// Game
#include "Core/ClockworkCharacter.h"
#include "Grid/ClockworkGrid.h"

// Generated
#include "ClockworkGameMode.generated.h"


UCLASS(minimalapi)
class AClockworkGameMode : public AGameModeBase
{
	GENERATED_BODY()


	//-------------------------
	// Member Variables
	//-------------------------
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AClockworkCharacter> CharacterClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AClockworkGrid> GridClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<AActor> GridSpawnPoint;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 GridWidth{ 5 };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 GridDepth{ 5 };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bGridVariableHeight{ false };


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bGenerateGridFromLayoutFile{ false };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString GridLayoutFile;


	//-------------------------
	// Constructor
	//-------------------------

public:
	AClockworkGameMode();
};
