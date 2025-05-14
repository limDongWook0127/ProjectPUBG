// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/LandscapeManager.h"
#include "Landscape.h"
#include "GameState/BaseGameState.h"
#include "Kismet/GameplayStatics.h"


ULandscapeManager::ULandscapeManager()
{
}

void ULandscapeManager::InitializeManager()
{
	CurrentLandscape = nullptr;
}

void ULandscapeManager::FindLandscape()
{
	if (UWorld* World = GetWorld())
	{
		// 월드에서 ALandscape 클래스를 가진 액터 찾기
		ALandscape* FoundLandscape = Cast<ALandscape>(UGameplayStatics::GetActorOfClass(World, ALandscape::StaticClass()));

		if (FoundLandscape)
		{
			SetLandscape(FoundLandscape);			
			UE_LOG(LogTemp, Warning, TEXT("월드에서 랜드스케이프를 찾아 설정했습니다: %s"), *FoundLandscape->GetName());

			
			UE_LOG(LogTemp, Warning, TEXT("GetComponentsBoundingBox()() :  %s"), *CurrentLandscape->GetComponentsBoundingBox().ToString());
			UE_LOG(LogTemp, Warning, TEXT("GetLandscapeSize() :  %s"), *GetLandscapeSize().ToString());
			UE_LOG(LogTemp, Warning, TEXT("GetRandomLocationOnLandscape() :  %s"), *GetRandomLocationOnLandscape().ToString());

			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("월드에서 랜드스케이프를 찾지 못했습니다."));
		}
	}
}

void ULandscapeManager::SetLandscape(ALandscape* Landscape)
{
	if (Landscape)
	{
		CurrentLandscape = Landscape;
		UE_LOG(LogTemp, Warning, TEXT("랜드스케이프가 저장됨: %s"), *CurrentLandscape->GetName());

		ABaseGameState* GameState = GetWorld()->GetGameState<ABaseGameState>();
		if (GameState && GameState->HasAuthority())
		{
			GameState->UpdateLandScapeBoundingBoxXY(GetLandscapeBoundingBox()); // 비행기 탑승 알림 설정
		}
	}
}

FBox ULandscapeManager::GetLandscapeBoundingBox() const
{
	if (!CurrentLandscape) return FBox(0, 0);

	FBox Bounds = CurrentLandscape->GetComponentsBoundingBox();

	return Bounds;
}

FVector2D ULandscapeManager::GetLandscapeSize() const
{
	if (!CurrentLandscape) return FVector2D(0, 0);

	FBox Bounds = CurrentLandscape->GetComponentsBoundingBox();
	
	return FVector2D(Bounds.Max.X - Bounds.Min.X, Bounds.Max.Y - Bounds.Min.Y);
}

FVector ULandscapeManager::GetRandomLocationOnLandscape()
{
	if (!CurrentLandscape) return FVector::ZeroVector;

	// 랜드스케이프 바운딩 박스 가져오기
	FBox Bounds = CurrentLandscape->GetComponentsBoundingBox();

	// X, Y 범위에서 랜덤 값 생성
	float RandomX = FMath::RandRange(Bounds.Min.X, Bounds.Max.X);
	float RandomY = FMath::RandRange(Bounds.Min.Y, Bounds.Max.Y);

	// 기본 Z 값 설정 (높이 보정 없이 반환)
	return FVector(RandomX, RandomY, Bounds.Min.Z);
}
