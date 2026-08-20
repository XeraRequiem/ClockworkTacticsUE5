#pragma once

// Engine
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Generated
#include "ClockworkCharacter.generated.h"


//-------------------------
// Class
//-------------------------

UCLASS(Config=Game)
class AClockworkCharacter : public ACharacter
{
	 GENERATED_BODY()

	 //-------------------------
	 // Member Variables
	 //-------------------------

public:
	 /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	 float TurnRateGamepad;

private:
	 /** Camera boom positioning the camera behind the character */
	 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	 class USpringArmComponent* CameraBoom;

	 /** Follow camera */
	 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	 class UCameraComponent* FollowCamera;


	 //-------------------------
	 // Constructor
	 //-------------------------

public:
	AClockworkCharacter();


	 //-------------------------
	 // Inherited - APawn
	 //-------------------------

protected:
	 virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	 //-------------------------
	 // Implementation
	 //-------------------------

protected:
	/**
	 * Called for forwards/backward input
	 * @param Value Scaled distance to move forward/backwards
	 */

	void MoveForward(float Value);

	/**
	 * Called for side to side input
	 * @param Rate	Scaled distance to move left/right
	 */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
};

