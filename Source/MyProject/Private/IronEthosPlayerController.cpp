// Fill out your copyright notice in the Description page of Project Settings.


#include "IronEthosPlayerController.h"

#include "../HexMapEditor/HexMapEditor.h"

void AIronEthosPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // ������������¼�
    InputComponent->BindAction("Click", IE_Pressed, this, &AIronEthosPlayerController::OnMouseClick);
}

void AIronEthosPlayerController::OnMouseClick()
{
    // ��ȡ���λ��
    float MouseX, MouseY;
    GetMousePosition(MouseX, MouseY);

    // ת��Ϊ����ռ�����
    FVector WorldLocation, WorldDirection;
    DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection);

    //m_HexGrid->TouchCell(WorldLocation);
    // ���������յ�
    FVector Start = WorldLocation;
    FVector End = Start + (WorldDirection * 10000.f);

    FHitResult HitResult;
    FCollisionQueryParams TraceParams(FName(TEXT("MouseTrace")), true);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams))
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            if (m_HexMapEditor)
            {
                m_HexMapEditor->HandleInput(HitResult.Location);
                return;
            }
            //HighlightActor(HitResult.GetActor());
            // �����Զ����¼���������� Actor��
            //UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
        }
    }

    // ��ѡ�����Ƶ�������
    //DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);
}

void AIronEthosPlayerController::HighlightActor(AActor* ActorToHighlight)
{
    
    if (!ActorToHighlight || ActorToHighlight == CurrentlyHighlightedActor) return;

    RestoreOriginalMaterials();

    TArray<UMeshComponent*> MeshComponents;
    ActorToHighlight->GetComponents<UMeshComponent>(MeshComponents);

    for (UMeshComponent* MeshComponent : MeshComponents)
    {
        for (int32 i = 0; i < MeshComponent->GetMaterials().Num(); i++)
        {
            UMaterialInterface* OriginalMaterial = MeshComponent->GetMaterial(i);
            OriginalMaterials.Add(MeshComponent, OriginalMaterial);

            UMaterialInstanceDynamic* DynamicMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(i);
            if (DynamicMaterial)
            {
                auto actorLocation = ActorToHighlight->GetActorLocation();
                int x = actorLocation.X;
                int z = actorLocation.Z;
                DynamicMaterial->SetScalarParameterValue("EmissiveIntensity", 10.0f);
                //DynamicMaterial->SetVectorParameterValue("HighlightColor", FLinearColor::Red);
                DynamicMaterial->SetVectorParameterValue("BaseColor", FLinearColor::Red);
                //GEngine->AddOnScreenDebugMessage(-1, 1000000, FColor::Red, FString::Printf(TEXT("Highlight X:%d - Z:%d\n"),x,z));
            }
        }
    }

    CurrentlyHighlightedActor = ActorToHighlight;
}

void AIronEthosPlayerController::RestoreOriginalMaterials()
{
    if (!CurrentlyHighlightedActor) return;

    TArray<UMeshComponent*> MeshComponents;
    CurrentlyHighlightedActor->GetComponents<UMeshComponent>(MeshComponents);

    for (UMeshComponent* MeshComponent : MeshComponents)
    {
        for (int32 i = 0; i < MeshComponent->GetMaterials().Num(); i++)
        {
            if (UMaterialInterface** OriginalMaterial = OriginalMaterials.Find(MeshComponent))
            {
                MeshComponent->SetMaterial(i, *OriginalMaterial);
            }
        }
    }

    OriginalMaterials.Empty();
    CurrentlyHighlightedActor = nullptr;
}

void AIronEthosPlayerController::SetHexGrid(ACHexGrid* hexGrid)
{
    m_HexGrid = hexGrid;
}

void AIronEthosPlayerController::SetHexMapEditor(UHexMapEditor* hexEditor)
{
    m_HexMapEditor = hexEditor;
}

void AIronEthosPlayerController::BeginPlay()
{

}
