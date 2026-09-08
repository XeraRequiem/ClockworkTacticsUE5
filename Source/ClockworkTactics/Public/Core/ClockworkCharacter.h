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

	 /** Maximum distance camera can be from pawn */
	 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	 float MaxCameraDistance{ 250.0f };

	 /** Minimum distance camera can be from pawn */
	 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	 float MinCameraDistance{ 50.0f };

	 /** Rate at which to zoom in/out */
	 UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	 float ZoomInRate{ 5.0f };

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
	 * Called for camera zoom in input
	 * @param Rate	Scaled distance to move in/out
	 */
	void ZoomIn();

	/**
	 * Called for camera zoom out input
	 * @param Rate	Scaled distance to move in/out
	 */

	void ZoomOut();

	/**
	 * Called for camera zoom out input
	 * @param Rate	Scaled distance to move in/out
	 */
	void Zoom(float Value);
};

