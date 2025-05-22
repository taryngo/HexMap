// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
//class MYPROJECT_API HexDirection
//{
//public:
//	HexDirection();
//	~HexDirection();
//};
namespace HexMap
{
	enum HexDirection
	{
		/// <summary>Northeast.</summary>
		NE,
		/// <summary>East.</summary>
		E,
		/// <summary>Southeast.</summary>
		SE,
		/// <summary>Southwest.</summary>
		SW,
		/// <summary>West.</summary>
		W,
		/// <summary>Northwest.</summary>
		NW
	};

	enum HexEdgeType
	{
		Flat, Slope, Cliff
	};
}


class MYPROJECT_API HexDirectionExtensions
{
public:
	static HexMap::HexDirection Opposite(HexMap::HexDirection direction)
	{
		return HexMap::HexDirection((int)direction < 3 ? (direction + 3) : (direction - 3));
	}

	static HexMap::HexDirection Previous(HexMap::HexDirection direction)
	{
		return direction == HexMap::NE ? HexMap::NW : HexMap::HexDirection((int)direction - 1);
	}

	static HexMap::HexDirection Next(HexMap::HexDirection direction)
	{
		return direction == HexMap::NW ? HexMap::NE : HexMap::HexDirection((int)direction + 1);
	}
};