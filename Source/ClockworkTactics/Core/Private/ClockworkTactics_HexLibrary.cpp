// Parent
#include "ClockworkTactics_HexLibrary.h"

// Neighbors

TArray<FOffsetCoordinate> UClockworkTactics_HexLibrary::GetNeighborsOfOffsetCoordinate(FOffsetCoordinate coordinate)
{
    TArray<FOffsetCoordinate> neighbors = TArray<FOffsetCoordinate>();
    neighbors.Add(FOffsetCoordinate(coordinate.X + 1, coordinate.Y));
    neighbors.Add(FOffsetCoordinate(coordinate.X - 1, coordinate.Y));
    neighbors.Add(FOffsetCoordinate(coordinate.X, coordinate.Y - 1));
    neighbors.Add(FOffsetCoordinate(coordinate.X, coordinate.Y + 1));

    // Diagonal changes based on even vs odd row
    neighbors.Add(FOffsetCoordinate(coordinate.X - 1 + 2 * (coordinate.Y % 2), coordinate.Y - 1));
    neighbors.Add(FOffsetCoordinate(coordinate.X - 1 + 2 * (coordinate.Y % 2), coordinate.Y - 1));

    return neighbors;
}

TArray<FDoubledCoordinate> UClockworkTactics_HexLibrary::GetNeighborsOfDoubledCoordinate(FDoubledCoordinate coordinate)
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

TArray<FCubeCoordinate> UClockworkTactics_HexLibrary::GetNeighborsOfCubeCoordinate(FCubeCoordinate coordinate)
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

TArray<FAxialCoordinate> UClockworkTactics_HexLibrary::GetNeighborsOfAxialCoordinate(FAxialCoordinate coordinate)
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

uint8 UClockworkTactics_HexLibrary::GetDistanceBetweenOffsetCoordinates(FOffsetCoordinate start, FOffsetCoordinate end)
{
    FCubeCoordinate startCube = ConvertOffsetCoordinateToCube(start);
    FCubeCoordinate endCube = ConvertOffsetCoordinateToCube(end);

    return GetDistanceBetweenCubeCoordinates(startCube, endCube);
}

uint8 UClockworkTactics_HexLibrary::GetDistanceBetweenDoubledCoordinates(FDoubledCoordinate start, FDoubledCoordinate end)
{
    FDoubledCoordinate delta = end - start;
    return FMath::Abs(delta.Y) + FMath::Max(0, (FMath::Abs(delta.X) - FMath::Abs(delta.Y) / 2));
}

uint8 UClockworkTactics_HexLibrary::GetDistanceBetweenCubeCoordinates(FCubeCoordinate start, FCubeCoordinate end)
{
    FCubeCoordinate delta = end - start;

    return (FMath::Abs(delta.Q) + FMath::Abs(delta.S) + FMath::Abs(delta.R)) / 2;
}

uint8 UClockworkTactics_HexLibrary::GetDistanceBetweenAxialCoordinates(FAxialCoordinate start, FAxialCoordinate end)
{
    FAxialCoordinate delta = end - start;

    return (FMath::Abs(delta.Q) + FMath::Abs(delta.Q + delta.R) + FMath::Abs(delta.R)) / 2;
}

// Conversion

FCubeCoordinate UClockworkTactics_HexLibrary::ConvertOffsetCoordinateToCube(FOffsetCoordinate coordinate)
{
    uint8 q = coordinate.X - (coordinate.Y - (coordinate.Y & 1)) / 2;
    uint8 r = coordinate.Y;
    
    return FCubeCoordinate(r, -q - r, q);
}

FAxialCoordinate UClockworkTactics_HexLibrary::ConvertOffsetCoordinateToAxial(FOffsetCoordinate coordinate)
{
    return FAxialCoordinate(coordinate.Y, coordinate.X - (coordinate.Y - (coordinate.Y & 1)) / 2);
}

FDoubledCoordinate UClockworkTactics_HexLibrary::ConvertOffsetCoordinateToDoubled(FOffsetCoordinate coordinate)
{
    return FDoubledCoordinate(coordinate.X * 2 + coordinate.Y % 2, coordinate.Y);
}


FOffsetCoordinate UClockworkTactics_HexLibrary::ConvertCubeCoordinateToOffset(FCubeCoordinate coordinate)
{
    return FOffsetCoordinate(coordinate.Q + (coordinate.R - (coordinate.R & 1)) / 2, coordinate.R);
}

FAxialCoordinate UClockworkTactics_HexLibrary::ConvertCubeCoordinateToAxial(FCubeCoordinate coordinate)
{
    return FAxialCoordinate(coordinate.R, coordinate.Q);
}

FDoubledCoordinate UClockworkTactics_HexLibrary::ConvertCubeCoordinateToDoubled(FCubeCoordinate coordinate)
{
    return FDoubledCoordinate(2 * coordinate.Q + coordinate.R, coordinate.R);
}


FOffsetCoordinate UClockworkTactics_HexLibrary::ConvertAxialCoordinateToOffset(FAxialCoordinate coordinate)
{
    return FOffsetCoordinate(coordinate.Q + (coordinate.R - (coordinate.R & 1)) / 2, coordinate.R);
}

FCubeCoordinate UClockworkTactics_HexLibrary::ConvertAxialCoordinateToCube(FAxialCoordinate coordinate)
{
    return FCubeCoordinate(coordinate.R, -coordinate.Q - coordinate.R, coordinate.Q);
}

FDoubledCoordinate UClockworkTactics_HexLibrary::ConvertAxialCoordinateToDoubled(FAxialCoordinate coordinate)
{
    return FDoubledCoordinate(2 * coordinate.Q + coordinate.R, coordinate.R);
}


FOffsetCoordinate UClockworkTactics_HexLibrary::ConvertDoubledCoordinateToOffset(FDoubledCoordinate coordinate)
{
    return FOffsetCoordinate(coordinate.X / 2, coordinate.Y);
}

FCubeCoordinate UClockworkTactics_HexLibrary::ConvertDoubledCoordinateToCube(FDoubledCoordinate coordinate)
{
    uint8 q = (coordinate.X - coordinate.Y) / 2;
    uint8 r = coordinate.Y;

    return FCubeCoordinate(r, -q - r, q);
}

FAxialCoordinate UClockworkTactics_HexLibrary::ConvertDoubledCoordinateToAxial(FDoubledCoordinate coordinate)
{
    return FAxialCoordinate(coordinate.Y,(coordinate.X - coordinate.Y) / 2);
}
