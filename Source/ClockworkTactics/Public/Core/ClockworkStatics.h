#pragma once

// Engine
#include "CoreMinimal.h"

// Generated
#include "ClockworkStatics.generated.h"


//-------------------------
// Structs
//-------------------------

/*
*
*/
USTRUCT(BlueprintType)
struct FClockworkEntityDeathData
{
	GENERATED_BODY()


	//-------------------------
	// Member Variables
	//-------------------------

public:
	UPROPERTY(BlueprintReadOnly)
	FString DeadEntity;

	UPROPERTY(BlueprintReadOnly)
	FString KillerEntity;


	//-------------------------
	// Constructors
	//-------------------------

public:
	FClockworkEntityDeathData() :
		DeadEntity(TEXT("Invalid Entity")),
		KillerEntity(TEXT("Invalid Entity"))
	{}

	FClockworkEntityDeathData(const FString& InDeadEntity, const FString& InKillerEntity) :
		DeadEntity(InDeadEntity),
		KillerEntity(InKillerEntity)
	{}
};


//-------------------------
// Class
//-------------------------
	
UCLASS(config=Game)
class UClockworkStatics : public UObject
{
	GENERATED_BODY()


};

