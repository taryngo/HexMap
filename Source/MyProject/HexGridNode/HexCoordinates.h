// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MYPROJECT_API HexCoordinates
{
public:
	HexCoordinates();
	~HexCoordinates();

	int X;
	int Y;

	HexCoordinates(int x, int z) {
		X = x;
		Y = z;
	}

	int GetY()
	{
		return Y;
	}
	
	int GetX()
	{
		return X;
	}

	int GetZ()
	{
		
		return -X - Y;
	}

	//static HexCoordinates*FromOffsetCoordinates(int x, int z) {
	//	return new HexCoordinates(x, z);
	//}

	FString ToString() {
		return FString::Printf(TEXT("( %d , %d )"),X,Y);
	}

	FString ToStringOnSeparateLines() 
	{
		return FString::Printf(TEXT("( %d \n %d )"), X, Y);
	}

	static HexCoordinates*FromOffsetCoordinates(int x, int z)
	{
		return new HexCoordinates(x - z / 2, z);
	}

	static HexCoordinates FromPosition(FVector position);

};
