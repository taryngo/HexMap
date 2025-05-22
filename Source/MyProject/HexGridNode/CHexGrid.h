// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "CHexCell.h"
#include "HexGridChunk.h"
#include "CHexGrid.generated.h"

UCLASS()
class MYPROJECT_API ACHexGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACHexGrid();
	void CreateCells();

	void OnHexMapColorCellDelegate(int type , bool state);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void CreateCell(int x, int z, int i, FColor Basecolor);
	void GenHexMapColor();
	void InitGameModeAndController();
	void GenMesh();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TouchCell(FVector  position);
	void ColorCell(FVector position, FColor color);
	UCHexCell* GetCell(FVector position);

	void Refresh();
	int GetColorSelectType() {
		return m_ColorSelectType
			;
	}
	void AddCellToChunk(int x,int  y, UCHexCell* cell);
	void CreateChunks();
public:

	//ACHexCell m_cellPrefab;
	UPROPERTY(VisibleAnywhere)
		TArray<UCHexCell*> m_cells;
	//TSharedPtr<class ACHexMesh> m_CHexMesh;
	UPROPERTY(VisibleAnywhere)
		class ACHexMesh* m_CHexMesh;
	int m_cellCountX = 10;
	int m_cellCountY = 10;

	class UMaterialInterface* m_GridCellMaterial = nullptr;
	typedef TMap<int, bool> HexMapGridColorMap;
	HexMapGridColorMap m_HexMapGridColorMap;
	int m_ColorSelectType = 0;

	UPROPERTY(VisibleAnywhere)
	class UHexMapEditor* m_MapEditor = nullptr;

	int chunkCountX =4;
	int chunkCountY =3;

	TArray<UHexGridChunk*> chunks;
};
