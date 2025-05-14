// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/BaseGameInstance.h"

#include "Manager/AirplaneManager.h"
#include "Manager/DataTableManager.h"
#include "Manager/GameEventManager.h"
#include "Manager/LandscapeManager.h"
#include "Manager/SoundManager.h"
#include "Manager/TimeManager.h"
#include "Manager/ZoneManager.h"

UBaseGameInstance::UBaseGameInstance()
{
	TimeManager = nullptr;
}

void UBaseGameInstance::Init()
{
	Super::Init();
	
	// 시간 관리 매니저 초기화
	TimeManager = NewObject<UTimeManager>(this, TimeManagerClass);
	TimeManager->InitializeManager();
	LandscapeManager = NewObject<ULandscapeManager>(this, LandscapeManagerClass);
	LandscapeManager->InitializeManager();
	GameEventManager = NewObject<UGameEventManager>(this, GameEventManagerClass);
	GameEventManager->InitializeManager();
	ZoneManager = NewObject<UZoneManager>(this, ZoneManagerClass);
	ZoneManager->InitializeManager();
	AirplaneManager = NewObject<UAirplaneManager>(this, AirplaneManagerClass);
	AirplaneManager->InitializeManager();
	DataTableManager = NewObject<UDataTableManager>(this, DataTableManagerClass);
	DataTableManager->InitializeManager();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//SoundManager = GetWorld()->SpawnActor<ASoundManager>(ASoundManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (SoundManager)
	{
		UE_LOG(LogTemp, Display, TEXT("SoundManager Created"));
	}
}

void UBaseGameInstance::PlaySound2D(USoundBase* Sound, float VolumeMultiplier, float PitchMultiplier, float StartTime,
	USoundConcurrency* ConcurrencySettings, const AActor* OwningActor, bool bIsUISound) const
{
	UE_LOG(LogTemp, Display, TEXT("PlaySound2D"));

	if (SoundManager)
	{
		UE_LOG(LogTemp, Display, TEXT("PlaySound2D::SoundManager"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("PlaySound2D::SoundManager nullptr"));
	}
	
	if (SoundManager && Sound)
	{
		UE_LOG(LogTemp, Display, TEXT("PlaySound2DPlay"));
		SoundManager->Server_PlaySound2D(Sound, VolumeMultiplier, PitchMultiplier, StartTime, ConcurrencySettings, OwningActor, bIsUISound);
	}
}

void UBaseGameInstance::PlaySoundAtLocation(USoundBase* Sound, FVector Location, float VolumeMultiplier,
	float PitchMultiplier, float StartTime, class USoundAttenuation* AttenuationSettings,
	USoundConcurrency* ConcurrencySettings, const UInitialActiveSoundParams* InitialParams) const
{
	if (SoundManager && Sound)
	{
		SoundManager->Server_PlaySoundAtLocation(Sound, Location, VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings, ConcurrencySettings, InitialParams);
	}
}
