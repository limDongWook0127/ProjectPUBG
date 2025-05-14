// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/SoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"


ASoundManager::ASoundManager()
{
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);
	
	bReplicates = true;
	bAlwaysRelevant = true;
	NetUpdateFrequency = 100.0f;
}

void ASoundManager::BeginDestroy()
{
	Super::BeginDestroy();

	UE_LOG(LogTemp, Warning, TEXT("ASoundManager::BeginDestroy"));
}

void ASoundManager::Server_PlaySound2D_Implementation(USoundBase* Sound, float VolumeMultiplier, float PitchMultiplier,
                                                      float StartTime, USoundConcurrency* ConcurrencySettings, const AActor* OwningActor, bool bIsUISound)
{
	UE_LOG(LogTemp, Display, TEXT("Server_PlaySound2D_Implementation"));
	Multicast_PlaySound2D(Sound, VolumeMultiplier, PitchMultiplier, StartTime, ConcurrencySettings, OwningActor, bIsUISound);
}

void ASoundManager::Multicast_PlaySound2D_Implementation(USoundBase* Sound, float VolumeMultiplier,
	float PitchMultiplier, float StartTime, USoundConcurrency* ConcurrencySettings, const AActor* OwningActor,
	bool bIsUISound)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->PlayerState)
	{
		int32 UniqueNetID = PC->PlayerState->GetUniqueID();
		UE_LOG(LogTemp, Warning, TEXT("My Unique Net ID: %d"), UniqueNetID);
	}
	
	if (Sound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), Sound, VolumeMultiplier, PitchMultiplier, StartTime, ConcurrencySettings, OwningActor, bIsUISound);
		
	}
}

void ASoundManager::Server_PlaySoundAtLocation_Implementation(USoundBase* Sound, FVector Location,
	float VolumeMultiplier, float PitchMultiplier, float StartTime, class USoundAttenuation* AttenuationSettings,
	USoundConcurrency* ConcurrencySettings, const UInitialActiveSoundParams* InitialParams)
{
	Multicast_PlaySoundAtLocation(Sound, Location, VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings, ConcurrencySettings, InitialParams);
}

void ASoundManager::Multicast_PlaySoundAtLocation_Implementation(USoundBase* Sound, FVector Location,
	float VolumeMultiplier, float PitchMultiplier, float StartTime, class USoundAttenuation* AttenuationSettings,
	USoundConcurrency* ConcurrencySettings, const UInitialActiveSoundParams* InitialParams)
{
	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location, VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings, ConcurrencySettings, InitialParams);
	}
}
