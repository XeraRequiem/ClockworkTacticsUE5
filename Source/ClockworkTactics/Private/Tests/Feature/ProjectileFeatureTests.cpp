// Engine
#include "Tests/TestHarnessAdapter.h"

// Game
#include "Projectile/ClockworkProjectileBase.h"


// -------------------------
// --- Test Class
// -------------------------

namespace {

	class AClockworkProjectileFeatureTest : public AClockworkProjectileBase
	{
public:
		EClockworkProjectileDestroyReason DestroyReason{ EClockworkProjectileDestroyReason::None };

public:
		void SetMaxTravelTime(float Time)
		{
			MaxTravelTime = Time;
		}

		void SetTravelTimeBuffer(float Time)
		{
			TravelTimeBuffer = Time;
		}

		void SetTravelTime(float Time)
		{
			TravelTime = Time;
		}

		void SetTarget(AClockworkHexEntity* Target)
		{
			TargetEntity = Target;
		}


		float GetMaxTravelTime()
		{
			return MaxTravelTime;
		}

		float GetTravelTimeBuffer()
		{
			return TravelTimeBuffer;
		}

		float GetTravelTime()
		{
			return TravelTime;
		}

		AClockworkHexEntity* GetTarget()
		{
			return TargetEntity;
		}


		void DestroyProjectile(EClockworkProjectileDestroyReason Reason) override
		{
			DestroyReason = Reason;
		}
	};
}


// -------------------------
// --- Tests
// -------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FClockworkProjectileTestTickNoTarget,	"Clockwork.Feature.TickNoTarget",	EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FClockworkProjectileTestTickTimeout,	"Clockwork.Feature.TickTimeout",		EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FClockworkProjectileTestTick,				"Clockwork.Feature.Tick",				EAutomationTestFlags::ClientContext | EAutomationTestFlags::ProductFilter);

/*
* Check Projectile is Destroyed if Target Entity is Invalid
*/
bool FClockworkProjectileTestTickNoTarget::RunTest(const FString& Parameters)
{
	AClockworkProjectileFeatureTest* Projectile = NewObject<AClockworkProjectileFeatureTest>();
	
	float TickDT{ 0.33f };
	Projectile->Tick(TickDT);

	if (Projectile->GetTravelTime() != TickDT)
	{
		return false;
	}

	if (Projectile->DestroyReason != EClockworkProjectileDestroyReason::InvalidTarget)
	{
		return false;
	}

	// Clean-up
	Projectile->Destroy();

	return true;
}

/*
* Check Projectile is Destroyed if it has been travelling too long
*/
bool FClockworkProjectileTestTickTimeout::RunTest(const FString& Parameters)
{
	AClockworkProjectileFeatureTest* Projectile = NewObject<AClockworkProjectileFeatureTest>();

	AClockworkHexEntity* Entity = NewObject<AClockworkHexEntity>();
	Projectile->SetTarget(Entity);

	if (Projectile->GetTarget() == nullptr)
	{
		return false;
	}
		
	float MaxTravelTime{ 0.25f };
	Projectile->SetMaxTravelTime(MaxTravelTime);

	if (Projectile->GetMaxTravelTime() != MaxTravelTime)
	{
		return false;
	}

	float TickDT{ 0.33f };
	Projectile->Tick(TickDT);

	if (Projectile->GetTravelTime() != TickDT)
	{
		return false;
	}

	if (Projectile->DestroyReason != EClockworkProjectileDestroyReason::Timeout)
	{
		return false;
	}

	// Clean-up
	Projectile->Destroy();
	Entity->Destroy();

	return true;
}

/*
* Check Projectile transform is updated with valid target & within travel time
*/
bool FClockworkProjectileTestTick::RunTest(const FString& Parameters)
{
	// Initialize Target Entity
	FVector EntityLocation = FVector(2.0, 2.0, 2.0);
	
	AClockworkHexEntity* Entity = NewObject<AClockworkHexEntity>();
	Entity->SetActorLocation(EntityLocation);
	if (Entity->GetActorLocation() != EntityLocation)
	{
		return false;
	}

	// Initialize Projectile
	AClockworkProjectileFeatureTest* Projectile = NewObject<AClockworkProjectileFeatureTest>();
	Projectile->SetTarget(Entity);

	if (Projectile->GetTarget() == nullptr)
	{
		return false;
	}

	float MaxTravelTime{ 1.0f };
	Projectile->SetMaxTravelTime(MaxTravelTime);

	if (Projectile->GetMaxTravelTime() != MaxTravelTime)
	{
		return false;
	}

	// Tick Projectile
	float TickDT{ 0.5f };
	Projectile->Tick(TickDT);

	if (Projectile->GetTravelTime() != TickDT)
	{
		return false;
	}

	if (Projectile->DestroyReason != EClockworkProjectileDestroyReason::None)
	{
		return false;
	}

	// Projectile Moved To Correct Location
	FVector ExpectedProjectileLocation = EntityLocation * (TickDT / MaxTravelTime);
	if (!Projectile->GetActorLocation().Equals(ExpectedProjectileLocation))
	{
		return false;
	}

	// Projectile Faces Target Location
	FRotator ExpectedProjectileRotation = FRotator(0.0, (EntityLocation - ExpectedProjectileLocation).Rotation().Yaw, 0.0);
	if (!Projectile->GetActorRotation().Equals(ExpectedProjectileRotation))
	{
		return false;
	}

	// Clean-up
	Projectile->Destroy();
	Entity->Destroy();

	return true;
}