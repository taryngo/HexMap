// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/**
 * 
 */
class CProceduralMeshHelper
{
public:
	CProceduralMeshHelper();
	~CProceduralMeshHelper();

	static void CreateSectorMesh(FVector CenterPoint, FVector Forward, FVector Up, float Angle, float Radius, float Height, int32 SectionNum, TArray<FVector>& Out_Vertices, TArray<int32>& Out_Triangles);

	static TArray<FVector> CalSectorPoints(FVector CenterPoint, FVector ForwardVector, FVector UpVector, float Angle, float Radius, int32 SectionNum);

	static TArray<int32> CircleTriangles(int32 CenterIdx, int32 StartIdx, int32 EndIdx, bool bAnticlockwise);

	static TArray<int32> RectangleTriangles(int32 Leftdown, int32 Lefttop, int32 Rightdown, int32 Righttop);

	static void CreateCubeMesh(FVector CubeRadius, TArray<FVector>& Vertices, TArray<int32>& Triangles);

	static void CreateSphereMesh(FVector CenterPoint, float Radius, int32 SectionNum, TArray<FVector>& Out_Vertices, TArray<int32>& Out_Triangles);
};
