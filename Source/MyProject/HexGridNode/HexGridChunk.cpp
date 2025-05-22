// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGridChunk.h"
#include "HexMetrics.h"

UHexGridChunk::UHexGridChunk()
{
}
void UHexGridChunk::GenMesh()
{
	m_HexMesh->Triangulate(m_Cells);
	m_HexMesh->GenMesh();
}

void UHexGridChunk::Init(UWorld* world)
{
	m_Cells.SetNum(HexMetrics::chunkSizeX * HexMetrics::chunkSizeY);
	if (world)
	{
		m_HexMesh = world->SpawnActor<ACHexMesh>(ACHexMesh::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	}
	
}
