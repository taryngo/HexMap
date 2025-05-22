// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexDirection.h"
#include <Templates/SharedPointer.h>
#include "Engine/TextureRenderTarget2D.h"
/**
 * 
 */
class MYPROJECT_API HexMetrics
{
public:
	HexMetrics();
	~HexMetrics();



	static TArray<FVector>corners;
	static float outerRadius;

	static float innerRadius;

	static float solidFactor;
	static float blendFactor;
	static float elevationStep;
	static int terracesPerSlope;
	static int terraceSteps;
	static float horizontalTerraceStepSize;
	static float verticalTerraceStepSize;
	static float cellPerturbStrength;
	static float noiseScale;
	static float elevationPerturbStrength;
	static UTexture2D* m_PerturbNoise;
	static int chunkSizeX;
	static int chunkSizeY;
	static HexMap::HexEdgeType GetEdgeType(int elevation1, int elevation2)
	{
		if (elevation1 == elevation2) 
		{
			return HexMap::HexEdgeType::Flat;
		}
		int delta = elevation2 - elevation1;
		if (delta == 1 || delta == -1) {
			return HexMap::HexEdgeType::Slope;
		}
		return HexMap::HexEdgeType::Cliff;
	}

	static const TArray<FVector>& GetCorners() {
		return corners
			;
	}

	static FVector GetFirstCorner(HexMap::HexDirection direction)
	{
		return corners[(int)direction];
	}

	static FVector GetSecondCorner(HexMap::HexDirection direction)
	{
		return corners[(int)direction + 1];
	}

	static FVector GetFirstSolidCorner(HexMap::HexDirection direction)
	{
		return corners[(int)direction] * solidFactor;
	}

	static FVector GetSecondSolidCorner(HexMap::HexDirection direction)
	{
		return corners[(int)direction + 1] * solidFactor;
	}

	static FVector GetBridge(HexMap::HexDirection direction)
	{
		return (corners[(int)direction] + corners[(int)direction + 1]) *
			blendFactor;
	}
	static FColor TerraceLerp(FColor a, FColor b, int step);
	static FVector TerraceLerp(FVector a, FVector b, int step);

	static FColor ColorLerp(FColor a, FColor b, float step);

	static TSharedPtr<class IImageWrapper> GetImageWrapperByExtention(const FString ImagePath);
	static UTexture2D* LoadTexture2D(const FString ImagePath);

	static FVector4 SampleNoise( FVector position);
	static FVector4 SampleNoisePrivate( FVector position);

	static UTextureRenderTarget2D* CreateRenderTargetCopy(UTexture2D* SourceTexture);
	// renderTarget 方式获取像素
	static FLinearColor GetPixelFromRenderTarget(UTextureRenderTarget2D* RT, FVector2D UV);
};
