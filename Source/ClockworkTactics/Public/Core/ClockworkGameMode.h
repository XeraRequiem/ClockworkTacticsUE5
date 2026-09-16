#pragma once

// Engine
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

// Game
#include "Core/ClockworkCharacter.h"
#include "Grid/ClockworkGrid.h"

// Generated
#include "ClockworkGameMode.generated.h"


//-------------------------
// Enums
//-------------------------

UENUM()
enum class EGameType : uint8
{
	Time, /* Limited Time */
	Ticket, /* Limited Number of Enemies */
	Lives /* Unlimited Enemies, Limited Lives */
};


//-------------------------
// Structs
//-------------------------

/*
* Time-Limited Game Mode. Defaults to 5min.
*		Optional: Limited Lives. Defaults to Off
*/
USTRUCT()
struct FTimeGameTypeData
{
	GENERATED_BODY()

	//-------------------------
	// Member Variables
	//-------------------------

private:
	FTimespan Duration{ FTimespan::FromMinutes(5.0) };

	FDateTime StartTime;
	FDateTime EndTime;

	uint8 MaxLives{ 0 };
	uint8 CurrentLives{ 0 };


	//-------------------------
	// Constructors
	//-------------------------

public:
	FTimeGameTypeData() {}

	FTimeGameTypeData(FTimespan InDuration) :
		Duration(InDuration)
	{}

	FTimeGameTypeData(FTimespan InDuration, uint8 InMaxLives) :
		Duration(InDuration),
		MaxLives(InMaxLives),
		CurrentLives(InMaxLives)
	{}


	//-------------------------
	// API
	//-------------------------

public:
	// --------------- Time --------------- //

	void Start()
	{
		StartTime = FDateTime::Now();
		EndTime = StartTime + Duration;
	}

	FTimespan GetTimeRemaining()
	{
		if (FDateTime::Now() >= EndTime)
		{
			return FTimespan::FromSeconds(0);
		}

		FTimespan TimeElapsed = FDateTime::Now() - StartTime;

		return Duration - (FDateTime::Now() - StartTime);
	}

	bool IsPastTime()
	{
		return FDateTime::Now() >= EndTime;
	}

	bool IsStarted()
	{
		return Duration.GetTicks() > 0;
	}

	// --------------- Lives --------------- //

	uint8 GetMaxLives()
	{
		return MaxLives;
	}

	uint8 GetRemainingtLives()
	{
		return CurrentLives;
	}

	bool HasLivesRemaining()
	{
		return CurrentLives > 0;
	}

	bool HasLimitedLives()
	{
		return MaxLives > 0;
	}

	void ReduceLives(uint8 Lives = 1)
	{
		CurrentLives -= (Lives > CurrentLives) ? CurrentLives : Lives;
	}
};

/*
* Ticket/Enemy limited Game Mode. Defaults to 10 Tickets
*		Optional: Limited Lives. Defaults to Off
*/
USTRUCT()
struct FTicketGameTypeData
{
	GENERATED_BODY()

	//-------------------------
	// Member Variables
	//-------------------------

private:
	uint8 MaxTickets{ 10 };
	uint8 CurrentTickets{ 0 };


	uint8 MaxLives{ 0 };
	uint8 CurrentLives{ 0 };


	//-------------------------
	// Constructors
	//-------------------------

public:
	FTicketGameTypeData() {}

	FTicketGameTypeData(uint8 InMaxTickets) :
		MaxTickets(InMaxTickets)
	{}

	FTicketGameTypeData(uint8 InMaxTickets, uint8 InMaxLives) :
		MaxTickets(InMaxTickets),
		MaxLives(InMaxLives),
		CurrentLives(InMaxLives)
	{}


	//-------------------------
	// API
	//-------------------------

public:
	// --------------- Tickets --------------- //
	
	uint8 GetRemainingTickets()
	{
		return CurrentTickets;
	}

	uint8 GetMaxTickets()
	{
		return MaxTickets;
	}

	void RemoveTickets(uint8 TicketsToRemove = 1)
	{
		CurrentTickets -= (TicketsToRemove > CurrentTickets) ? CurrentTickets : TicketsToRemove;
	}

	// --------------- Lives --------------- //

	int32 GetMaxLives()
	{
		return MaxLives;
	}

	int32 GetRemainingtLives()
	{
		return CurrentLives;
	}

	bool HasLivesRemaining()
	{
		return CurrentLives > 0;
	}

	bool HasLimitedLives()
	{
		return MaxLives > 0;
	}

	void ReduceLives(uint8 Lives = 1)
	{
		CurrentLives -= (Lives > CurrentLives) ? CurrentLives : Lives;
	}
};

/*
* Unlimited Time & Tickets/Enemies. Limited Number of Lives Only.
* Defaults to 5 Lives
*/
USTRUCT()
struct FLivesGameTypeData
{
	GENERATED_BODY()

	//-------------------------
	// Member Variables
	//-------------------------

private:
	FDateTime StartTime;
	uint8 MaxLives{ 5 };
	uint8 CurrentLives{ 5 };

	bool bStarted{ false }


	//-------------------------
	// Constructors
	//-------------------------

public;
	FLivesGameTypeData() {}

	FLivesGameTypeData(uint8 Lives) :
		MaxLives(Lives),
		CurrentLives(Lives)
	{}


	//-------------------------
	// API
	//-------------------------

public:
	void Start()
	{
		StartTime = FDateTime::Now();
	}


	FTimespan GetDuration()
	{
		if (!bStarted)
		{
			return FTimespan::FromSeconds(0);
		}

		return FDateTime::Now() - StartTime;
	}


	uint8 GetMaxLives()
	{
		return MaxLives;
	}

	uint8 GetRemainingLives()
	{
		return CurrentLives;
	}

	bool HasLivesRemaining()
	{
		return CurrentLives > 0;
	}

	void ReduceLives(uint8 Lives = 1)
	{
		CurrentLives -= (Lives > CurrentLives) ? CurrentLives : Lives;
	}
};


//-------------------------
// Classes
//-------------------------

UCLASS(minimalapi)
class AClockworkGameMode : public AGameModeBase
{
	GENERATED_BODY()


	//-------------------------
	// Member Variables
	//-------------------------
public:
	// --------------- Class Defaults --------------- //

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AClockworkCharacter> CharacterClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AClockworkGrid> GridClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<AActor> GridSpawnPoint;

	// --------------- Grid Data --------------- //

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 GridWidth{ 5 };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 GridDepth{ 5 };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bGridVariableHeight{ false };

	// --------------- File Layout --------------- //

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bGenerateGridFromLayoutFile{ false };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString GridLayoutFile;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString EntityLayoutFile;

	// --------------- Scoring --------------- //

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EGameType GameType { EGameType::Ticket };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 TotalLives{ 5 };

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentLives{ 5 };

	UPROPERTY(BlueprintReadOnly)
	int32 TotalEnemies{ 15 };

	UPROPERTY(BlueprintReadOnly)
	int32 EnemyDefeatCount{ 0 };


	// --------------- Debug --------------- //

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bDebugMode{ false };


	//-------------------------
	// Constructor
	//-------------------------

public:
	AClockworkGameMode();


	//-------------------------
	// Public API
	//-------------------------
};
