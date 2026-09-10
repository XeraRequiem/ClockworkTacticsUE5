// Required
#include "Projectile/ClockworkProjectileBase.h"

// Game
#include "Core/ClockworkTactics.h"


// -------------------------
// --- Constructors
// -------------------------

AClockworkProjectileBase::AClockworkProjectileBase() :
	 Super()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	// Create Static Mesh Component
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	// Create Capsule Collision Component
	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CollisionCapsule->InitCapsuleSize(42.f, 96.0f);
	CollisionCapsule->SetCollisionProfileName(TEXT("Projectile"));
	CollisionCapsule->SetupAttachment(ProjectileMesh);

	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AClockworkProjectileBase::OnCollisionBeginOverlap);
}


// -------------------------
// --- Inherited
// -------------------------

void AClockworkProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TravelTime += DeltaTime;

	// If the target entity is no longer valid, destroy the projectile
	if (TargetEntity == nullptr)
	{
		DestroyProjectile(EClockworkProjectDestroyReason::InvalidTarget);
		return;
	}
	
	// If the projectile has exceeded its maximum travel time plus a buffer, destroy it
	if (TravelTime > MaxTravelTime + TravelTimeBuffer)
	{
		DestroyProjectile(EClockworkProjectDestroyReason::Timeout);
		return;
	}

	// Interpolate the projectile's location between its current location and the target entity's location based on the travel time
	// Temp: Each Entity Should have a GetProjectileTargetLocation() function that returns the location the projectile should travel to (e.g., the center of the entity, or a specific socket location)

	FVector SourceOrigin;
	FVector SourceExtents;
	SourceEntity->GetActorBounds(true, SourceOrigin, SourceExtents);
	FVector SourceLocation = SourceOrigin + FVector(0.0, 0.0, SourceExtents.Z / 2.0f);

	UE_LOG(LogProjectile, Verbose, TEXT("(Source) Origin: %s, Extents: %s"), *SourceOrigin.ToString(), *SourceExtents.ToString());

	FVector TargetOrigin;
	FVector TargetExtents;
	TargetEntity->GetActorBounds(true, TargetOrigin, TargetExtents);
	FVector TargetLocation = TargetOrigin + FVector(0.0, 0.0, TargetExtents.Z / 2.0f);

	UE_LOG(LogProjectile, Verbose, TEXT("(Target) Origin: %s, Extents: %s"), *TargetOrigin.ToString(), *TargetExtents.ToString());

	FVector InterpolatedLocation = FMath::Lerp<FVector>(SourceLocation, TargetLocation, TravelTime / MaxTravelTime);
	SetActorLocation(InterpolatedLocation);

	// Rotate the projectile to face the target entity
	FVector ForwardVector = TargetEntity->GetActorLocation() - GetActorLocation();
	SetActorRotation(FRotator(0.0, ForwardVector.Rotation().Yaw, 0.0));
}


// -------------------------
// --- Implementation
// -------------------------

void AClockworkProjectileBase::DestroyProjectile(EClockworkProjectDestroyReason Reason)
{
	FString ReasonString;
	UEnum::GetValueAsString(Reason).Split(TEXT("::"), nullptr, &ReasonString);

	UE_LOG(LogProjectile, Verbose, TEXT("Destroying Projectile: %s From %s"), *GetName(), *ReasonString);
	Destroy();
}


void AClockworkProjectileBase::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogProjectile, Verbose, TEXT("Oh no, I hit %s"), *OtherActor->GetName());
	if (OtherActor->IsA<AClockworkHexEntity>())
	{
		// To-Do: Alert Other Actor To Collision
		DestroyProjectile(EClockworkProjectDestroyReason::HitTarget);
	}
}
