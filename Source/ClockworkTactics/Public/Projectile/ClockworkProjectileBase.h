#pragma once

// Engine
#include "Components/CapsuleComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Plugin
#include "Core/HexMath.h"

// Game
#include "Entity/ClockworkHexEntity.h"

// Generated
#include "ClockworkProjectileBase.generated.h"


// -------------------------
// --- Enum
// -------------------------

UENUM(BlueprintType)	
enum class EClockworkProjectDestroyReason : uint8
{
	None,
	InvalidTarget,
	HitTarget,
	Timeout
};


// -------------------------
// --- Classes
// -------------------------

UCLASS(BlueprintType)
class AClockworkProjectileBase : public AActor
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

protected:
	// -------------------- Components -------------------- //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UCapsuleComponent> CollisionCapsule;

	// -------------------- Targetting -------------------- //

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	TObjectPtr<AClockworkHexEntity> SourceEntity;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	TObjectPtr<AClockworkHexEntity> TargetEntity;

	// -------------------- Travel Time -------------------- //

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxTravelTime{ 1.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TravelTimeBuffer{ 0.5f };

	UPROPERTY(BlueprintReadOnly)
	float TravelTime{ 0.0f };


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	AClockworkProjectileBase();


	// -------------------------
	// --- Inherited
	// -------------------------

public:
	void Tick(float DeltaTime) override;


	// -------------------------
	// --- Implementation
	// -------------------------

protected:
	void DestroyProjectile(EClockworkProjectDestroyReason Reason);

	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
