// Engine
#include "Tests/TestHarnessAdapter.h"

// Game
#include "Projectile/ClockworkProjectileBase.h"


// -------------------------
// --- Test Class
// -------------------------

namespace
{
	class AClockworkProjectileUnitTest : public AClockworkProjectileBase
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

TEST_CASE_NAMED(FClockworkProjectileBaseTest, "Clockwork::Unit::Projectile", "[Smoke]")
{
	SECTION("Constructors")
	{
		AClockworkProjectileUnitTest* Projectile = NewObject<AClockworkProjectileUnitTest>();

		float InitialMaxTravelTime{ 1.0f };
		float InitialTravelTimeBuffer{ 0.5f };
		float InitialTravelTime{ 0.0f };

		CHECK(Projectile->GetMaxTravelTime() == InitialMaxTravelTime);
		CHECK(Projectile->GetTravelTimeBuffer() == InitialTravelTimeBuffer);
		CHECK(Projectile->GetTravelTime() == InitialTravelTime);

	}

	SECTION("Setters")
	{
		AClockworkProjectileUnitTest* Projectile = NewObject<AClockworkProjectileUnitTest>();

		float MaxTravelTime{ 17.1f };
		float TravelTimeBuffer{ 2.5f };
		float TravelTime{ 15.1f };

		Projectile->SetMaxTravelTime(MaxTravelTime);
		Projectile->SetTravelTimeBuffer(TravelTimeBuffer);
		Projectile->SetTravelTime(TravelTime);

		CHECK(Projectile->GetMaxTravelTime() == MaxTravelTime);
		CHECK(Projectile->GetTravelTimeBuffer() == TravelTimeBuffer);
		CHECK(Projectile->GetTravelTime() == TravelTime);
	}

}