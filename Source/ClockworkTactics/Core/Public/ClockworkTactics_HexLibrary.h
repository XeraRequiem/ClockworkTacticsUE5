#pragma once

// Engine
#include "CoreMinimal.h"

// Generated
#include "ClockworkTactics_HexLibrary.generated.h"

// -------------------------
// Structs
// -------------------------

USTRUCT(BlueprintType)
struct FOffsetCoordinate
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

public:
	UPROPERTY(BlueprintReadWrite)
	uint8 X;

	UPROPERTY(BlueprintReadWrite)
	uint8 Y;


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	FOffsetCoordinate() :
		X(0),
		Y(0)
	{
	}

	FOffsetCoordinate(uint8 inX, uint8 inY) :
		X(inX),
		Y(inY)
	{
	}


	// -------------------------
	// --- API
	// -------------------------

	 public:
		  bool IsWithinBounds(uint8 xMax, uint8 yMax)
		  {
				return X >= 0 && X <= xMax && Y >= 0 && Y <= yMax;
		  }
	 
};

USTRUCT(BlueprintType)
struct FCubeCoordinate
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

public:
	UPROPERTY(BlueprintReadWrite)
	uint8 R;

	UPROPERTY(BlueprintReadWrite)
	uint8 S;

	UPROPERTY(BlueprintReadWrite)
	uint8 Q;


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	FCubeCoordinate() :
		R(0),
		S(0),
		Q(0)
	{
	}

	FCubeCoordinate(uint8 inR, uint8 inS, uint8 inQ) :
		R(inR),
		S(inS),
		Q(inQ)
	{
	}


	// -------------------------
	// --- API
	// -------------------------

public:
	FCubeCoordinate operator-(FCubeCoordinate other)
	{
		return FCubeCoordinate(R - other.R, S - other.S, Q - other.Q);
	}
};

USTRUCT(BlueprintType)
struct FAxialCoordinate
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

public:
	UPROPERTY(BlueprintReadWrite)
	uint8 R;

	UPROPERTY(BlueprintReadWrite)
	uint8 Q;


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	FAxialCoordinate() :
		R(0),
		Q(0)
	{
	}

	FAxialCoordinate(uint8 inR, uint8 inQ) :
		R(inR),
		Q(inQ)
	{
	}


	// -------------------------
	// --- API
	// -------------------------

public:
	FAxialCoordinate operator-(FAxialCoordinate other)
	{
		return FAxialCoordinate(R - other.R, Q - other.Q);
	}
};

USTRUCT(BlueprintType)
struct FDoubledCoordinate
{
	GENERATED_BODY()


	// -------------------------
	// --- Member Variables
	// -------------------------

public:
	UPROPERTY(BlueprintReadWrite)
	uint8 X;

	UPROPERTY(BlueprintReadWrite)
	uint8 Y;


	// -------------------------
	// --- Constructors
	// -------------------------

public:
	FDoubledCoordinate() :
		X(0),
		Y(0)
	{
	}

	FDoubledCoordinate(uint8 inX, uint8 inY) :
		X(inX),
		Y(inY)
	{
	}


	// -------------------------
	// --- API
	// -------------------------

public:
	FDoubledCoordinate operator-(FDoubledCoordinate other)
	{
		return FDoubledCoordinate(X - other.X, Y - other.Y);
	}
};


// -------------------------
// Classes
// -------------------------

UCLASS()
class CLOCKWORKTACTICS_API UClockworkTactics_HexLibrary : public UObject
{
	GENERATED_BODY()

public:
	// Neighbors
	UFUNCTION(BlueprintCallable)
	static TArray<FOffsetCoordinate> GetNeighborsOfOffsetCoordinate(FOffsetCoordinate coordinate, uint8 xMax, uint8 yMax);

	UFUNCTION(BlueprintCallable)
	static TArray<FDoubledCoordinate> GetNeighborsOfDoubledCoordinate(FDoubledCoordinate coordinate);
	
	UFUNCTION(BlueprintCallable)
	static TArray<FCubeCoordinate> GetNeighborsOfCubeCoordinate(FCubeCoordinate coordinate);
	
	UFUNCTION(BlueprintCallable)
	static TArray<FAxialCoordinate> GetNeighborsOfAxialCoordinate(FAxialCoordinate coordinate);

	// Distance
	UFUNCTION(BlueprintCallable)
	uint8 GetDistanceBetweenOffsetCoordinates(FOffsetCoordinate start, FOffsetCoordinate end);

	UFUNCTION(BlueprintCallable)
	uint8 GetDistanceBetweenDoubledCoordinates(FDoubledCoordinate start, FDoubledCoordinate end);

	UFUNCTION(BlueprintCallable)
	uint8 GetDistanceBetweenCubeCoordinates(FCubeCoordinate start, FCubeCoordinate end);

	UFUNCTION(BlueprintCallable)
	uint8 GetDistanceBetweenAxialCoordinates(FAxialCoordinate start, FAxialCoordinate end);

	// Conversion
	UFUNCTION(BlueprintCallable)
	static FCubeCoordinate ConvertOffsetCoordinateToCube(FOffsetCoordinate coordinate);

	UFUNCTION(BlueprintCallable)
	static FAxialCoordinate ConvertOffsetCoordinateToAxial(FOffsetCoordinate coordinate);

	UFUNCTION(BlueprintCallable)
	static FDoubledCoordinate ConvertOffsetCoordinateToDoubled(FOffsetCoordinate coordinate);


	UFUNCTION(BlueprintCallable)
	static FOffsetCoordinate ConvertCubeCoordinateToOffset(FCubeCoordinate coordinate);

	UFUNCTION(BlueprintCallable)
	static FAxialCoordinate ConvertCubeCoordinateToAxial(FCubeCoordinate coordinate);

	UFUNCTION(BlueprintCallable)
	static FDoubledCoordinate ConvertCubeCoordinateToDoubled(FCubeCoordinate coordinate);


	UFUNCTION(BlueprintCallable)
	static FOffsetCoordinate ConvertAxialCoordinateToOffset(FAxialCoordinate coordinate);

	UFUNCTION(BlueprintCallable)
	static FCubeCoordinate ConvertAxialCoordinateToCube(FAxialCoordinate coordinate);

	UFUNCTION(BlueprintCallable)
	static FDoubledCoordinate ConvertAxialCoordinateToDoubled(FAxialCoordinate coordinate);


	UFUNCTION(BlueprintCallable)
	static FOffsetCoordinate ConvertDoubledCoordinateToOffset(FDoubledCoordinate coordinate);

	UFUNCTION(BlueprintCallable)
	static FCubeCoordinate ConvertDoubledCoordinateToCube(FDoubledCoordinate coordinate);

	UFUNCTION(BlueprintCallable)
	static FAxialCoordinate ConvertDoubledCoordinateToAxial(FDoubledCoordinate coordinate);
};
