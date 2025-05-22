// Fill out your copyright notice in the Description page of Project Settings.


#include "HexCoordinates.h"
#include "HexMetrics.h"
HexCoordinates::HexCoordinates()
{
}

HexCoordinates::~HexCoordinates()
{
}

HexCoordinates HexCoordinates::FromPosition(FVector position)
{
	float x = position.X / (HexMetrics::innerRadius * 2.0f);
	float y = -x;

	float offset = position.Y / (HexMetrics::outerRadius * 3.0f);
	x -= offset;
	y -= offset;

	int iX = FMath::RoundToInt(x);
	int iY = FMath::RoundToInt(y);
	int iZ = FMath::RoundToInt(-x - y);

	if (iX + iY + iZ != 0) {
		float dX = FMath::Abs(x - iX);
		float dY = FMath::Abs(y - iY);
		float dZ = FMath::Abs(-x - y - iZ);

		if (dX > dY && dX > dZ) {
			iX = -iY - iZ;
		}
		else if (dZ > dY) {
			iZ = -iX - iY;
		}
	}

	return  HexCoordinates(iX, iZ);
}
