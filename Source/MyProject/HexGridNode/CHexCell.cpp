// Fill out your copyright notice in the Description page of Project Settings.


#include "CHexCell.h"
#include "KismetProceduralMeshLibrary.h"
#include "CProceduralMeshHelper.h"
//#include "HexCoordinates.h"
#include "HexMetrics.h"
#include "HexGridChunk.h"
const float fHexCellWidth = 5;
const float fHexCellHeight = 5;
// Sets default values
UCHexCell::UCHexCell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;


	//ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
	//RootComponent = ProcMesh;
    //m_Neighbors.Reset(6);
    m_Neighbors.SetNum(6);
}

// Called when the game starts or when spawned
//void ACHexCell::BeginPlay()
//{
//	Super::BeginPlay();
//    
//}

// Called every frame
//void ACHexCell::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void UCHexCell::CreateCell(int x, int z, int i, UMaterialInterface* material, FColor Basecolor)
{
    SetIndex(i);
    m_BaseColor = Basecolor;
    //GenerateCustomGeometry();
	FVector position;
    // Y -Z 互相调换
	position.X = (x + z * 0.5f - z / 2) * (HexMetrics::innerRadius * 2.0f);
	//position.Y = 0.0f;
	//position.Z = z * HexMetrics::outerRadius * 1.5f;
    position.Y = z * HexMetrics::outerRadius * 1.5f;
    position.Z = 0.0f;

    SetLocalPos(position);
	//auto node = CreateDefaultSubobject<USceneComponent>(TEXT("CustomScene"));
	//cell.SetRootComponent(node);
	//SetActorLocation(position);

    FVector textPosition(0.0,0.0,0.0);
    textPosition.Y -= 1;
    //auto scene =this->GetRootComponent();
    //FString text = FString::Printf(TEXT("%d\n%d"), (int)x, (int)z);
    // DrawDebugText(scene, text, textPosition,FLinearColor::Yellow,3);


	//cell.GetParentComponent();
	//cell.SetRootComponent(GetRootComponent());
}


void UCHexCell::GenerateCustomGeometry()
{
    
    // 定义正方形的顶点坐标（X-Y平面，Z=0）
    TArray<FVector> Vertices;
    float Size = fHexCellHeight; // 正方形边长的一半
    Vertices.Add(FVector(-Size, 0, -Size)); // 左下
    Vertices.Add(FVector(-Size, 0, Size)); // 左上
    Vertices.Add(FVector(Size, 0, Size)); // 右上
    Vertices.Add(FVector(Size, 0, -Size)); // 右下

    // 定义三角形索引（两个三角形，顺时针方向）
    TArray<int32> Triangles;
    Triangles.Add(0); Triangles.Add(1); Triangles.Add(2); // 第一个三角形
    Triangles.Add(0); Triangles.Add(2); Triangles.Add(3); // 第二个三角形

    // 定义UV坐标（可选）
    TArray<FVector2D> UVs;
    UVs.Add(FVector2D(0, 0));
    UVs.Add(FVector2D(0, 1));
    UVs.Add(FVector2D(1, 1));
    UVs.Add(FVector2D(1, 0));

    // 生成网格（不计算法线和切线）
    //ProcMesh->CreateMeshSection(
    //    0,          // Section Index
    //    Vertices,    // 顶点
    //    Triangles,   // 三角形索引
    //    TArray<FVector>(),   // 法线（留空自动计算）
    //    UVs,         // UV
    //    TArray<FColor>(),    // 顶点颜色
    //    TArray<FProcMeshTangent>(), // 切线（留空自动计算）
    //    false       // 是否启用碰撞
    //);

    //// 设置材质
    //auto  material = LoadObject<UMaterial>(nullptr, TEXT("/Game/StarterContent/Materials/M_Wood_Walnut"));
    //if (material)
    //{
    //    ProcMesh->SetMaterial(0, material);
    //}
    
}

void UCHexCell::DrawDebugText(USceneComponent* DummyRoot, const FString& text, const FVector& pos, const FLinearColor& TextColor, float WorldSize)
{
    UTextRenderComponent* ScoreText = nullptr;

    // Create static mesh component
    //ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
    ScoreText = NewObject<UTextRenderComponent>(this, UTextRenderComponent::StaticClass());
    ScoreText->RegisterComponent();
    ScoreText->AttachToComponent(DummyRoot, FAttachmentTransformRules::KeepRelativeTransform);
    //ScoreText->AttachTo(DummyRoot);
    ScoreText->SetRelativeRotation(FRotator(0, -90.f, 0.f));
    ScoreText->SetRelativeLocation(pos);
    ScoreText->SetWorldSize(WorldSize);
    ScoreText->SetTextRenderColor(FColor::Yellow);
    // Update text
    ScoreText->SetText(text);
}

void UCHexCell::SetCoordinates(HexCoordinates* coord)
{
    coordinates = coord;
}

HexCoordinates* UCHexCell::GetHexCoordinates()
{
    return coordinates;
}

void UCHexCell::GenerateUV_PlanarProjection(
    const TArray<FVector>& Vertices,
    TArray<FVector2D>& OutUVs,
    float UVScale
) 
{
    OutUVs.Empty();

    // 获取顶点包围盒范围
    FBox Bounds(Vertices);
    FVector Min = Bounds.Min;
    FVector Size = Bounds.GetSize();

    for (const FVector& Vertex : Vertices) {
        // 投影到 XZ 平面并归一化
        float U = (Vertex.X - Min.X) / Size.X;
        float V = (Vertex.Z - Min.Z) / Size.Z;
        OutUVs.Add(FVector2D(U, V) * UVScale);
    }
}
void UCHexCell::SetCellBaseColor(FLinearColor baseColor)
{
    //if (m_DynamicMaterial)
    //{
    //    m_DynamicMaterial->SetVectorParameterValue("BaseColor", baseColor);
    //}
}
void UCHexCell::SetColor(FColor color)
{
    m_BaseColor = color;
}
FLinearColor UCHexCell::GetCellBaseColor()
{
    return FLinearColor(m_BaseColor);
}
FColor UCHexCell::GetColor()
{
    return m_BaseColor;
}
void UCHexCell::SetIndex(int index)
{
    m_Index = index;
}
UCHexCell* UCHexCell::GetNeighbor(HexMap::HexDirection direction)
{
    if (m_Neighbors.Num() <= int(direction))
    {
        return nullptr;
    }
    return m_Neighbors[(int)direction];
}
void UCHexCell::SetNeighbor(HexMap::HexDirection direction, UCHexCell* cell)
{
    int directionIndex = (int)direction;
    
    m_Neighbors[directionIndex] = cell;
    int OppositeIndex = (int)HexDirectionExtensions::Opposite(direction);
    cell->m_Neighbors[OppositeIndex] = this;
    return;
}
void UCHexCell::SetLocalPos(FVector pos)
{
    m_LocalPosition = pos;
}
FVector UCHexCell::GetLocalPos()
{
    return m_LocalPosition;
}
int UCHexCell::GetElevation()
{
    return m_Elevation;
}
void UCHexCell::SetElevation(int elevation)
{
    m_Elevation = elevation;
    FVector position = m_LocalPosition;
    float elevationTmp = m_Elevation * HexMetrics::elevationStep;
    position.Z = elevationTmp;

    position.Z+= (HexMetrics::SampleNoise(position).Z * 2.0f - 1.0f) *
        HexMetrics::elevationPerturbStrength;
    //transform.localPosition = position;
    m_LocalPosition = position;

    //FVector uiPosition = uiRect.localPosition;
    //uiPosition.z = elevation * -HexMetrics.elevationStep;
    //uiRect.localPosition = uiPosition;
}

void UCHexCell::SetHexGridChunk(UHexGridChunk* chunkPtr)
{
    chunk = chunkPtr;
}

HexMap::HexEdgeType UCHexCell::GetEdgeType(HexMap::HexDirection direction)
{
    return HexMetrics::GetEdgeType(
        m_Elevation, m_Neighbors[(int)direction]->GetElevation()
    );
}

HexMap::HexEdgeType UCHexCell::GetEdgeType(UCHexCell* otherCell)
{
    return HexMetrics::GetEdgeType(
        m_Elevation, otherCell->GetElevation()
    );
}

void UCHexCell::Refresh()
{
    if (chunk) 
    {
        chunk->Refresh();
        for (int i = 0; i < m_Neighbors.Num(); i++) 
        {
            UCHexCell* neighbor = m_Neighbors[i];
            if (neighbor != nullptr && neighbor->GetChunk() != chunk) 
            {
                neighbor->GetChunk()->Refresh();
            }
        }
    }
}
