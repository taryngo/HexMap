// Fill out your copyright notice in the Description page of Project Settings.


#include "CHexGrid.h"
#include "CHexMesh.h"
#include "HexCoordinates.h"
#include "IronEthosPlayerController.h"
#include "IronEthosGameMode.h"
#include "../HexMapEditor/HexMapEditor.h"
// Sets default values
ACHexGrid::ACHexGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_cellCountX = chunkCountX * HexMetrics::chunkSizeX;
	m_cellCountY = chunkCountY * HexMetrics::chunkSizeY;

	//m_cells.SetNum(m_Width * m_Height);
}

// Called when the game starts or when spawned
void ACHexGrid::BeginPlay()
{
	Super::BeginPlay();
	CreateChunks();
	CreateCells();
}

void ACHexGrid::CreateCells()
{
	m_cells.SetNum(m_cellCountY * m_cellCountX);
	GenHexMapColor();

	InitGameModeAndController();
	
	FColor Basecolor = FColor::White;
	for (int z = 0, i = 0; z < m_cellCountY; z++)
	{
		for (int x = 0; x < m_cellCountX; x++)
		{
			int colorIndex = FMath::RandRange(1, 4);
			if (colorIndex == 1) { Basecolor = FColor::Yellow; }
			else if(colorIndex==2){ Basecolor = FColor::Green; }
			else if (colorIndex == 3) { Basecolor = FColor::Blue; }
			else if (colorIndex == 4) { Basecolor = FColor::White; }

			Basecolor = FColor::White;
			CreateCell(x, z, i++, Basecolor);
		}
	}
	GenMesh();
	
}

void ACHexGrid::OnHexMapColorCellDelegate(int type, bool state)
{
	if (state)
	{
		m_ColorSelectType = type;
	}

	m_HexMapGridColorMap[type] = state;

	TArray<int> selectColor;
	for (auto iter = m_HexMapGridColorMap.begin(); iter ; ++iter)
	{
		if (iter->Value== true)
		{
			selectColor.Add(iter->Key);
		}
	}
	for (int32_t i = 0; i < m_cells.Num(); i++)
	{
		auto cell = m_cells[i];
		if (cell)
		{
			int Key = 0;
			if (selectColor.Num()>0)
			{
				int randomSeq = 0;
				randomSeq = FMath::RandRange(0, selectColor.Num()-1);
				Key = selectColor[randomSeq];
			}

			FLinearColor baseColor;
			if (Key == 1) { baseColor = FLinearColor::Yellow; }
			else if (Key == 2) { baseColor = FLinearColor::Blue; }
			else if (Key == 3) { baseColor = FLinearColor::Green; }
			else if (Key == 4) { baseColor = FLinearColor::White; }
			else
			{
				baseColor = FLinearColor::White;
			}
			cell->SetCellBaseColor(baseColor);
		}
	}
}

void ACHexGrid::CreateCell(int x, int z, int i, FColor Basecolor)
{

	FVector pos(0.0,0.0,0.0);
	//ACHexCell* cell = GetWorld()->SpawnActor<ACHexCell>(ACHexCell::StaticClass(),pos, FRotator::ZeroRotator);
	UCHexCell* cell = NewObject<UCHexCell>();
	cell->CreateCell(x,z,i, m_GridCellMaterial, Basecolor);

	//cell->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	m_cells[i] = cell;
	auto coord = HexCoordinates::FromOffsetCoordinates(x,z);
	cell->SetCoordinates(coord);

	if (x > 0) 
	{
		cell->SetNeighbor(HexMap::W, m_cells[i - 1]);
	}
	if (z > 0) 
	{
		if ((z & 1) == 0) 
		{
			cell->SetNeighbor(HexMap::SE, m_cells[i - m_cellCountX]);
			if (x > 0) 
			{
				cell->SetNeighbor(HexMap::SW, m_cells[i - m_cellCountX - 1]);
			}
		}
		else 
		{
			cell->SetNeighbor(HexMap::SW, m_cells[i - m_cellCountX]);
			if (x < m_cellCountX - 1)
			{
				cell->SetNeighbor(HexMap::SE, m_cells[i - m_cellCountX + 1]);
			}
		}
	}
	AddCellToChunk(x, z, cell);
}

void ACHexGrid::GenHexMapColor()
{
	m_HexMapGridColorMap.Add(1, false);
	m_HexMapGridColorMap.Add(2, false);
	m_HexMapGridColorMap.Add(3, false);
	m_HexMapGridColorMap.Add(4, false);

	// 初始化基础颜色
	if (!m_GridCellMaterial)
	{
		m_GridCellMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Materials/M_GridCell"));
	}
}

void ACHexGrid::InitGameModeAndController()
{
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (!playerController)
	{
		return;
	}
	AIronEthosPlayerController* Controller = Cast<AIronEthosPlayerController>(playerController);
	if (Controller)
	{
		Controller->SetHexGrid(this);
	}

	m_MapEditor = NewObject<UHexMapEditor>();
	m_MapEditor->SetHexGrid(this);
	Controller->SetHexMapEditor(m_MapEditor);

	AGameModeBase* BaseGameMode = GetWorld()->GetAuthGameMode();
	if (BaseGameMode)
	{
		AIronEthosGameMode* GameMode = Cast<AIronEthosGameMode>(BaseGameMode);
		if (GameMode)
		{
			GameMode->SetHexGrid(this);
			GameMode->BindEditor(m_MapEditor);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to cast to GameMode!"));
		}
	}

}

void ACHexGrid::GenMesh()
{
	//m_CHexMesh = GetWorld()->SpawnActor<ACHexMesh>(ACHexMesh::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	//m_CHexMesh->Triangulate(m_cells);
	//m_CHexMesh->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	//m_CHexMesh->GenMesh();
}

void ACHexGrid::TouchCell(FVector  position) 
{
	//position = transform.InverseTransformPoint(position);
	HexCoordinates coordinates = HexCoordinates::FromPosition(position);
	int index = coordinates.GetX() + coordinates.GetZ() * m_cellCountX + coordinates.GetZ() / 2;
	UCHexCell* cell = m_cells[index];
	//cell.color = touchedColor;
	//hexMesh.Triangulate(cells);
}
UCHexCell* ACHexGrid::GetCell(FVector position)
{
	//position = transform.InverseTransformPoint(position);
	HexCoordinates coordinates = HexCoordinates::FromPosition(position);
	int index = coordinates.GetX() + coordinates.GetY() * m_cellCountX + coordinates.GetY() / 2;
	return m_cells[index];
}
void ACHexGrid::Refresh()
{
	m_CHexMesh->Triangulate(m_cells);
	m_CHexMesh->UpdateMeshSection();
}
void ACHexGrid::AddCellToChunk(int x, int y, UCHexCell* cell)
{
	int chunkX = x / HexMetrics::chunkSizeX;
	int chunkY = y / HexMetrics::chunkSizeY;
	UHexGridChunk* chunk = chunks[chunkX + chunkY * chunkCountX];

	int localX = x - chunkX * HexMetrics::chunkSizeX;
	int localZ = y - chunkY * HexMetrics::chunkSizeY;
	chunk->AddCell(localX + localZ * HexMetrics::chunkSizeX, cell);
}
void ACHexGrid::CreateChunks()
{
	//chunks.SetNum(chunkCountX * chunkCountY);

	for (int z = 0, i = 0; z < chunkCountY; z++) 
	{
		for (int x = 0; x < chunkCountX; x++) 
		{
			//UHexGridChunk *chunk = chunks[i++];
			UHexGridChunk* chunk =NewObject<UHexGridChunk>();
			chunk->Init(GetWorld());
			chunks.Add(chunk);
		}
	}
}
// Called every frame
void ACHexGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACHexGrid::ColorCell(FVector position, FColor color) 
{
	//position = transform.InverseTransformPoint(position);
	HexCoordinates coordinates = HexCoordinates::FromPosition(position);
	int index = coordinates.GetX() + coordinates.GetZ() * m_cellCountX + coordinates.GetZ() / 2;
	UCHexCell *cell = m_cells[index];
	//cell.color = color;
	cell->SetColor(color);
	m_CHexMesh->Triangulate(m_cells);
}