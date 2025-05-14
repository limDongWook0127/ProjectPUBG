// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/GameEventManager.h"

#include "GameInstance/BaseGameInstance.h"
#include "GameplayActor/RedZone/RedZone.h"
#include "GameplayActor/SupplyDrop/SupplyDrop.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/LandscapeManager.h"

void UGameEventManager::InitializeManager()
{
	GetWorld()->GetTimerManager().ClearTimer(SupplyDropTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(RedZoneTimerHandle);
	SpawnHeight = 1000.f;
}

void UGameEventManager::StartGameEventTimer()
{
	StartSupplyDropTimer();
	StartRedZoneTimer();
}

void UGameEventManager::StartSupplyDropTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Supply drop event timer started."));

	// 랜덤한 시간 간격으로 설정
	float RandomTime = FMath::RandRange(SupplyDropTimeRange.X, SupplyDropTimeRange.Y);

	// 일정 시간이 지나면 OnSupplyDrop 실행
	GetWorld()->GetTimerManager().SetTimer(
		SupplyDropTimerHandle,
		this,
		&UGameEventManager::OnSupplyDrop,
		RandomTime,
		false // 반복 실행하지 않음 (이벤트 발생 후 다시 호출)
	);
}

void UGameEventManager::StartRedZoneTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Red Zone event timer started."));

	// 랜덤한 시간 간격으로 설정
	float RandomTime = FMath::RandRange(RedZoneTimeRange.X, RedZoneTimeRange.Y);

	// 일정 시간이 지나면 OnSupplyDrop 실행
	GetWorld()->GetTimerManager().SetTimer(
		RedZoneTimerHandle,
		this,
		&UGameEventManager::OnRedZone,
		RandomTime,
		false // 반복 실행하지 않음 (이벤트 발생 후 다시 호출)
	);
}

void UGameEventManager::OnSupplyDrop()
{
	SpawnSupplyDrop();

	// 새로운 랜덤 시간 후 다시 이벤트 실행
	StartSupplyDropTimer();
}

void UGameEventManager::SpawnSupplyDrop()
{
	UBaseGameInstance* GI = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// FActorSpawnParameters SpawnParameters;
	// SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (GI)
	{
		FVector RandomPosition = GI->GetLandscapeManager()->GetRandomLocationOnLandscape();
		RandomPosition.Z += SpawnHeight;

		// 보급품 스폰
		ASupplyDrop* SpawnedActor = GetWorld()->SpawnActor<ASupplyDrop>(SupplyDropClass, RandomPosition, FRotator::ZeroRotator);
		SpawnedActor->SpawnSupplyDropItem();
		if (SpawnedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Supply drop spawned at: %s"), *RandomPosition.ToString());
		}
	}
}

void UGameEventManager::OnRedZone()
{
	SpawnRedZone();
	StartRedZoneTimer();
}

void UGameEventManager::SpawnRedZone()
{
	UBaseGameInstance* GI = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GI)
	{
		FVector RandomPosition = GI->GetLandscapeManager()->GetRandomLocationOnLandscape();
		RandomPosition.Z += SpawnHeight;

		// 보급품 스폰
		ARedZone* SpawnedActor = GetWorld()->SpawnActor<ARedZone>(RedZoneClass, RandomPosition, FRotator::ZeroRotator);
		
		if (SpawnedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("ARedZone spawned at: %s"), *RandomPosition.ToString());
			SpawnedActor->StartExplosions();
		}
	}
}
