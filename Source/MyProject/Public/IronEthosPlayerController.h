// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IronEthosPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AIronEthosPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
    virtual void SetupInputComponent() override;

    // 鼠标点击事件回调函数
    UFUNCTION()
        void OnMouseClick();

    void HighlightActor(AActor* ActorToHighlight);
    void RestoreOriginalMaterials();
    void SetHexGrid(class ACHexGrid* hexGrid);
    void SetHexMapEditor(class UHexMapEditor* hexEditor);
    virtual void BeginPlay() override;

    UHexMapEditor* GetEditor() {
        return m_HexMapEditor
            ;
    }
private:
    AActor* CurrentlyHighlightedActor = nullptr;
    TMap<UMeshComponent*, UMaterialInterface*> OriginalMaterials;

    UPROPERTY(VisibleAnywhere)
    class ACHexGrid* m_HexGrid=nullptr;

    UPROPERTY(VisibleAnywhere)
    class UHexMapEditor* m_HexMapEditor= nullptr;
};
