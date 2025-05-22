// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IronEthosGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AIronEthosGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AIronEthosGameMode();

	virtual void BeginPlay() override;

	void SetHexGrid(class ACHexGrid* hexGrid);

	void InitHexMapWidget();

	void BindEditor(class UHexMapEditor* editor);
private:
	class ACHexGrid* m_HexGrid = nullptr;
	class UHexGridCellColorUserWidget* m_HexMapColor = nullptr;
};
