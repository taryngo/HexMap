// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CHexMesh.h"
#include "CHexCell.h"
#include "HexGridChunk.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UHexGridChunk : public UObject
{
	GENERATED_BODY()
	

public:
	UHexGridChunk();
	void GenMesh();
	void Init(UWorld * world);

	void AddCell(int index, UCHexCell* cell) {
		m_Cells[index] = cell;
		cell->SetHexGridChunk(this);
	}
	void Refresh() {
		m_HexMesh->Triangulate(m_Cells);
	}
public:


	ACHexMesh* m_HexMesh = nullptr;
	TArray<UCHexCell*> m_Cells;
};
