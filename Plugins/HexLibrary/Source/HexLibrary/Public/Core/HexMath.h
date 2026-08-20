#pragma once

// Generated
#include "HexMath.generated.h"


/*
* Grid Math from https://www.redblobgames.com/grids/hexagons/
*/


// -------------------------
// Structs
// -------------------------

/*
* The most common approach is to offset every other column or row.
* Columns are named col (q). Rows are named row (r).
* You can either offset the odd or the even column/rows, so the horizontal and vertical hexagons each have two variants.
* 
* “odd-r” horizontal layout shoves odd rows right (Pointed Bottom)
*		  (0,0) (1,0) ....
*          (0,1) (1,1)...
* 		  (0,2) (1,2) ...
* “even-r” horizontal layout shoves even rows right (Pointed Bottom)
*		      (0,0) (1,0) ...
*        (0,1) (1,1)...
*				(0,2) (1,2)...
* [Chosen Format]
* “odd-q” vertical layout shoves odd columns down (Flat Bottom)
* 		   (0,0)
*				 (1,0)
*			(0,1)
* “even-q” vertical layout shoves even columns down (Flat Bottom)
*				(0,0)
*			(1,0)
*				(0,1)
*/


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
	// --- Static Functions
	// -------------------------

	static FOffsetCoordinate FromString(const FString& str)
	{
		FString trimmedStr = str;
		trimmedStr.TrimStartAndEndInline();
		if (trimmedStr.StartsWith("(") && trimmedStr.EndsWith(")"))
		{
			trimmedStr = trimmedStr.Mid(1, trimmedStr.Len() - 2);
			TArray<FString> parts;
			trimmedStr.ParseIntoArray(parts, TEXT(","), true);
			if (parts.Num() == 2)
			{
				int32 x = FCString::Atoi(*parts[0]);
				int32 y = FCString::Atoi(*parts[1]);
				return FOffsetCoordinate(x, y);
			}
		}
		return FOffsetCoordinate();
	}

	// -------------------------
	// --- Public API
	// -------------------------
	
	FString ToString() const
	{
		 return FString::Printf(TEXT("(%d, %d)"), X, Y);
	}
};

/*
* Another way to look at hexagonal grids is to see that there are three primary axes; q, s, and r.
* 
* Each direction on the cube grid corresponds to a line on the hex grid. Each direction on the hex grid is a combination of two directions on the cube grid.
* For example, northwest on the hex grid lies between the +s and -r, so every step northwest involves adding 1 to s and subtracting 1 from r. We'll use this to calculate neighbors.
*
* The constraint is that q + r + s = 0 so the algorithms must preserve that. The constraint also ensures that there's a canonical coordinate for each hex.
* 
* r = 0: horizontal line
* s = 0: NE/SW line
* q = 0: NW/SE line
*
* Supports standard vector operations: add/subtract coordinates, multiply/divide by a scalar, etc. Offset coordinates do not support these operations.
* Cube coordinates for hexagonal grids can adapt algorithms like distances, rotation, reflection, line drawing, conversion to/from screen coordinates, etc. from 3D Cartesian vector operation.
*
*            (q, r, s)
* 
*	      (0,-1,1) (1,-1,0)    
*    (-1,0,1) (0,0,0) (1,0,-1)
*        (-1,1,0) (0,1,-1) 
*/

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


/*
* The same as the cube system except we don't store the s coordinate.
* Since we have a constraint q + r + s = 0, we can calculate s = -q-r when we need it
*/

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


/*
* Double Coordinates makes many of the algorithms easier to implement. Instead of alternation, the doubled coordinates double either the horizontal or vertical step size.
*
* It has a constraint (col + row) % 2 == 0.
* 
* “double-width” horizontal layout doubles column values
*	 (0,0)(2,0)(4,0)
*		(1,1) (3,1)
*	 (0,2)(2,2)(4,2)
* 
* “double-height” horizontal layout doubles row values
* 	 (0,0) (2,0) (4,0)
*		(1,1)  (3,1)
*	 (0,2) (2,2) (4,2)
*/

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
// Class
// -------------------------

UCLASS()
class HEXLIBRARY_API UHexMath : public UObject
{
	GENERATED_BODY()

public:
	// Neighbors
	UFUNCTION(BlueprintCallable)
	static TArray<FOffsetCoordinate> GetNeighborsOfOffsetCoordinate(FOffsetCoordinate coordinate);

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
