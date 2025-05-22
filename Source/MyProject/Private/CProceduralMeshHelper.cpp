// Fill out your copyright notice in the Description page of Project Settings.


#include "CProceduralMeshHelper.h"
#include "GameFramework/Actor.h"
#include "KismetProceduralMeshLibrary.h"

CProceduralMeshHelper::CProceduralMeshHelper()
{
}

CProceduralMeshHelper::~CProceduralMeshHelper()
{
}


//扇形体
/*
*	创建扇形体（圆柱体）绘制信息;
*	@CenterPoint 扇形体（圆柱体）的中心点;
*	@Forward 朝向；
*	@Up 朝上方向；
*	@Angle 扇形角度，360表示为圆柱体;
*	@Radius 扇形半径;
*	@Height 扇形体（圆柱体）的高度
*	@SectionNum 扇形体（圆柱体）的分割面数。数值越大，越圆滑。
*	@Out_Vertices 输出的顶点；
*	@Out_Triangles 输出的三角形；
*/
void CProceduralMeshHelper::CreateSectorMesh(FVector CenterPoint, FVector Forward, FVector Up, float Angle, float Radius, float Height, int32 SectionNum, TArray<FVector>& Out_Vertices, TArray<int32>& Out_Triangles)
{
    //上下2片扇形，左右2个矩形，前面多个碎面;
    //1、算出所有的顶点;
    Forward.Normalize();
    Up.Normalize();
    Angle = Angle > 360.f ? 360.f : Angle;
    FVector lowPeakPoint = CenterPoint - Up * (Height / 2.0f);
    FVector highPeakPoint = CenterPoint + Up * (Height / 2.0f);
    TArray<FVector> lowPoints = CalSectorPoints(lowPeakPoint, Forward, Up, Angle, Radius, SectionNum);
    TArray<FVector> highPoints = CalSectorPoints(highPeakPoint, Forward, Up, Angle, Radius, SectionNum);
    Out_Vertices.Add(lowPeakPoint);
    Out_Vertices.Append(lowPoints);
    int32 LowStartIdx = 0;
    int32 LowEndIdx = Out_Vertices.Num() - 1;
    int32 HighStartIdx = Out_Vertices.Num();
    Out_Vertices.Add(highPeakPoint);
    Out_Vertices.Append(highPoints);
    int32 HighEndIdx = Out_Vertices.Num() - 1;
    //2.1 下面的三角形
    TArray<int32> tmplowTriangle = CircleTriangles(LowStartIdx, LowStartIdx + 1, LowEndIdx, false);
    Out_Triangles.Append(tmplowTriangle);
    //2.2 上面的三角形
    TArray<int32> tmphightTriangle = CircleTriangles(HighStartIdx, HighStartIdx + 1, HighEndIdx, true);
    Out_Triangles.Append(tmphightTriangle);
    //如果是圆柱形的话不需要生成侧面的矩形
    if (!FMath::IsNearlyEqual(Angle, 360.f))
    {
        TArray<int32> tmpleftRectangle = RectangleTriangles(LowStartIdx + 1, HighStartIdx + 1, LowStartIdx, HighStartIdx);
        Out_Triangles.Append(tmpleftRectangle);
        TArray<int32> tmprightRectangle = RectangleTriangles(LowStartIdx, HighStartIdx, LowEndIdx, HighEndIdx);
        Out_Triangles.Append(tmprightRectangle);
    }
    for (int32 idx = 1; idx < LowEndIdx; idx++)
    {
        int32 tmprightdown = idx;
        int32 tmpleftdown = idx + 1;
        int32 tmprighttop = HighStartIdx + tmprightdown;
        int32 tmplefttop = HighStartIdx + tmpleftdown;
        TArray<int32> tmpChips = RectangleTriangles(tmpleftdown, tmplefttop, tmprightdown, tmprighttop);
        Out_Triangles.Append(tmpChips);
    }
}

TArray<FVector> CProceduralMeshHelper::CalSectorPoints(FVector CenterPoint, FVector ForwardVector, FVector UpVector, float Angle, float Radius, int32 SectionNum)
{
    TArray<FVector> ResArray;
    ForwardVector.Normalize();
    UpVector.Normalize();
    float SectionAngle = SectionNum > 0 ? Angle / SectionNum : Angle;
    FVector SectionForward = ForwardVector.RotateAngleAxis(-1 * (Angle / 2), UpVector) * Radius;
    FVector leftpoint = CenterPoint + SectionForward;
    ResArray.Add(leftpoint);
    for (int32 idx = 0; idx < SectionNum; idx++)
    {
        SectionForward = SectionForward.RotateAngleAxis(SectionAngle, UpVector);
        FVector tmpPoint = CenterPoint + SectionForward;
        ResArray.Add(tmpPoint);
    }
    return ResArray;
}

TArray<int32> CProceduralMeshHelper::CircleTriangles(int32 CenterIdx, int32 StartIdx, int32 EndIdx, bool bAnticlockwise)
{
    TArray<int32> Triangles;
    //CenterIdx是圆的中心点下标
    for (int32 Idx = StartIdx; Idx < EndIdx; Idx++)
    {
        if (bAnticlockwise)
        {
            //逆时针构造;
            Triangles.Add(CenterIdx);
            Triangles.Add(Idx + 1);
            Triangles.Add(Idx);
        }
        else
        {
            //顺时针构造;
            Triangles.Add(CenterIdx);
            Triangles.Add(Idx);
            Triangles.Add(Idx + 1);
        }
    }
    return Triangles;
}


//矩形
TArray<int32> CProceduralMeshHelper::RectangleTriangles(int32 Leftdown, int32 Lefttop, int32 Rightdown, int32 Righttop)
{
    TArray<int32> Triangles;
    Triangles.Add(Rightdown);
    Triangles.Add(Righttop);
    Triangles.Add(Lefttop);
    Triangles.Add(Rightdown);
    Triangles.Add(Lefttop);
    Triangles.Add(Leftdown);
    return Triangles;
}


//长方体（正方体）
/*
*	创建长方体（正方体）绘制信息;
*	@CubeRadius 长方体（正方体）各个轴的半径;
*	@Out_Vertices 输出的顶点；
*	@Out_Triangles 输出的三角形；
*/
void CProceduralMeshHelper::CreateCubeMesh(FVector CubeRadius, TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
 
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    UKismetProceduralMeshLibrary::GenerateBoxMesh(CubeRadius, Vertices, Triangles, Normals, UVs, Tangents);
}

//球体
/*
*   创建球体绘制信息;
*   @CenterPoint 球体的中心点;
*   @Radius 球体半径;
*   @SectionNum 球体上每个圆的分割面数。数值越大，越圆滑。
*   @Out_Vertices 输出的顶点；
*   @Out_Triangles 输出的三角形；
*/
void CProceduralMeshHelper::CreateSphereMesh(FVector CenterPoint, float Radius, int32 SectionNum, TArray<FVector>& Out_Vertices, TArray<int32>& Out_Triangles)
{
    //垂直于X轴的平面上的圆
    TArray<FVector> XPoints = CalSectorPoints(CenterPoint, FVector::RightVector, FVector::ForwardVector, 360.f, Radius, SectionNum);
    //垂直于Y轴的平面上的圆
    TArray<FVector> YPoints = CalSectorPoints(CenterPoint, FVector::UpVector, FVector::RightVector, 360.f, Radius, SectionNum);
    //垂直于Z轴的平面上的圆
    TArray<FVector> ZPoints = CalSectorPoints(CenterPoint, FVector::ForwardVector, FVector::UpVector, 360.f, Radius, SectionNum);
    //中心点
    Out_Vertices.Add(CenterPoint);
    int32 CenterIdx = 0;
    //X轴
    int32 XStartIdx = Out_Vertices.Num();
    Out_Vertices.Append(XPoints);
    int32 XEndIdx = Out_Vertices.Num() - 1;
    //Y轴
    int32 YStartIdx = Out_Vertices.Num();
    Out_Vertices.Append(YPoints);
    int32 YEndIdx = Out_Vertices.Num() - 1;
    //Z轴
    int32 ZStartIdx = Out_Vertices.Num();
    Out_Vertices.Append(ZPoints);
    int32 ZEndIdx = Out_Vertices.Num() - 1;
    TArray<int32> XTriangles = CircleTriangles(CenterIdx, XStartIdx, XEndIdx, false);
    Out_Triangles.Append(XTriangles);
    TArray<int32> YTriangles = CircleTriangles(CenterIdx, YStartIdx, YEndIdx, false);
    Out_Triangles.Append(YTriangles);
    TArray<int32> ZTriangles = CircleTriangles(CenterIdx, ZStartIdx, ZEndIdx, false);
    Out_Triangles.Append(ZTriangles);
}