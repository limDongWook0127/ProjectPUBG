// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ZoneManager.generated.h"

class AZone;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PUBG_API UZoneManager : public UObject
{
	GENERATED_BODY()

	
public:
	UZoneManager();
	
	void InitializeManager();
	
	/** 게임 시작 타이머 */
	void SpawnZone();

private:	
	//스폰할 액터의 클래스
	UPROPERTY(EditAnywhere,Category = "Zone Spawner")
	TSubclassOf<AZone> ZoneClass;
	
	UPROPERTY(EditAnywhere,Category = "Zone Spawner")
	AZone* SpawnedZone;

	UPROPERTY(EditAnywhere,Category = "Zone Spawner", meta = (AllowPrivateAccess = "true"))
	int ShirnkTime;
};
