// Required
#include "Entity/ClockworkHexCharacter.h"

// Engine
#include "Kismet/GameplayStatics.h"

// Plugin
#include "Core/HexMath.h"

// Game
#include "Core/ClockworkGameMode.h"
#include "Core/ClockworkTactics.h"
#include "Core/ClockworkWorldSubsystem.h"
#include "Grid/ClockworkHex.h"


// -------------------------
// --- Constructors
// -------------------------

AClockworkHexCharacter::AClockworkHexCharacter() :
	 Super()
{
	 FriendlyName = TEXT("Clockwork Hex Character");

	 UnitData = FClockworkHexUnitData{ .AttackRange = 1 };
}


// -------------------------
// --- Inherited
// -------------------------

void AClockworkHexCharacter::Tick(float dt)
{
	UpdateTargetEntitiesInRange();

	if (TargetEntities.Num() > 0)
	{
		FVector ForwardDirection = TargetEntities[0]->GetActorLocation() - GetActorLocation();
		FRotator ForwardRotation = FRotator(0, ForwardDirection.Rotation().Yaw, 0);
		SetActorRotation(ForwardRotation);
	}

	TimeSinceLastLog += dt;
	if (TimeSinceLastLog >= 2.0f)
	{
		UE_LOG(LogClockwork, Verbose, TEXT("Current Targets:"));
		if (TargetEntities.Num() == 0)
		{
			UE_LOG(LogClockwork, Verbose, TEXT("\tNone"));
		}

		for (AClockworkHexEntity* Target : TargetEntities)
		{
			UE_LOG(LogClockwork, Verbose, TEXT("\t%s"), *Target->GetFriendlyName());
		}

		TimeSinceLastLog = 0.0f;
	}
}


// -------------------------
// --- Implementation
// -------------------------

TArray<FOffsetCoordinate> AClockworkHexCharacter::GetCoordinatesInRange()
{
	FOffsetCoordinate Cooordinate = OccupiedHex->GetGridCoordinate();
	FCubeCoordinate CoordinateCube = UHexMath::ConvertOffsetCoordinateToCube(Cooordinate);

	TArray<FOffsetCoordinate> CoordinatesInRange;
	for (int32 q = CoordinateCube.Q - UnitData.AttackRange; q <= CoordinateCube.Q + UnitData.AttackRange; ++q)
	{
		for (int32 r = CoordinateCube.R - UnitData.AttackRange; r <= CoordinateCube.R + UnitData.AttackRange; ++r)
		{
			int32 s = -q - r;
			if (FMath::Abs(s - CoordinateCube.S) <= UnitData.AttackRange)
			{
				FCubeCoordinate TargetCubeCoordinate(r, s, q);
				if (TargetCubeCoordinate.Equals(CoordinateCube))
				{
					continue;
				}

				CoordinatesInRange.Add(UHexMath::ConvertCubeCoordinateToOffset(TargetCubeCoordinate));
			}
		}
	}

	return CoordinatesInRange;
}


void AClockworkHexCharacter::UpdateTargetEntitiesInRange()
{
	TArray<AClockworkHexEntity*> EntitiesInRange = GetTargetableEntitiesWithinRange(UnitData.AttackRange);
	EntitiesInRange.Sort([](const AClockworkHexEntity& A, const AClockworkHexEntity& B)
	{
		// A = unit, B =/= unit
		if (A.IsA<AClockworkHexUnit>() && !B.IsA<AClockworkHexUnit>())
		{
			return true;
		}

		// A =/= unit, B = unit
		if (!A.IsA<AClockworkHexUnit>() && !B.IsA<AClockworkHexUnit>())
		{
			return false;
		}

		// A =/= unit, B =/= unit
		// To-Do: Sort by distance from Character if neither are units
		if (!A.IsA<AClockworkHexUnit>() && !B.IsA<AClockworkHexUnit>())
		{
			return true;	
		}

		// A = unit, B = unit
		const AClockworkHexUnit* UnitA = Cast<AClockworkHexUnit>(&A);
		const AClockworkHexUnit* UnitB = Cast<AClockworkHexUnit>(&B);

		return UnitA->GetDistanceToTargetHex() < UnitB->GetDistanceToTargetHex();
	});
	
	TargetEntities.Empty();
	for (int i = 0; i < MaxTargets && i < EntitiesInRange.Num(); ++i)
	{
		TargetEntities.Add(EntitiesInRange[i]);
	}
}

TArray<AClockworkHexEntity*> AClockworkHexCharacter::GetTargetableEntitiesWithinRange(uint8 Range)
{
	// Get All Coordinates Within Attack Range
	TArray<FOffsetCoordinate> CoordinatesInRange = GetCoordinatesInRange();

	// Get All Hexes Within Attack Range
	TArray<AClockworkHex*> HexesInRange;
	for (FOffsetCoordinate Coordinate : CoordinatesInRange)
	{
		AClockworkGrid* Grid = GetWorld()->GetSubsystem<UClockworkWorldSubsystem>()->GetHexGrid();
		if (Grid != nullptr)
		{
			AClockworkHex* Hex = Grid->GetHexAtCoordinate(Coordinate);
			if (Hex != nullptr)
			{
				HexesInRange.Add(Hex);
			}
		}
	}

	// Debug: Paint Hexes In Range
	if (AClockworkGameMode* GameMode = Cast<AClockworkGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (GameMode->bDebugMode)
		{
			Debug_PaintHexesInRange(HexesInRange);
		}
	}

	// Get All Entities Within Attack Range
	TArray<AClockworkHexEntity*> EntitiesInRange;
	for (AClockworkHex* Hex : HexesInRange)
	{
		if (Hex->IsOccupied())
		{
			EntitiesInRange.Add(Hex->GetOccupant());
		}
	}

	return EntitiesInRange;
}


// -------------------------
// --- Debug
// -------------------------

void AClockworkHexCharacter::Debug_PaintHexesInRange(TArray<AClockworkHex*> Hexes)
{
	for (AClockworkHex* Hex : Hexes)
	{
		if (Hex != nullptr)
		{
			Hex->Debug_SetColor(FColor::Cyan);
		}
	}
}
