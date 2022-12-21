#pragma once

// Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

#include "Clockwork_HexTile.generated.h"

UCLASS()
class AClockwork_HexTile : public AActor
{
	GENERATED_BODY()


	//-------------------------
	// Static Variables
	//-------------------------
	
public:
	static uint8 DiameterInner;
	static uint8 DiameterOuter;

	//-------------------------
	// Member Variables
	//-------------------------

protected:
	UPROPERTY(BlueprintReadOnly, Replicated)
	float TimeSinceColorChange;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float TimeBetweenColorChange;


	//-------------------------
	// Constructors
	//-------------------------

public:
	AClockwork_HexTile();


	//-------------------------
	// Inherited
	//-------------------------

public:
	void Tick(float delta) override;

	void GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const override;


	//-------------------------
	// Implementation
	//-------------------------

protected:
	UFUNCTION()
	void ChangeColor();

	UFUNCTION(NetMulticast, Reliable)
	void SetColor(FLinearColor inColor);


	//-------------------------
	// Blueprint Events
	//-------------------------

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnColorSet(FLinearColor inColor);
};

