// Fill out your copyright notice in the Description page of Project Settings.


#include "IronEthosGameMode.h"
#include "../UI/HexGridCellColorUserWidget.h"
#include "../HexGridNode/CHexGrid.h"
#include "../HexMapEditor/HexMapEditor.h"

AIronEthosGameMode::AIronEthosGameMode()
{

}

void AIronEthosGameMode::BeginPlay()
{

}

void AIronEthosGameMode::SetHexGrid(ACHexGrid* hexGrid)
{
	InitHexMapWidget();
	m_HexGrid = hexGrid;
	
}

void AIronEthosGameMode::InitHexMapWidget()
{
	UClass* WidgetClass = LoadClass<UHexGridCellColorUserWidget>(nullptr, TEXT("/Game/BluePrints/UI/BP_HexGridCellColor.BP_HexGridCellColor_C"));
	m_HexMapColor = CreateWidget<UHexGridCellColorUserWidget>(GetWorld(), WidgetClass);
	if (m_HexMapColor)
	{
		m_HexMapColor->AddToViewport(); // 将 UMG 添加到视口
	}
}

void AIronEthosGameMode::BindEditor(UHexMapEditor* editor)
{
	if (m_HexMapColor)
	{
		auto& ColorEvent = m_HexMapColor->GetFHexMapColorParamDelegate();
		ColorEvent.BindUObject(editor, &UHexMapEditor::OnHexMapColorCellDelegate);

		auto& SliderEvent = m_HexMapColor->GetFSliderHexMapColorParamDelegate();
		SliderEvent.BindUObject(editor, &UHexMapEditor::OnHexMapSlideCellDelegate);
	}
}
