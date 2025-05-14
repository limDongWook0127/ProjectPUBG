// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"

class ASoundManager;
class UAirplaneManager;
class UTimeManager;
class UGameEventManager;
class UZoneManager;
class ULandscapeManager;
class UDataTableManager;
;

/**
 * 
 */
UCLASS()
class PUBG_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UBaseGameInstance();
	
	virtual void Init() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UTimeManager* TimeManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	ULandscapeManager* LandscapeManager ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UGameEventManager* GameEventManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UZoneManager* ZoneManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UAirplaneManager* AirplaneManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UDataTableManager* DataTableManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	ASoundManager* SoundManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UTimeManager> TimeManagerClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ULandscapeManager> LandscapeManagerClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameEventManager> GameEventManagerClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UZoneManager> ZoneManagerClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAirplaneManager> AirplaneManagerClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDataTableManager> DataTableManagerClass;

public:
	UTimeManager* GetTimeManager() const { return TimeManager; }
	UGameEventManager* GetGameEventManager() const { return GameEventManager; }
	ULandscapeManager* GetLandscapeManager() const { return LandscapeManager; }
	UZoneManager* GetZoneManager() const { return ZoneManager; }
	UAirplaneManager* GetAirplaneManager() const { return AirplaneManager; }
	UDataTableManager* GetDataTableManager() const { return DataTableManager; }
	ASoundManager* GetSoundManager() const { return SoundManager; }

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlaySound2D(USoundBase* Sound, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f, float StartTime = 0.f,
		USoundConcurrency* ConcurrencySettings = nullptr, const AActor* OwningActor = nullptr, bool bIsUISound = true) const;
	
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlaySoundAtLocation(USoundBase* Sound, FVector Location,
		float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f, float StartTime = 0.f, class USoundAttenuation* AttenuationSettings = nullptr,
		USoundConcurrency* ConcurrencySettings = nullptr, const UInitialActiveSoundParams* InitialParams = nullptr) const;
};
