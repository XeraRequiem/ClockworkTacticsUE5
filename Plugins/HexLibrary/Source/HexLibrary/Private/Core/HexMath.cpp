// Parent
#include "Core/HexMath.h"


// Neighbors

TArray<FOffsetCoordinate> UHexMath::GetNeighborsOfOffsetCoordinate(FOffsetCoordinate Coordinate)
{
    TArray<FOffsetCoordinate> neighbors = TArray<FOffsetCoordinate>();
    neighbors.Add(FOffsetCoordinate(Coordinate.X + 1, Coordinate.Y));
    neighbors.Add(FOffsetCoordinate(Coordinate.X - 1, Coordinate.Y));
    neighbors.Add(FOffsetCoordinate(Coordinate.X, Coordinate.Y - 1));
    neighbors.Add(FOffsetCoordinate(Coordinate.X, Coordinate.Y + 1));

    // Diagonal changes based on even vs odd row (Assumes odd-r)

	 if (Coordinate.Y % 2 == 0)
	 {
		  // Even Row
		  neighbors.Add(FOffsetCoordinate(Coordinate.X - 1, Coordinate.Y + 1));
		  neighbors.Add(FOffsetCoordinate(Coordinate.X - 1, Coordinate.Y - 1));
	 }
	 else
	 {
		  // Odd Row
		  neighbors.Add(FOffsetCoordinate(Coordinate.X + 1, Coordinate.Y + 1));
	     neighbors.Add(FOffsetCoordinate(Coordinate.X + 1, Coordinate.Y - 1));
	 }

    return neighbors;
}

TArray<FDoubledCoordinate> UHexMath::GetNeighborsOfDoubledCoordinate(FDoubledCoordinate coordinate)
{
    TArray<FDoubledCoordinate> neighbors = TArray<FDoubledCoordinate>();
    neighbors.Add(FDoubledCoordinate(coordinate.X + 2, coordinate.Y));
    neighbors.Add(FDoubledCoordinate(coordinate.X - 2, coordinate.Y));

    neighbors.Add(FDoubledCoordinate(coordinate.X + 1, coordinate.Y - 1));
    neighbors.Add(FDoubledCoordinate(coordinate.X - 1, coordinate.Y + 1));

    neighbors.Add(FDoubledCoordinate(coordinate.X + 1, coordinate.Y + 1));
    neighbors.Add(FDoubledCoordinate(coordinate.X - 1, coordinate.Y - 1));

    return neighbors;
}

TArray<FCubeCoordinate> UHexMath::GetNeighborsOfCubeCoordinate(FCubeCoordinate coordinate)
{
    TArray<FCubeCoordinate> neighbors = TArray<FCubeCoordinate>();
    neighbors.Add(FCubeCoordinate(coordinate.R + 1, coordinate.S - 1, coordinate.Q));
    neighbors.Add(FCubeCoordinate(coordinate.R + 1, coordinate.S, coordinate.Q - 1));
    neighbors.Add(FCubeCoordinate(coordinate.R - 1, coordinate.S + 1, coordinate.Q));
    neighbors.Add(FCubeCoordinate(coordinate.R - 1, coordinate.S, coordinate.Q + 1));
    neighbors.Add(FCubeCoordinate(coordinate.R, coordinate.S + 1, coordinate.Q - 1));
    neighbors.Add(FCubeCoordinate(coordinate.R, coordinate.S - 1, coordinate.Q + 1));

    return neighbors;
}

TArray<FAxialCoordinate> UHexMath::GetNeighborsOfAxialCoordinate(FAxialCoordinate coordinate)
{
    TArray<FAxialCoordinate> neighbors = TArray<FAxialCoordinate>();
    neighbors.Add(FAxialCoordinate(coordinate.R + 1, coordinate.Q));
    neighbors.Add(FAxialCoordinate(coordinate.R + 1, coordinate.Q - 1));
    neighbors.Add(FAxialCoordinate(coordinate.R - 1, coordinate.Q));
    neighbors.Add(FAxialCoordinate(coordinate.R - 1, coordinate.Q + 1));
    neighbors.Add(FAxialCoordinate(coordinate.R, coordinate.Q - 1));
    neighbors.Add(FAxialCoordinate(coordinate.R, coordinate.Q + 1));

    return neighbors;
}

// Distance 

uint8 UHexMath::GetDistanceBetweenOffsetCoordinates(FOffsetCoordinate start, FOffsetCoordinate end)
{
    FCubeCoordinate startCube = ConvertOffsetCoordinateToCube(start);
    FCubeCoordinate endCube = ConvertOffsetCoordinateToCube(end);

    return GetDistanceBetweenCubeCoordinates(startCube, endCube);
}

uint8 UHexMath::GetDistanceBetweenDoubledCoordinates(FDoubledCoordinate start, FDoubledCoordinate end)
{
    FDoubledCoordinate delta = end - start;
    return FMath::Abs(delta.Y) + FMath::Max(0, (FMath::Abs(delta.X) - FMath::Abs(delta.Y) / 2));
}

uint8 UHexMath::GetDistanceBetweenCubeCoordinates(FCubeCoordinate start, FCubeCoordinate end)
{
    FCubeCoordinate delta = end - start;

    return (FMath::Abs(delta.Q) + FMath::Abs(delta.S) + FMath::Abs(delta.R)) / 2;
}

uint8 UHexMath::GetDistanceBetweenAxialCoordinates(FAxialCoordinate start, FAxialCoordinate end)
{
    FAxialCoordinate delta = end - start;

    return (FMath::Abs(delta.Q) + FMath::Abs(delta.Q + delta.R) + FMath::Abs(delta.R)) / 2;
}

// Conversion

FCubeCoordinate UHexMath::ConvertOffsetCoordinateToCube(FOffsetCoordinate coordinate)
{
    uint8 q = coordinate.X - (coordinate.Y - (coordinate.Y & 1)) / 2;
    uint8 r = coordinate.Y;
    
    return FCubeCoordinate(r, -q - r, q);
}

FAxialCoordinate UHexMath::ConvertOffsetCoordinateToAxial(FOffsetCoordinate coordinate)
{
    return FAxialCoordinate(coordinate.Y, coordinate.X - (coordinate.Y - (coordinate.Y & 1)) / 2);
}

FDoubledCoordinate UHexMath::ConvertOffsetCoordinateToDoubled(FOffsetCoordinate coordinate)
{
    return FDoubledCoordinate(coordinate.X * 2 + coordinate.Y % 2, coordinate.Y);
}


FOffsetCoordinate UHexMath::ConvertCubeCoordinateToOffset(FCubeCoordinate coordinate)
{
    return FOffsetCoordinate(coordinate.Q + (coordinate.R - (coordinate.R & 1)) / 2, coordinate.R);
}

FAxialCoordinate UHexMath::ConvertCubeCoordinateToAxial(FCubeCoordinate coordinate)
{
    return FAxialCoordinate(coordinate.R, coordinate.Q);
}

FDoubledCoordinate UHexMath::ConvertCubeCoordinateToDoubled(FCubeCoordinate coordinate)
{
    return FDoubledCoordinate(2 * coordinate.Q + coordinate.R, coordinate.R);
}


FOffsetCoordinate UHexMath::ConvertAxialCoordinateToOffset(FAxialCoordinate coordinate)
{
    return FOffsetCoordinate(coordinate.Q + (coordinate.R - (coordinate.R & 1)) / 2, coordinate.R);
}

FCubeCoordinate UHexMath::ConvertAxialCoordinateToCube(FAxialCoordinate coordinate)
{
    return FCubeCoordinate(coordinate.R, -coordinate.Q - coordinate.R, coordinate.Q);
}

FDoubledCoordinate UHexMath::ConvertAxialCoordinateToDoubled(FAxialCoordinate coordinate)
{
    return FDoubledCoordinate(2 * coordinate.Q + coordinate.R, coordinate.R);
}


FOffsetCoordinate UHexMath::ConvertDoubledCoordinateToOffset(FDoubledCoordinate coordinate)
{
    return FOffsetCoordinate(coordinate.X / 2, coordinate.Y);
}

FCubeCoordinate UHexMath::ConvertDoubledCoordinateToCube(FDoubledCoordinate coordinate)
{
    uint8 q = (coordinate.X - coordinate.Y) / 2;
    uint8 r = coordinate.Y;

    return FCubeCoordinate(r, -q - r, q);
}

FAxialCoordinate UHexMath::ConvertDoubledCoordinateToAxial(FDoubledCoordinate coordinate)
{
    return FAxialCoordinate(coordinate.Y,(coordinate.X - coordinate.Y) / 2);
}
