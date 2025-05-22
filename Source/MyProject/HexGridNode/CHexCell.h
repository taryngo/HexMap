// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProceduralMeshComponent.h"
#include "Components/TextRenderComponent.h"
//#include "HexCoordinates.h"
#include "HexDirection.h"
#include "CHexCell.generated.h"




UCLASS()
class MYPROJECT_API UCHexCell : public UObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UCHexCell();


	
protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	void CreateCell(int x, int z, int i, UMaterialInterface * material,FColor Basecolor);

	void GenerateCustomGeometry();


	void DrawDebugText(USceneComponent* DummyRoot, const FString& text, const FVector& pos, const FLinearColor& TextColor, float WorldSize);

	//FVector GetLocalPosition() { return 1; }
	void SetCoordinates(class HexCoordinates* coord);
	HexCoordinates* GetHexCoordinates();
	void GenerateUV_PlanarProjection(const TArray<FVector>& Vertices,
		TArray<FVector2D>& OutUVs,
		float UVScale = 1.0f);

	void SetCellBaseColor(FLinearColor baseColor);
	void SetColor(FColor color);
	FLinearColor GetCellBaseColor();
	FColor GetColor();
	void SetIndex(int index);
	UCHexCell* GetNeighbor(HexMap::HexDirection direction);

	void SetNeighbor(HexMap::HexDirection direction, UCHexCell* cell);

	void SetNeighborAndOpposite(HexMap::HexDirection direction, UCHexCell* cell)
	{
		m_Neighbors[(int)direction] = cell;
		cell->GetNeighbors()[(int)HexDirectionExtensions::Opposite(direction)] = this;
	}

	TArray<UCHexCell*>& GetNeighbors() {
		return m_Neighbors
			;
	}

	void SetLocalPos(FVector pos);
	FVector GetLocalPos();

	int GetElevation();
	void SetElevation(int elevation);

	void SetHexGridChunk(class UHexGridChunk* chunkPtr);

	HexMap::HexEdgeType GetEdgeType(HexMap::HexDirection direction);
	HexMap::HexEdgeType GetEdgeType(UCHexCell* otherCell);

	UHexGridChunk* GetChunk()
	{
		return chunk;
	}
	void Refresh();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UProceduralMeshComponent* ProcMesh;

private:
		class HexCoordinates* coordinates;
		UMaterialInstanceDynamic* m_DynamicMaterial = nullptr;
		FColor m_BaseColor;
		int m_Index = -1;
		TArray<UCHexCell*> m_Neighbors;
		FVector m_LocalPosition;
		int m_Elevation = 0;

		class UHexGridChunk* chunk = nullptr;
};
