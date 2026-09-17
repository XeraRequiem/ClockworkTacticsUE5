#pragma once

// Engine
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

// Game
#include "Core/ClockworkCharacter.h"
#include "Core/ClockworkStatics.h"
#include "Grid/ClockworkGrid.h"

// Generated
#include "ClockworkGameMode.generated.h"


//-------------------------
// Enums
//-------------------------

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EGameType : uint8
{
	None		= 0 UMETA(Hidden),
	Time		= 1 << 0, /* Limited Time */
	Ticket	= 1 << 1, /* Limited Number of Enemies */
	Lives		= 4 << 2  /* Limited Lives */
};
ENUM_CLASS_FLAGS(EGameType);


//-------------------------
// Structs
//-------------------------

/*
* Time-Limited Game Mode. Defaults to 5min.
*/
USTRUCT(BlueprintType)
struct FClockworkTimeData
{
	GENERATED_BODY()

	//-------------------------
	// Member Variables
	//-------------------------

private:
	UPROPERTY()
	FTimespan Duration{ FTimespan::FromMinutes(0.0) };

	UPROPERTY()
	FDateTime StartTime{ FDateTime::FDateTime() };

	UPROPERTY()
	FDateTime EndTime{ FDateTime::FDateTime() };


	UPROPERTY()
	bool bTimeLimited{ true };


	//-------------------------
	// Constructors
	//-------------------------

public:
	FClockworkTimeData() {}

	FClockworkTimeData(FTimespan InDuration) :
		Duration(InDuration)
	{}


	//-------------------------
	// API
	//-------------------------

public:
	void Start()
	{
		StartTime = FDateTime::Now();

		if (bTimeLimited)
		{
			EndTime = StartTime + Duration;
		}
	}


	FTimespan GetTimeRemaining()
	{
		return (IsPastTime()) ? FTimespan::FromSeconds(0) : EndTime - FDateTime::Now();
	}

	FTimespan GetTimeElapsed()
	{
		return FDateTime::Now() - StartTime;
	}

	bool IsPastTime()
	{
		return FDateTime::Now() >= EndTime;
	}

	bool HasStarted()
	{
		return Duration.GetTicks() > 0;
	}
};

/*
* Ticket/Enemy limited Game Mode. Defaults to 10 Tickets
*/
USTRUCT(BlueprintType)
struct FClockworkTicketData
{
	GENERATED_BODY()

	//-------------------------
	// Member Variables
	//-------------------------

private:
	UPROPERTY()
	int16 MaxTickets{ 10 };

	UPROPERTY()
	int16 CurrentTickets{ 0 };


	//-------------------------
	// Constructors
	//-------------------------

public:
	FClockworkTicketData() {}

	FClockworkTicketData(uint8 InMaxTickets) :
		MaxTickets(InMaxTickets)
	{}


	//-------------------------
	// API
	//-------------------------

public:
	// --------------- Tickets --------------- //
	
	int16 GetRemainingTickets()
	{
		return CurrentTickets;
	}

	int16 GetMaxTickets()
	{
		return MaxTickets;
	}

	void AddTickets(int16 TicketsToAdd = 1)
	{
		CurrentTickets = FMath::Min(CurrentTickets + TicketsToAdd, MaxTickets);
	}
};

/*
* Unlimited Time & Tickets/Enemies. Limited Number of Lives Only.
* Defaults to 5 Lives
*/
USTRUCT(BlueprintType)
struct FClockworkLivesData
{
	GENERATED_BODY()

	//-------------------------
	// Member Variables
	//-------------------------

private:
	uint8 MaxLives{ 5 };
	uint8 CurrentLives{ 5 };


	//-------------------------
	// Constructors
	//-------------------------

public:
	FClockworkLivesData() {}

	FClockworkLivesData(uint8 Lives) :
		MaxLives(Lives),
		CurrentLives(Lives)
	{}


	//-------------------------
	// API
	//-------------------------

public:
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "/Script/ClockworkTactics.EGameType"))
	uint8 GameType { 7 };

	UPROPERTY(BlueprintReadOnly)
	FClockworkTicketData TicketData;

	UPROPERTY(BlueprintReadOnly)
	FClockworkTimeData TimeData;

	UPROPERTY(BlueprintReadOnly)
	FClockworkLivesData LifeData;

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

	void NotifyEntityDestruction(const FClockworkEntityDeathData& DeathData);
};
