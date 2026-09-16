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
	// ---------- Components --------------- //

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UCapsuleComponent> CollisionCapsule;

	// ---------- Targetting --------------- //

	UPROPERTY(BlueprintReadOnly)
	FVector StartLocation;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	TObjectPtr<AClockworkHexEntity> TargetEntity;

	// ---------- Travel Time --------------- //

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxTravelTime{ 1.0f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TravelTimeBuffer{ 0.5f };

	UPROPERTY(BlueprintReadOnly)
	float TravelTime{ 0.0f };

	// ---------- Damage --------------- //

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	FClockworkDamage Damage;


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


	// -------------------------
	// --- Event Handlers
	// -------------------------

protected:
	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
