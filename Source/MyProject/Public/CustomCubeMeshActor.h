// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"


#include "CustomCubeMeshActor.generated.h"

UCLASS()
class MYPROJECT_API ACustomCubeMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomCubeMeshActor();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		UProceduralMeshComponent* ProcMesh;

	// 生成自定义几何体的函数
	void GenerateCustomGeometry();
	void AddQuadTriangles(TArray<int32>& Triangles, int32 V0, int32 V1, int32 V2, int32 V3);
	//UProceduralMeshHelper
	void CreateConeMeshByRadius(FVector PeakPoint, FVector Forward, float Height, float Radius, int32 SectionNum, TArray<FVector>& Out_Vertices, TArray<int32>& Out_Triangles);

	void CreateHexGrid();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
