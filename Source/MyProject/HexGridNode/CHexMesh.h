// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexDirection.h"
#include "ProceduralMeshComponent.h"
#include "EdgeVertices.h"

#include "CHexMesh.generated.h"

UCLASS()
class MYPROJECT_API ACHexMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACHexMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void GenMesh();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Triangulate(const TArray<class UCHexCell*> &cells);

	void AddTriangle(FVector v1, FVector v2, FVector v3);
	void Triangulate(class UCHexCell* cell);
	void Triangulate(HexMap::HexDirection direction, class UCHexCell* cell);

	void TriangulateConnection(HexMap::HexDirection direction, UCHexCell* cell, EdgeVertices e1);
	void TriangulateEdgeTerraces(
		EdgeVertices begin, UCHexCell* beginCell,
		EdgeVertices end, UCHexCell* endCell
	);
	void TriangulateEdgeStrip(
		EdgeVertices e1, FColor c1,
		EdgeVertices e2, FColor c2
	);
	void AddTriangleColor(FColor c1, FColor c2, FColor c3);
	void AddQuad(FVector v1, FVector v2, FVector v3, FVector  v4);
	void AddQuadColor(FColor c1, FColor c2);
	void AddQuadColor(FColor c1, FColor c2, FColor c3, FColor c4);
	void AddTriangleColor(FColor color);

	void UpdateMeshSection();

	void TriangulateCorner(
		FVector bottom, UCHexCell* bottomCell,
		FVector left, UCHexCell* leftCell,
		FVector right, UCHexCell* rightCell
	);
	void TriangulateCornerTerraces(
		FVector  begin, UCHexCell* beginCell,
		FVector left, UCHexCell* leftCell,
		FVector right, UCHexCell* rightCell
	);

	void TriangulateCornerTerracesCliff(FVector begin, UCHexCell* beginCell,
		FVector  left, UCHexCell* leftCell,
		FVector right, UCHexCell* rightCell);

	void TriangulateCornerCliffTerraces(
		FVector begin, UCHexCell* beginCell,
		FVector left, UCHexCell* leftCell,
		FVector right, UCHexCell* rightCell
	);
	void TriangulateBoundaryTriangle(
		FVector begin, UCHexCell* beginCell,
		FVector left, UCHexCell* leftCell,
		FVector boundary, FColor boundaryColor
	);

	FVector Perturb(FVector position);

	void TriangulateEdgeFan(FVector center, EdgeVertices edge, FColor color);
	void AddTriangleUnperturbed(FVector  v1, FVector v2, FVector v3);
private:
	TArray<FVector> m_Vertices;
	TArray<int32> m_Triangles;
	TArray<FVector> m_Normals;
	TArray<FColor> m_Colors;



	UMaterialInstanceDynamic* m_DynamicMaterial = nullptr;
	FLinearColor m_BaseColor;
	bool m_IsUpdateMeshSection = false;
	UMaterial* m_BaseMaterial = nullptr;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UProceduralMeshComponent* m_ProcMesh=nullptr;

	UPROPERTY(VisibleAnywhere)
	UTexture2D* m_NoiseVertexTexture = nullptr;
};
