// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundManager.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API ASoundManager : public AActor
{
	GENERATED_BODY()

public:
	ASoundManager();

	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_PlaySound2D(USoundBase* Sound, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f, float StartTime = 0.f,
		USoundConcurrency* ConcurrencySettings = nullptr, const AActor* OwningActor = nullptr, bool bIsUISound = true);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySound2D(USoundBase* Sound, float VolumeMultiplier, float PitchMultiplier,
	float StartTime, USoundConcurrency* ConcurrencySettings, const AActor* OwningActor, bool bIsUISound);
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_PlaySoundAtLocation(USoundBase* Sound, FVector Location,
		float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f, float StartTime = 0.f, class USoundAttenuation* AttenuationSettings = nullptr,
		USoundConcurrency* ConcurrencySettings = nullptr, const UInitialActiveSoundParams* InitialParams = nullptr);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySoundAtLocation(USoundBase* Sound, FVector Location,
	float VolumeMultiplier, float PitchMultiplier, float StartTime, class USoundAttenuation* AttenuationSettings,
	USoundConcurrency* ConcurrencySettings, const UInitialActiveSoundParams* InitialParams);
};
