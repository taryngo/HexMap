// Fill out your copyright notice in the Description page of Project Settings.


#include "HexMetrics.h"
#include "IImageWrapper.h"  
#include "IImageWrapperModule.h"  

float HexMetrics::outerRadius = 10.0f;
float HexMetrics::innerRadius = outerRadius * 0.866025404f;

float HexMetrics::solidFactor = 0.8f;

float HexMetrics::blendFactor = 1.0f - solidFactor;
float HexMetrics::elevationStep = 5.0f;  // 阶梯步长

int HexMetrics::terracesPerSlope = 2;

int HexMetrics::terraceSteps = terracesPerSlope * 2 + 1;
float HexMetrics::horizontalTerraceStepSize = 1.0f / terraceSteps;
float HexMetrics::verticalTerraceStepSize = 1.0f / (terracesPerSlope + 1);
float HexMetrics::cellPerturbStrength = 2.0f;
float HexMetrics::noiseScale = 0.003f;
float HexMetrics::elevationPerturbStrength = 1.5f; // 扰动强度
UTexture2D *HexMetrics::m_PerturbNoise = nullptr;
int HexMetrics::chunkSizeX = 5;
int HexMetrics::chunkSizeY = 5;
//TArray<FVector> HexMetrics::corners = {
//			 FVector(0.f, 0.f, outerRadius),
//			 FVector(innerRadius, 0.f, 0.5f * outerRadius),
//			 FVector(innerRadius, 0.f, -0.5f * outerRadius),
//			 FVector(0.f, 0.f, -outerRadius),
//			 FVector(-innerRadius, 0.f, -0.5f * outerRadius),
//			 FVector(-innerRadius, 0.f, 0.5f * outerRadius),
//			 FVector(0.f, 0.f, outerRadius)
//};

TArray<FVector> HexMetrics::corners = {
			 FVector(0.f, outerRadius, 0.f),
			 FVector(innerRadius, 0.5f * outerRadius,  0.f),
			 FVector(innerRadius, -0.5f * outerRadius,  0.f),
			 FVector(0.f, -outerRadius,  0.f),
			 FVector(-innerRadius, -0.5f * outerRadius,  0.f),
			 FVector(-innerRadius, 0.5f * outerRadius, 0.f),
			 FVector(0.f, outerRadius,  0.f)
};


HexMetrics::HexMetrics()
{
}

HexMetrics::~HexMetrics()
{
}

FVector HexMetrics::TerraceLerp(FVector a, FVector b, int step) 
{
	float h = step * HexMetrics::horizontalTerraceStepSize;
	a.X += (b.X - a.X) * h;
	a.Y += (b.Y - a.Y) * h;
	float v = ((step + 1) / 2) * HexMetrics::verticalTerraceStepSize;
	a.Z += (b.Z - a.Z) * v;
	return a;
}

FColor HexMetrics::ColorLerp(FColor a, FColor b, float step)
{
	float h = step;
	//return FMath::Lerp(a, b, h);
	FLinearColor c1(a);
	FLinearColor c2(b);
	//FLinearColor ret =FLinearColor::LerpUsingHSV(c1, c2, step);
	float cMax = FMath::Max(c1.R, c2.R);
	float cMin = FMath::Min(c1.R, c2.R);

	float cr = FMath::Lerp(cMin, cMax, step);
	cMax = FMath::Max(c1.G, c2.G);
	cMin = FMath::Min(c1.G, c2.G);
	float cg = FMath::Lerp(cMin, cMax, step);
	cMax = FMath::Max(c1.B, c2.B);
	cMin = FMath::Min(c1.B, c2.B);
	float cb = FMath::Lerp(cMin, cMax, step);
	cMax = FMath::Max(c1.A, c2.A);
	cMin = FMath::Min(c1.A, c2.A);
	float ca = FMath::Lerp(cMin, cMax, step);
	FLinearColor ret(FMath::Abs(cr), FMath::Abs(cg), FMath::Abs(cb), FMath::Abs(ca));
	return ret.ToFColor(true);
}

UTexture2D* HexMetrics::LoadTexture2D(const FString ImagePath)
{
	UTexture2D* LoadTexture = nullptr;
	// 判断文件路径是否有效
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ImagePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Image url is not exist!"));
		return nullptr;
	}

	TArray<uint8> RawFileData;
	// 将图片文件转换成uint8数据
	if (FFileHelper::LoadFileToArray(RawFileData, *ImagePath))
	{
		// 获取图片的IImageWrapperPtr
		TSharedPtr<IImageWrapper> ImageWrapper = GetImageWrapperByExtention(ImagePath);
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
		{
			// 载入图片数据
			TArray<uint8> UncompressedBGRA;
			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
			{
				// 创建一个空白纹理
				LoadTexture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

				// 改写纹理数据
				void* TextureData = LoadTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
				LoadTexture->PlatformData->Mips[0].BulkData.Unlock();

				// 更新纹理数据
				LoadTexture->UpdateResource();
			}
		}
	}
	m_PerturbNoise = LoadTexture;
	return LoadTexture;

}

FVector4 HexMetrics::SampleNoise(FVector UVCoords)
{
#ifdef RENDERTARGET
	UTextureRenderTarget2D* RT = CreateRenderTargetCopy(m_PerturbNoise);
	FLinearColor pixel = GetPixelFromRenderTarget(RT, FVector2D(UVCoords.X, UVCoords.Y));
	FColor color = pixel.ToFColor(true);
	return FVector4(color);
#else
	return SampleNoisePrivate(UVCoords);
#endif // !1
}

FVector4 HexMetrics::SampleNoisePrivate( FVector UVCoords)
{
	UVCoords.X *=HexMetrics::noiseScale;
	UVCoords.Y *= HexMetrics::noiseScale;
	if (!m_PerturbNoise)
	{
		return FColor::Black;  // 纹理为空时返回黑色
	}

	// 获取纹理的MIP数据
	const FTexture2DMipMap& MipMap = m_PerturbNoise->GetPlatformMips()[0];

	// 获取纹理的宽度和高度
	const int32 Width = MipMap.SizeX;
	const int32 Height = MipMap.SizeY;

	// 将UV坐标转换为像素坐标
	int32 X = FMath::Clamp(FMath::FloorToInt(UVCoords.X * Width), 0, Width - 1);
	int32 Y = FMath::Clamp(FMath::FloorToInt(UVCoords.Y * Height), 0, Height - 1);

	// 获取纹理数据的指针
	const FColor* Data = (FColor*)MipMap.BulkData.LockReadOnly();

	// 获取每个像素的R, G, B, A值
	//int32 PixelIndex = (Y * Width + X) * 4;  // 假设每个像素有4个通道 (RGBA)
	//uint8 R = RawData[PixelIndex];
	//uint8 G = RawData[PixelIndex + 1];
	//uint8 B = RawData[PixelIndex + 2];
	//uint8 A = RawData[PixelIndex + 3];

	FColor Pixel = Data[Y * Width + X];

	MipMap.BulkData.Unlock();
	return FVector4(Pixel.ReinterpretAsLinear());
}

// 创建临时RenderTarget并复制纹理
UTextureRenderTarget2D* HexMetrics::CreateRenderTargetCopy(UTexture2D* SourceTexture)
{
	UTextureRenderTarget2D* RT = NewObject<UTextureRenderTarget2D>();
	RT->InitCustomFormat(SourceTexture->GetSizeX(), SourceTexture->GetSizeY(), PF_B8G8R8A8, false);
	RT->UpdateResourceImmediate(true);

	// 使用绘图指令复制
	ENQUEUE_RENDER_COMMAND(CopyTextureCommand)(
		[SourceTexture, RT](FRHICommandListImmediate& RHICmdList)
		{
			FRHITexture2D* SourceRHI = SourceTexture->GetResource()->TextureRHI->GetTexture2D();
			FRHITexture2D* DestRHI = RT->GetResource()->TextureRHI->GetTexture2D();

			RHICmdList.CopyTexture(
				SourceRHI,
				DestRHI,
				FRHICopyTextureInfo()
			);
		}
	);

	return RT;
}

// 从RenderTarget读取像素
FLinearColor HexMetrics::GetPixelFromRenderTarget(UTextureRenderTarget2D* RT, FVector2D UV)
{
	TArray<FColor> Pixels;
	FTextureRenderTargetResource* RTResource = RT->GameThread_GetRenderTargetResource();

	if (RTResource)
	{
		RTResource->ReadPixels(Pixels, FReadSurfaceDataFlags());

		const int32 X = FMath::FloorToInt(UV.X * (RT->SizeX - 1));
		const int32 Y = RT->SizeY - 1 - FMath::FloorToInt(UV.Y * (RT->SizeY - 1));

		return Pixels.IsValidIndex(Y * RT->SizeX + X) ?
			Pixels[Y * RT->SizeX + X].ReinterpretAsLinear() :
			FLinearColor::Black;
	}
	return FLinearColor::Black;
}

TSharedPtr<IImageWrapper> HexMetrics::GetImageWrapperByExtention(const FString ImagePath)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	if (ImagePath.EndsWith(".png"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	}
	else if (ImagePath.EndsWith(".jpg") || ImagePath.EndsWith(".jpeg"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	}
	else if (ImagePath.EndsWith(".bmp"))
	{
		return ImageWrapperModule.CreateImageWrapper(EImageFormat::BMP);
	}
	return nullptr;
}

 FColor HexMetrics::TerraceLerp(FColor a, FColor b, int step)
 {
	float h = step * HexMetrics::horizontalTerraceStepSize;
	//return FMath::Lerp(a, b, h);
	FLinearColor c1(a);
	FLinearColor c2(b);
	//FLinearColor ret =FLinearColor::LerpUsingHSV(c1, c2, step);
	float cMax = FMath::Max(c1.R, c2.R);
	float cMin = FMath::Min(c1.R, c2.R);

	float cr = FMath::Lerp(cMin, cMax, step);
	cMax = FMath::Max(c1.G, c2.G);
	cMin = FMath::Min(c1.G, c2.G);
	float cg = FMath::Lerp(cMin, cMax, step);
	cMax = FMath::Max(c1.B, c2.B);
	cMin = FMath::Min(c1.B, c2.B);
	float cb = FMath::Lerp(cMin, cMax, step);
	cMax = FMath::Max(c1.A, c2.A);
	cMin = FMath::Min(c1.A, c2.A);
	float ca = FMath::Lerp(cMin, cMax, step);
	FLinearColor ret(FMath::Abs(cr), FMath::Abs(cg), FMath::Abs(cb), FMath::Abs(ca));
	return ret.ToFColor(true);
}