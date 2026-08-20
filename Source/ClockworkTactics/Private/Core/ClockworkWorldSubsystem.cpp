// Required
#include "Core/ClockworkWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"

// Game
#include "Core/ClockworkGameMode.h"
#include "Core/ClockworkTactics.h"


//-------------------------
// Constructor
//-------------------------

UClockworkWorldSubsystem::UClockworkWorldSubsystem() {}


//-------------------------
// Inherited
//-------------------------

void UClockworkWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();

	HexGrid = nullptr;
}

void UClockworkWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	GenerateHexGrid();
}


//-------------------------
// Const API
//-------------------------

AClockworkGrid* UClockworkWorldSubsystem::GetHexGrid() const
{
	return HexGrid;
}


//-------------------------
// API
//-------------------------

void UClockworkWorldSubsystem::GenerateHexGrid()
{
	if (bGridGenerated) return;

	if (AClockworkGameMode* GameMode = Cast<AClockworkGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (TSubclassOf<AClockworkGrid> GridClass = GameMode->GridClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FTransform GridSpawnTransform = (GameMode->GridSpawnPoint != nullptr) ? GameMode->GridSpawnPoint->GetTransform() : FTransform();

			HexGrid = GetWorld()->SpawnActor<AClockworkGrid>(GridClass, GridSpawnTransform, SpawnParams);
			
			if (GameMode->bGenerateGridFromLayoutFile && !GameMode->GridLayoutFile.IsEmpty())
			{
				HexGrid->InitializeWithLayout(GameMode->GridLayoutFile);
			}
			else
			{
				HexGrid->Initialize(GameMode->GridWidth, GameMode->GridDepth, GameMode->bGridVariableHeight);
				HexGrid->GenerateGrid();
			}

			bGridGenerated = true;
		}
		else
		{
			UE_LOG(LogClockwork, Warning, TEXT("Failed to get grid class"));
		}
	}
	else
	{
		UE_LOG(LogClockwork, Warning, TEXT("Failed to get GameMode"));
	}
}

void UClockworkWorldSubsystem::GenerateHexGrid(const FString& GridJsonData)
{
	if (bGridGenerated) return;

}
