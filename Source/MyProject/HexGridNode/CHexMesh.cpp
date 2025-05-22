// Fill out your copyright notice in the Description page of Project Settings.


#include "CHexMesh.h"
#include "CHexCell.h"
#include "HexMetrics.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
ACHexMesh::ACHexMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
	RootComponent = m_ProcMesh;
	
	m_BaseColor = FLinearColor(0.6, 0.6, 0.0, 0.8);
	m_NoiseVertexTexture = HexMetrics::LoadTexture2D(TEXT("E:/uedemo/MyProject/Content/Textures/Noise.png"));
}

// Called when the game starts or when spawned
void ACHexMesh::BeginPlay()
{
	Super::BeginPlay();
	
}
#include "DrawDebugHelpers.h"
void ACHexMesh::GenMesh()
{
	if (m_ProcMesh)
	{
		m_ProcMesh->ClearAllMeshSections();
	}
	//m_ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
	//RootComponent = m_ProcMesh;

	TArray<FProcMeshTangent> Tangents;
	TArray<FVector2D> UVs;
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(
		m_Vertices,
		m_Triangles,
		UVs,
		m_Normals,
		Tangents
	);
	//for (int32 i = 0; i < m_Normals.Num(); i++)
	//{
	//	Swap(m_Normals[i].Y, m_Normals[i].Z);
	//}
	//for (int i = 0; i < Normals.Num(); i++)
	//{
	//	auto tmp = -Normals[i];
	//	Normals[i] = tmp;
	//}
	// 生成网格（不计算法线和切线）
	m_ProcMesh->CreateMeshSection(
		0,          // Section Index
		m_Vertices,    // 顶点
		m_Triangles,   // 三角形索引
		m_Normals,   // 法线（留空自动计算）
		UVs,         // UV
		m_Colors,    // 顶点颜色
		Tangents, // 切线（留空自动计算）
		true       // 是否启用碰撞
	);
	m_ProcMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	m_ProcMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_ProcMesh->SetCollisionProfileName(TEXT("BlockAll"));
	// 调试法线
#if 0
	FlushPersistentDebugLines(GetWorld());
	for (int32 i=0;i< m_Vertices.Num();i++)
	{
		//if (i > 1000)
		//{
		//	break;
		//}
		FVector Start = m_Vertices[i];          // 箭头起点（世界坐标）
		FVector End = m_Normals[i]*3+ Start;    // 箭头终点（示例：X轴方向延伸100单位）
		float ArrowHeadSize = 0.1;                 // 箭头头部尺寸
		FColor Color = FColor::Red;                // 箭头颜色
		bool bPersistent = true;                    // 是否持续显示（false=仅一帧）
		float LifeTime = 100000.0f;                       // 显示时长（0表示瞬时）
		uint8 DepthPriority = 0;                     // 深度优先级（0为默认）
		float Thickness = 0.1;                      // 线宽

		DrawDebugDirectionalArrow(
			GetWorld(), Start, End, ArrowHeadSize,
			Color, bPersistent, LifeTime, DepthPriority, Thickness
		);
	}
#endif
	//DrawDirectionalArrow()
	
	//m_ProcMesh->SetRenderCustomDepth(true);
	//m_ProcMesh->SetCustomDepthStencilValue(1); // 模板值对应材质中的判断
	//ProcMesh->MarkRenderStateDirty();
	// 设置材质（需提前引用材质资源）

	if (!m_BaseMaterial)
	{
		m_BaseMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Materials/M_GridCell"));
	}
	m_ProcMesh->SetMaterial(0, m_BaseMaterial); // 第一个参数是 SectionIndex
	//// 创建动态材质实例
	//m_DynamicMaterial = m_ProcMesh->CreateDynamicMaterialInstance(
	//	0,                          // SectionIndex
	//	materials                  // 基础材质
	//);
	//m_DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), m_BaseColor);
}

// Called every frame
void ACHexMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACHexMesh::Triangulate(const TArray<class UCHexCell*>& cells)
{
	//hexMesh.Clear();
	m_Vertices.Empty();
	m_Triangles.Empty();
	m_Colors.Empty();
	m_Normals.Empty();
	for(int i = 0; i < cells.Num(); i++) 
	{
		Triangulate(cells[i]);
	}

}


void ACHexMesh::AddTriangle(FVector v1, FVector v2, FVector v3)
{
	int vertexIndex = m_Vertices.Num();
	m_Vertices.Add(Perturb(v1));
	m_Vertices.Add(Perturb(v2));
	m_Vertices.Add(Perturb(v3));
	m_Triangles.Add(vertexIndex);
	m_Triangles.Add(vertexIndex + 1);
	m_Triangles.Add(vertexIndex + 2);
}

void ACHexMesh::Triangulate(UCHexCell* cell)
{
	int dIndex = (int)HexMap::NE;
	for (dIndex = HexMap::NE; dIndex <= HexMap::NW; dIndex++)
	{
		Triangulate((HexMap::HexDirection)dIndex, cell);
	}
}

void ACHexMesh::Triangulate(HexMap::HexDirection direction, UCHexCell* cell)
{
	FVector center = cell->GetLocalPos();

	EdgeVertices e =  EdgeVertices(
		center + HexMetrics::GetFirstSolidCorner(direction),
		center + HexMetrics::GetSecondSolidCorner(direction)
	);


	TriangulateEdgeFan(center, e, cell->GetColor());

	if (direction <= HexMap::HexDirection::SE) {
		TriangulateConnection(direction, cell, e);
	}
}

void ACHexMesh::AddTriangleColor(FColor c1, FColor c2, FColor c3)
{
	m_Colors.Add(c1);
	m_Colors.Add(c2);
	m_Colors.Add(c3);
}
void ACHexMesh::AddTriangleColor(FColor color)
{
	m_Colors.Add(color);
	m_Colors.Add(color);
	m_Colors.Add(color);
}
void ACHexMesh::UpdateMeshSection()
{
	GenMesh();

	//TArray<FVector> Normals;
	//TArray<FProcMeshTangent> Tangents;
	//TArray<FVector2D> UVs;
	//UKismetProceduralMeshLibrary::CalculateTangentsForMesh(
	//	m_Vertices,
	//	m_Triangles,
	//	UVs,
	//	Normals,
	//	Tangents
	//);

	//// 生成网格（不计算法线和切线）
	//m_ProcMesh->UpdateMeshSection(
	//	0,          // Section Index
	//	m_Vertices,    // 顶点
	//	Normals,   // 法线（留空自动计算）
	//	UVs,         // UV
	//	m_Colors,    // 顶点颜色
	//	Tangents // 切线（留空自动计算）
	//);

}
void ACHexMesh::AddQuad(FVector v1, FVector v2, FVector v3, FVector  v4) 
{
	int vertexIndex = m_Vertices.Num();
	m_Vertices.Add(Perturb(v1));
	m_Vertices.Add(Perturb(v2));
	m_Vertices.Add(Perturb(v3));
	m_Vertices.Add(Perturb(v4));
	m_Triangles.Add(vertexIndex);
	m_Triangles.Add(vertexIndex + 2);
	m_Triangles.Add(vertexIndex + 1);
	m_Triangles.Add(vertexIndex + 1);
	m_Triangles.Add(vertexIndex + 2);
	m_Triangles.Add(vertexIndex + 3);
}

void ACHexMesh::AddQuadColor(FColor c1, FColor c2)
{
	m_Colors.Add(c1);
	m_Colors.Add(c1);
	m_Colors.Add(c2);
	m_Colors.Add(c2);
}

void ACHexMesh::AddQuadColor(FColor c1, FColor c2, FColor c3, FColor c4)
{
	m_Colors.Add(c1);
	m_Colors.Add(c2);
	m_Colors.Add(c3);
	m_Colors.Add(c4);
}
void ACHexMesh::TriangulateConnection(HexMap::HexDirection direction, UCHexCell* cell, EdgeVertices e1)
{
	UCHexCell* neighbor = cell->GetNeighbor(direction);
	if (neighbor == nullptr) 
	{
		return;
	}
	FVector bridge = HexMetrics::GetBridge(direction);
	//FVector v3 = v1 + bridge;
	//FVector v4 = v2 + bridge;
	//v3.Z = v4.Z = neighbor->GetLocalPos().Z;
	bridge.Z = neighbor->GetLocalPos().Z - cell->GetLocalPos().Z;
	EdgeVertices e2 =  EdgeVertices(
		e1.v1 + bridge,
		e1.v4 + bridge
	);

	//AddQuad(v1, v2, v3, v4);
	//AddQuadColor(cell->GetColor(), neighbor->GetColor());
	//TriangulateEdgeTerraces(v1, v2, cell, v3, v4, neighbor);
	if (cell->GetEdgeType(direction) == HexMap::HexEdgeType::Slope) 
	{
		TriangulateEdgeTerraces(e1, cell, e2, neighbor);
	}
	else 
	{
		TriangulateEdgeStrip(e1, cell->GetColor(), e2, neighbor->GetColor());
	}

	UCHexCell*nextNeighbor = cell->GetNeighbor(HexDirectionExtensions::Next(direction));
	if (direction <= HexMap::E && nextNeighbor != nullptr)
	{
		FVector  v5 = e1.v4 + HexMetrics::GetBridge(HexDirectionExtensions::Next(direction));
		//v5.Z = nextNeighbor->GetElevation() * HexMetrics::elevationStep;
		v5.Z = nextNeighbor->GetLocalPos().Z;
		//AddTriangle(v2, v4, v5);
		//AddTriangleColor(cell->GetColor(), neighbor->GetColor(), nextNeighbor->GetColor());
		if (cell->GetElevation() <= neighbor->GetElevation()) {
			if (cell->GetElevation() <= nextNeighbor->GetElevation()) {
				TriangulateCorner(e1.v4, cell, e2.v4, neighbor, v5, nextNeighbor);
			}
			else {
				TriangulateCorner(v5, nextNeighbor, e1.v4, cell, e2.v4, neighbor);
			}
		}
		else if (neighbor->GetElevation() <= nextNeighbor->GetElevation()) {
			TriangulateCorner(e2.v4, neighbor, v5, nextNeighbor, e1.v4, cell);
		}
		else {
			TriangulateCorner(v5, nextNeighbor, e1.v4, cell, e2.v4, neighbor);
		}
	}
}

void ACHexMesh::TriangulateCornerTerraces(
	FVector  begin, UCHexCell* beginCell,
	FVector left, UCHexCell* leftCell,
	FVector right, UCHexCell* rightCell
) {
	FVector v3 = HexMetrics::TerraceLerp(begin, left, 1);
	FVector v4 = HexMetrics::TerraceLerp(begin, right, 1);
	FColor c3 = HexMetrics::TerraceLerp(beginCell->GetColor(), leftCell->GetColor(), 1);
	FColor c4 = HexMetrics::TerraceLerp(beginCell->GetColor(), rightCell->GetColor(), 1);

	AddTriangle(begin, v3, v4);
	AddTriangleColor(beginCell->GetColor(), c3, c4);

	for (int i = 2; i < HexMetrics::terraceSteps; i++) {
		FVector  v1 = v3;
		FVector v2 = v4;
		FColor c1 = c3;
		FColor c2 = c4;
		v3 = HexMetrics::TerraceLerp(begin, left, i);
		v4 = HexMetrics::TerraceLerp(begin, right, i);
		c3 = HexMetrics::TerraceLerp(beginCell->GetColor(), leftCell->GetColor(), i);
		c4 = HexMetrics::TerraceLerp(beginCell->GetColor(), rightCell->GetColor(), i);
		AddQuad(v1, v2, v3, v4);
		AddQuadColor(c1, c2, c3, c4);
	}

	AddQuad(v3, v4, left, right);
	AddQuadColor(c3, c4, leftCell->GetColor(), rightCell->GetColor());
}

void ACHexMesh::TriangulateCornerTerracesCliff(FVector begin, UCHexCell* beginCell, FVector left, UCHexCell* leftCell, FVector right, UCHexCell* rightCell)
{
	float b = 1.0f / (rightCell->GetElevation() - beginCell->GetElevation());
	if (b < 0) {
		b = -b;
	}
	//FVector boundary; = FVector::LeLerp(begin, right, b);
	FVector boundary=FMath::Lerp(Perturb(begin), Perturb(right), b);
	
	//FColor boundaryColor = Color.Lerp(beginCell->GetColor(), rightCell->GetColor(), b);

	FColor boundaryColor = HexMetrics::ColorLerp(beginCell->GetColor(), rightCell->GetColor(), b);
	TriangulateBoundaryTriangle(
		begin, beginCell, left, leftCell, boundary, boundaryColor
	);

	if (leftCell->GetEdgeType(rightCell) == HexMap::HexEdgeType::Slope) {
		TriangulateBoundaryTriangle(
			left, leftCell, right, rightCell, boundary, boundaryColor
		);
	}
	else {
		AddTriangleUnperturbed(Perturb(left), Perturb(right), boundary);
		AddTriangleColor(leftCell->GetColor(), rightCell->GetColor(), boundaryColor);
	}
}


void ACHexMesh::TriangulateCornerCliffTerraces(FVector begin, UCHexCell* beginCell, FVector left, UCHexCell* leftCell, FVector right, UCHexCell* rightCell)
{
	float b = 1.0f / (leftCell->GetElevation() - beginCell->GetElevation());
	if (b < 0) {
		b = -b;
	}
	FVector boundary = FMath::Lerp(Perturb(begin), Perturb(left), b);
	
	//FLinearColor s1= beginCell->GetColor();
	//FLinearColor s2 = leftCell->GetColor();
	//FVector4 tt = FMath::Lerp(FVector4(s1), FVector4(s2), b);

	FColor boundaryColor = HexMetrics::ColorLerp(beginCell->GetColor(), leftCell->GetColor(), b);

	TriangulateBoundaryTriangle(
		right, rightCell, begin, beginCell, boundary, boundaryColor
	);

	if (leftCell->GetEdgeType(rightCell) == HexMap::HexEdgeType::Slope) {
		TriangulateBoundaryTriangle(
			left, leftCell, right, rightCell, boundary, boundaryColor
		);
	}
	else {
		AddTriangleUnperturbed(Perturb(left), Perturb(right), boundary);
		AddTriangleColor(leftCell->GetColor(), rightCell->GetColor(), boundaryColor);
	}
}

void ACHexMesh::TriangulateCorner(
	FVector bottom, UCHexCell *bottomCell,
	FVector left, UCHexCell* leftCell,
	FVector right, UCHexCell* rightCell
) {
	HexMap::HexEdgeType leftEdgeType = bottomCell->GetEdgeType(leftCell);
	HexMap::HexEdgeType rightEdgeType = bottomCell->GetEdgeType(rightCell);


	if (leftEdgeType == HexMap::HexEdgeType::Slope) {
		if (rightEdgeType == HexMap::HexEdgeType::Slope) {
			TriangulateCornerTerraces(
				bottom, bottomCell, left, leftCell, right, rightCell
			);
		}
		else if (rightEdgeType == HexMap::HexEdgeType::Flat) {
			TriangulateCornerTerraces(
				left, leftCell, right, rightCell, bottom, bottomCell
			);
		}
		else {
			TriangulateCornerTerracesCliff(
				bottom, bottomCell, left, leftCell, right, rightCell
			);
		}
	}
	else if (rightEdgeType == HexMap::HexEdgeType::Slope) {
		if (leftEdgeType == HexMap::HexEdgeType::Flat) {
			TriangulateCornerTerraces(
				right, rightCell, bottom, bottomCell, left, leftCell
			);
		}
		else {
			TriangulateCornerCliffTerraces(
				bottom, bottomCell, left, leftCell, right, rightCell
			);
		}
	}
	else if (leftCell->GetEdgeType(rightCell) == HexMap::HexEdgeType::Slope) {
		if (leftCell->GetElevation() < rightCell->GetElevation()) {
			TriangulateCornerCliffTerraces(
				right, rightCell, bottom, bottomCell, left, leftCell
			);
		}
		else {
			TriangulateCornerTerracesCliff(
				left, leftCell, right, rightCell, bottom, bottomCell
			);
		}
	}
	else {
		AddTriangle(bottom, left, right);
		AddTriangleColor(bottomCell->GetColor(), leftCell->GetColor(), rightCell->GetColor());
	}
}
void ACHexMesh::TriangulateBoundaryTriangle(
	FVector begin, UCHexCell *beginCell,
	FVector left, UCHexCell* leftCell,
	FVector boundary, FColor boundaryColor
) {
	FVector v2 = Perturb(HexMetrics::TerraceLerp(begin, left, 1));
	FColor c2 = HexMetrics::TerraceLerp(beginCell->GetColor(), leftCell->GetColor(), 1);

	AddTriangleUnperturbed(Perturb(begin), v2, boundary);
	AddTriangleColor(beginCell->GetColor(), c2, boundaryColor);

	for (int i = 2; i < HexMetrics::terraceSteps; i++) {
		FVector v1 = v2;
		FColor c1 = c2;
		v2 = Perturb(HexMetrics::TerraceLerp(begin, left, i));
		c2 = HexMetrics::TerraceLerp(beginCell->GetColor(), leftCell->GetColor(), i);
		AddTriangleUnperturbed(v1, v2, boundary);
		AddTriangleColor(c1, c2, boundaryColor);
	}

	AddTriangleUnperturbed(v2, Perturb(left), boundary);
	AddTriangleColor(c2, leftCell->GetColor(), boundaryColor);
}

void ACHexMesh::TriangulateEdgeStrip(
	EdgeVertices e1, FColor c1,
	EdgeVertices e2, FColor c2
) {
	AddQuad(e1.v1, e1.v2, e2.v1, e2.v2);
	AddQuadColor(c1, c2);
	AddQuad(e1.v2, e1.v3, e2.v2, e2.v3);
	AddQuadColor(c1, c2);
	AddQuad(e1.v3, e1.v4, e2.v3, e2.v4);
	AddQuadColor(c1, c2);
}
void ACHexMesh::TriangulateEdgeTerraces(
	EdgeVertices begin, UCHexCell* beginCell,
	EdgeVertices end, UCHexCell* endCell
) 
{
	EdgeVertices e2 = EdgeVertices::TerraceLerp(begin, end, 1);
	FColor c2 = HexMetrics::TerraceLerp(beginCell->GetColor(), endCell->GetColor(), 1);

	TriangulateEdgeStrip(begin, beginCell->GetColor(), e2, c2);

	for (int i = 2; i < HexMetrics::terraceSteps; i++) {
		EdgeVertices e1 = e2;
		FColor c1 = c2;
		e2 = EdgeVertices::TerraceLerp(begin, end, i);
		c2 = HexMetrics::TerraceLerp(beginCell->GetColor(), endCell->GetColor(), i);
		TriangulateEdgeStrip(e1, c1, e2, c2);
	}

	TriangulateEdgeStrip(e2, c2, end, endCell->GetColor());
}


FVector ACHexMesh::Perturb(FVector position)
{
	FVector4 sample = HexMetrics::SampleNoise(position);
	position.X += (sample.X * 2.f - 1.f)* HexMetrics::cellPerturbStrength;
	position.Y += (sample.Y * 2.f - 1.f) * HexMetrics::cellPerturbStrength;
	// 不调整高度
	//position.Z += (sample.X * 2.f - 1.f) * HexMetrics::cellPerturbStrength;
	return position;
}

void ACHexMesh::TriangulateEdgeFan(FVector center, EdgeVertices edge, FColor color)
{
	AddTriangle(center, edge.v1, edge.v2);
	AddTriangleColor(color);
	AddTriangle(center, edge.v2, edge.v3);
	AddTriangleColor(color);
	AddTriangle(center, edge.v3, edge.v4);
	AddTriangleColor(color);
}

void ACHexMesh::AddTriangleUnperturbed(FVector  v1, FVector v2, FVector v3) {
	int vertexIndex = m_Vertices.Num();
	m_Vertices.Add(v1);
	m_Vertices.Add(v2);
	m_Vertices.Add(v3);
	m_Triangles.Add(vertexIndex);
	m_Triangles.Add(vertexIndex + 1);
	m_Triangles.Add(vertexIndex + 2);
}