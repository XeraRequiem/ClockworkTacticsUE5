#pragma once

// Engine
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

// Generated
#include "HexTile.generated.h"

class AHexGrid;


// -------------------------
// --- Enums
// -------------------------

UENUM(BlueprintType, Blueprintable)
enum class EOccupationStatus : uint8
{
	Vacant,
	Reserved,
	Occupied
};


// -------------------------
// --- Classes
// -------------------------

/*
* Horizonal Row Oriented Hexagonal Tile
*/
UCLASS()
class AHexTile : public AActor
{
	 GENERATED_BODY()


	 // -------------------------
	 // --- Member Variables
	 // -------------------------

protected:
	 UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated)
	 UStaticMeshComponent* Mesh;

	 UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Replicated)
	 USceneComponent* OccupationLocation;

	 UPROPERTY(BLueprintReadOnly, ReplicatedUsing=RepNotify_Height)
	 float Height;

	 UPROPERTY(BlueprintReadOnly, ReplicatedUsing=RepNotify_OccupationStatus)
	 EOccupationStatus OccupationStatus;
		  
	 UPROPERTY(BlueprintReadOnly, Replicated)
	 AClockworkUnit* Occupant;

	 UPROPERTY(BlueprintReadOnly, Replicated)
	 AHexGrid* OwningGrid;

	 UPROPERTY(EditDefaultsOnly)
	 FLinearColor OccupiedColor;

	 UPROPERTY(EditDefaultsOnly)
	 FLinearColor ReservedColor;

	 UPROPERTY(EditDefaultsOnly)
	 FLinearColor VacantColor;

private:
	 FVector2D Coordinate;

	 bool bDebugMode;


	 // -------------------------
	 // --- Constructors
	 // -------------------------

public:
	 AHexTile();


	 // -------------------------
	 // --- Inherited
	 // -------------------------

public:
	 void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


	 // -------------------------
	 // --- API
	 // -------------------------

public:
	 UFUNCTION(Server, Reliable)
	 void InitializeTile(AHexGrid* grid, FVector location, float inHeight, bool bDebug = false);

	 UFUNCTION(Server, Reliable)
	 void SetHeight(float inHeight);


	 UFUNCTION(Server, Reliable)
	 void Reserve(AClockworkUnit* clockwork);

	 UFUNCTION(Server, Reliable)
	 void Occupy(AClockworkUnit* clockwork);

	 UFUNCTION(Server, Reliable)
	 void Vacate(AClockworkUnit* clockwork);


	 // -------------------------
	 // --- Const API
	 // -------------------------

public:
	 UFUNCTION(BlueprintPure)
	 EOccupationStatus GetOccupationStatus() const;


	 UFUNCTION(BlueprintPure)
	 AClockworkUnit* GetOccupant() const;

	 UFUNCTION(BlueprintPure)
	 FVector GetOccupationLocation() const;


	 UFUNCTION(BlueprintPure)
	 bool IsOccupied() const;

	 UFUNCTION(BlueprintPure)
	 bool IsReserved() const;

	 UFUNCTION(BlueprintPure)
	 bool IsVacant() const;


	 UFUNCTION()
	 bool CanBeReservedBy(const AClockworkUnit* clockwork) const;

	 UFUNCTION()
	 bool CanBeOccupiedBy(const AClockworkUnit* clockwork) const;

	 UFUNCTION()
	 bool CanBeVacatedBy(const AClockworkUnit* clockwork) const;


	 UFUNCTION(BlueprintPure)
	 AHexGrid* GetOwningGrid() const;

	 UFUNCTION(BlueprintPure)
	 FVector2D GetGridCoordinate() const;


	 UFUNCTION(BlueprintPure)
	 float GetCircumradius() const;

	 UFUNCTION(BlueprintPure)
	 float GetInradius() const;

	 UFUNCTION(BlueprintPure)
	 float GetMaximalDiameter() const;

	 UFUNCTION(BlueprintPure)
	 float GetMinimalDiameter() const;


	 // -------------------------
	 // --- Implementation
	 // -------------------------

protected:
	 UFUNCTION()
	 void UpdateMeshHeight();

	 UFUNCTION()
	 void SetColorByOccupationStatus();


	 // -------------------------
	 // --- Variable Replication
	 // -------------------------

protected:
	 UFUNCTION()
	 void RepNotify_OccupationStatus();

	 UFUNCTION()
	 void RepNotify_Height();


	 // -------------------------
	 // --- Blueprint Events
	 // -------------------------

protected:
	 UFUNCTION(BlueprintImplementableEvent)
	 void OnSetColor(FLinearColor Color);

	 // -------------------------
	 // --- Debugging
	 // -------------------------

public:
	 UFUNCTION()
	 void Debug_SetText(const FString& text);

protected:
	 UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	 void Debug_OnTextSet(const FString& text);
};
