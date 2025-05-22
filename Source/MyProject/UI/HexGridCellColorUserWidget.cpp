// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGridCellColorUserWidget.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
bool UHexGridCellColorUserWidget::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}
	return false;
}

void UHexGridCellColorUserWidget::OnYellowClick(bool state)
{
	OnColorChanaged(1, state);
}

void UHexGridCellColorUserWidget::OnGreenClick(bool state)
{
	OnColorChanaged(3, state);

}

void UHexGridCellColorUserWidget::OnBlueClick(bool state)
{
	OnColorChanaged(2, state);

}

void UHexGridCellColorUserWidget::OnWhiteClick(bool state)
{
	OnColorChanaged(4, state);

}

void UHexGridCellColorUserWidget::OnSliderChanged(float value)
{
	OnSliderValueChanged.ExecuteIfBound(value);
}



void UHexGridCellColorUserWidget::Bind(AActor* actor)
{

}

void UHexGridCellColorUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	m_YellowCheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &UHexGridCellColorUserWidget::OnYellowClick);
	m_GreenCheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &UHexGridCellColorUserWidget::OnGreenClick);
	m_BlueCheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &UHexGridCellColorUserWidget::OnBlueClick);
	m_WhiteCheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &UHexGridCellColorUserWidget::OnWhiteClick);

	if (m_ElevationSlider)
	{
		// 设置初始值范围
		m_ElevationSlider->SetMinValue(0.0f);
		m_ElevationSlider->SetMaxValue(10.0f);
		m_ElevationSlider->SetValue(0); // 默认值

		// 绑定值变化事件
		m_ElevationSlider->OnValueChanged.AddUniqueDynamic(this, &UHexGridCellColorUserWidget::OnSliderChanged);
	}
}