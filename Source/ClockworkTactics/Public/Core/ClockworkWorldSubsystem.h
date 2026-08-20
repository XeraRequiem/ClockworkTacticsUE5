#pragma once

// Engine
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Subsystems/WorldSubsystem.h"

// Game
#include "Grid/ClockworkGrid.h"

// Generated
#include "ClockworkWorldSubsystem.generated.h"


UCLASS(Config=Game)
class UClockworkWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()


	//-------------------------
	// Member Variables
	//-------------------------

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AClockworkGrid> HexGrid;

	UPROPERTY()
	bool bGridGenerated{ false };


	//-------------------------
	// Constructor
	//-------------------------

public:
	UClockworkWorldSubsystem();


	//-------------------------
	// Inherited
	//-------------------------

public:
	virtual void Deinitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;


	//-------------------------
	// Const API
	//-------------------------

public:
	AClockworkGrid* GetHexGrid() const;


	//-------------------------
	// API
	//-------------------------

public:
	void GenerateHexGrid();
	void GenerateHexGrid(const FString& GridJsonData);
};



