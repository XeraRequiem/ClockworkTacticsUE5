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

	if (EntitiesWithinRange.Num() > 0)
	{
		// Rotate Character to Face Primary Target
		FVector ForwardDirection = EntitiesWithinRange[0]->GetActorLocation() - GetActorLocation();
		SetActorRotation(FRotator(0, ForwardDirection.Rotation().Yaw, 0));
	}
}


// -------------------------
// --- Implementation
// -------------------------

void AClockworkHexCharacter::UpdateTargetEntitiesInRange()
{
	EntitiesWithinRange.Empty();
	EntitiesWithinRange = GetTargetableEntitiesWithinRange(UnitData.AttackRange);
	EntitiesWithinRange.Sort([](const AClockworkHexEntity& A, const AClockworkHexEntity& B)
	{
		// A = unit, B =/= unit
		if (A.IsA<AClockworkHexUnit>() && !B.IsA<AClockworkHexUnit>())
		{
			return true;
		}

		// A =/= unit, B = unit
		if (!A.IsA<AClockworkHexUnit>() && B.IsA<AClockworkHexUnit>())
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

TArray<FOffsetCoordinate> AClockworkHexCharacter::GetCoordinatesInRange()
{
	FCubeCoordinate CoordinateCube = UHexMath::ConvertOffsetCoordinateToCube(OccupiedHex->GetGridCoordinate());

	TArray<FOffsetCoordinate> CoordinatesInRange;
	for (int32 q = CoordinateCube.Q - UnitData.AttackRange; q <= CoordinateCube.Q + UnitData.AttackRange; ++q)
	{
		for (int32 r = CoordinateCube.R - UnitData.AttackRange; r <= CoordinateCube.R + UnitData.AttackRange; ++r)
		{
			int32 s = -q - r;
			if (FMath::Abs(s - CoordinateCube.S) <= UnitData.AttackRange)
			{
				// Skip the center hex (the character's current position)
				if (CoordinateCube.Q == q && CoordinateCube.R == r && CoordinateCube.S == s)
				{
					continue;
				}
				
				CoordinatesInRange.Add(UHexMath::ConvertCubeCoordinateToOffset(FCubeCoordinate(r, s, q)));
			}
		}
	}

	return CoordinatesInRange;
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
