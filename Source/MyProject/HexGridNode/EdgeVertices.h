// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexMetrics.h"
/**
 * 
 */
class MYPROJECT_API EdgeVertices
{
public:

	EdgeVertices();
	~EdgeVertices();

	FVector v1, v2, v3, v4;

	EdgeVertices(FVector corner1, FVector corner2) 
	{
		v1 = corner1;
		v2 =FMath::Lerp(corner1, corner2, 1.0f / 3.0f);
		v3 = FMath::Lerp(corner1, corner2, 2.0f / 3.0f);
		v4 = corner2;
	}

	static EdgeVertices TerraceLerp(EdgeVertices a, EdgeVertices b, int step)
	{
		EdgeVertices result;
		result.v1 = HexMetrics::TerraceLerp(a.v1, b.v1, step);
		result.v2 = HexMetrics::TerraceLerp(a.v2, b.v2, step);
		result.v3 = HexMetrics::TerraceLerp(a.v3, b.v3, step);
		result.v4 = HexMetrics::TerraceLerp(a.v4, b.v4, step);
		return result;
	}
};
