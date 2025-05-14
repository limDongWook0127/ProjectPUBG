// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/AirplaneManager.h"

#include "GameInstance/BaseGameInstance.h"
#include "GameplayActor/Airplane/Airplane.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/LandscapeManager.h"

void UAirplaneManager::InitializeManager()
{
	if (SpawnedPlane)
	{
		SpawnedPlane->Destroy();
	}
	
	SpawnedPlane = nullptr;
}

void UAirplaneManager::NotifyStartToMoveAirplane()
{
	SpwanAirplane();
	StartToMoveAirplane();
}

void UAirplaneManager::SpwanAirplane()
{
	if (!PlaneClass) return;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(FVector(0, 0, 0));
	//SpawnTransform.SetRotation();
	SpawnTransform.SetScale3D(FVector(1, 1, 1));

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;  // 항상 스폰
	
	// 비행기 스폰
	SpawnedPlane = GetWorld()->SpawnActor<AAirplane>(PlaneClass, SpawnTransform, SpawnParams);
	SpawnedPlane->SetDuration(Duration);
}


void UAirplaneManager::StartToMoveAirplane()
{
	if (SpawnedPlane)
	{
		UBaseGameInstance* GI = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		if (GI)
		{			
			FVector StartPoint;
			FVector EndPoint;

			FBox Bounds = GI->GetLandscapeManager()->GetLandscapeBoundingBox();

			float ScaleFactor = 0.7f;
			float MaxX = Bounds.Max.X * ScaleFactor;
			float MaxY = Bounds.Max.Y * ScaleFactor;
			float MinX = Bounds.Min.X * ScaleFactor;
			float MinY = Bounds.Min.Y * ScaleFactor;
			float Z = Bounds.Max.Z * ScaleFactor + 10000.f;
			UE_LOG(LogTemp, Warning, TEXT("Z:%f"),Z);

			// 랜덤하게 모서리 점 선택
			TArray<FVector> StartPoints = {
				FVector(FMath::RandRange(MinX, MaxX), MaxY, Z),  // Top
				FVector(FMath::RandRange(MinX, MaxX), MinY, Z),  // Bottom
				FVector(MaxX,FMath::RandRange(MinY, MaxY), Z),  // Right
				FVector(MinX,FMath::RandRange(MinY, MaxY), Z)  //  Left
			};

			// 모서리 점 중 랜덤으로 시작점 하나를 고르기
			int32 RandomIndex = FMath::RandRange(0, StartPoints.Num() - 1);
			StartPoint = StartPoints[RandomIndex];
			EndPoint = StartPoints[(RandomIndex % 2 == 0) ? RandomIndex + 1 : RandomIndex - 1 ];
			
			SpawnedPlane->MovePlane(StartPoint, EndPoint);
		}		
	}
}
