// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HexGridCellColorUserWidget.generated.h"

DECLARE_DELEGATE_TwoParams(FHexMapColorParamDelegate, int,bool);
DECLARE_DELEGATE_OneParam(FSliderHexMapColorParamDelegate, float);
/**
 * 
 */
UCLASS()
class MYPROJECT_API UHexGridCellColorUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	bool Initialize() override;

	UFUNCTION()
		void OnYellowClick(bool state);

	UFUNCTION()
		void OnGreenClick(bool state);

	UFUNCTION()
		void OnBlueClick(bool state);


	UFUNCTION()
		void OnWhiteClick(bool state);

	UFUNCTION()
		void OnSliderChanged(float value);

	void OnColorChanaged(int type,bool state) 
	{
		OnValueChanged.ExecuteIfBound(type, state); // ´¥·¢Î¯ÍÐ
	}


	FHexMapColorParamDelegate& GetFHexMapColorParamDelegate()
	{
		return OnValueChanged;
	}
	FSliderHexMapColorParamDelegate& GetFSliderHexMapColorParamDelegate()
	{
		return OnSliderValueChanged;
	}
	void Bind(AActor * actor);
public:
	UPROPERTY(meta = (BindWidget))
	class UCheckBox* m_YellowCheckBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* m_GreenCheckBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* m_BlueCheckBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* m_WhiteCheckBox = nullptr;

	UPROPERTY(meta = (BindWidget)) 
	class USlider* m_ElevationSlider = nullptr;

	FHexMapColorParamDelegate OnValueChanged;
	FSliderHexMapColorParamDelegate OnSliderValueChanged;
};
