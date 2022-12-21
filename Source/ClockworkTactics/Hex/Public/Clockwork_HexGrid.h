#pragma once

// Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

// Game
#include "Clockwork_HexTile.h"

#include "Clockwork_HexGrid.generated.h"

UCLASS()
class AClockwork_HexGrid : public AActor
{
	GENERATED_BODY()


	//-------------------------
	// Member Variables
	//-------------------------

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	uint8 TileWidth;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	uint8 TileLength;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	uint8 TileGap;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AClockwork_HexTile> TileClass;

	UPROPERTY(Replicated, BlueprintReadOnly)
	TArray<AClockwork_HexTile*> Tiles;


	//-------------------------
	// Constructors
	//-------------------------

public:
	AClockwork_HexGrid();


	//-------------------------
	// Inherited
	//-------------------------

public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void BeginPlay() override;


	//-------------------------
	// Implementation
	//-------------------------

protected:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void GenerateGrid();
};

