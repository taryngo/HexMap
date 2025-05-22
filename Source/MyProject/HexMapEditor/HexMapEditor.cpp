// Fill out your copyright notice in the Description page of Project Settings.


#include "HexMapEditor.h"
#include "../HexGridNode/CHexGrid.h"
#include "../HexGridNode/CHexCell.h"

void UHexMapEditor::SetHexGrid(ACHexGrid* hexGrid)
{
	m_HexGrid = hexGrid;
}

void UHexMapEditor::HandleInput(FVector position)
{
	UCHexCell * cell  = m_HexGrid->GetCell(position);
	EditCell(cell);
}


void UHexMapEditor::EditCell(UCHexCell*cell)
{
	cell->SetColor(m_ActiveColor);
	//cell.Elevation = m_ActiveEvevation;
	cell->SetElevation(m_ActiveElevation);
	cell->Refresh();
	
}

void UHexMapEditor::OnHexMapColorCellDelegate(int type, bool state)
{
	if (!state)
		return;
	int colorType = type;
	if (colorType == 1)
	{
		m_ActiveColor = FColor::Yellow;
	}
	else if (colorType == 2)
	{
		m_ActiveColor = FColor::Blue;
	}
	else if (colorType == 3)
	{
		m_ActiveColor = FColor::Green;
	}
	else if (colorType == 4)
	{
		m_ActiveColor = FColor::White;
	}
}

void UHexMapEditor::OnHexMapSlideCellDelegate(float value)
{
	m_ActiveElevation = value;
}
