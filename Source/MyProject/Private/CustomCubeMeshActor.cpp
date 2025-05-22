// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCubeMeshActor.h"
#include "KismetProceduralMeshLibrary.h"
#include "CProceduralMeshHelper.h"
// Sets default values
ACustomCubeMeshActor::ACustomCubeMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 创建ProceduralMeshComponent并设为根组件
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
	RootComponent = ProcMesh;
}

void ACustomCubeMeshActor::GenerateCustomGeometry()
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FColor> Colors;

   
    CProceduralMeshHelper::CreateCubeMesh(FVector(2,2,2), Vertices, Triangles);
    // 计算法线、UV和切线（示例简化）
    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVs, Normals, Tangents);
    
    // 创建网格段
    ProcMesh->CreateMeshSection(
        0,
        Vertices,
        Triangles,
        Normals,
        UVs,
        Colors,
        Tangents,
        true // 启用碰撞
    );

    // 设置材质（需提前创建材质资产）
    ProcMesh->SetMaterial(0, LoadObject<UMaterial>(nullptr, TEXT("/StarterContent/Materials/M_Metal_Burnished_Steel")));
}

//根据底面半径生成
/*
*   创建圆锥体绘制信息;
*   @PeakPoint 圆锥体顶点;
*   @Forward 圆锥方向
*   @Height 圆锥高度
*   @Radius 底面半径;
*   @SectionNum 球体上每个圆的分割面数。数值越大，越圆滑。
*   @Out_Vertices 输出的顶点；
*   @Out_Triangles 输出的三角形；
*/
void ACustomCubeMeshActor::CreateConeMeshByRadius(FVector PeakPoint, FVector Forward, float Height, float Radius, int32 SectionNum, TArray<FVector>& Out_Vertices, TArray<int32>& Out_Triangles)
{
    ////先计算底面的圆心
    //Forward.Normalize();
    //FVector CircleCenter = PeakPoint + Forward * Height;
    ////计算底面圆上的所有顶点
    //FMatrix NewMatrix = FRotationMatrix::MakeFromZ(Forward);
    //FVector CircleForward = NewMatrix.GetUnitAxis(EAxis::X);
    //TArray<FVector> CirclePoints = CalSectorPoints(CircleCenter, CircleForward, Forward, 360.f, Radius, SectionNum);
    //Out_Vertices.Add(CircleCenter);
    //Out_Vertices.Append(CirclePoints);
    //int32 CircleEndIdx = Out_Vertices.Num() - 1;
    //Out_Vertices.Add(PeakPoint);
    //​
    //    //底面圆的三角形
    //    TArray<int32> CircleTriangle = CircleTriangles(0, 1, CircleEndIdx, true);
    //Out_Triangles.Append(CircleTriangle);
    //​
    //    //侧面的三角形
    //    for (int32 Idx = 1; Idx < CircleEndIdx + 1; Idx++)
    //    {
    //        Out_Triangles.Add(CircleEndIdx + 1);
    //        Out_Triangles.Add(Idx);
    //        Out_Triangles.Add(Idx + 1);
    //    }
}

void ACustomCubeMeshActor::CreateHexGrid()
{

}

// 辅助函数：为四边形添加三角形索引
void ACustomCubeMeshActor::AddQuadTriangles(TArray<int32>& Triangles, int32 V0, int32 V1, int32 V2, int32 V3)
{
    Triangles.Add(V0); Triangles.Add(V1); Triangles.Add(V2);
    Triangles.Add(V0); Triangles.Add(V2); Triangles.Add(V3);
}

// Called when the game starts or when spawned
void ACustomCubeMeshActor::BeginPlay()
{
	Super::BeginPlay();
    GenerateCustomGeometry();

    //UWorld* World = GetWorld();
    //FVector pos(0.0, 0.0, 0.0);
    //auto grid = World->SpawnActor<ACHexGrid>(pos, FRotator::ZeroRotator);
    //grid->CreatePrefab();
    //grid->GetRootComponent()->AttachTo(GetRootComponent());
}

// Called every frame
void ACustomCubeMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

