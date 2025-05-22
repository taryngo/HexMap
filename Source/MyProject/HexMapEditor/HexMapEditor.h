// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "HexMapEditor.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UHexMapEditor : public UObject
{
	GENERATED_BODY()

public:
	void SetHexGrid(class ACHexGrid* hexGrid);
	void HandleInput(FVector position);
	void EditCell(class UCHexCell* cell);

	void OnHexMapColorCellDelegate(int type, bool state);
	void OnHexMapSlideCellDelegate(float value);
public:
	UPROPERTY(VisibleAnywhere)
		class ACHexGrid* m_HexGrid;

	FColor m_ActiveColor = FColor(200, 200,0, 200);
	int m_ActiveElevation = 0;
};
