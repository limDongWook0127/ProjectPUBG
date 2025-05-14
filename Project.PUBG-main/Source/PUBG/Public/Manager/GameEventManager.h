// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameEventManager.generated.h"

class ARedZone;
class ASupplyDrop;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PUBG_API UGameEventManager : public UObject
{
	GENERATED_BODY()

public:
	void InitializeManager();

	
	void StartGameEventTimer();
	
	void StartSupplyDropTimer();
	void StartRedZoneTimer();
	
	void OnSupplyDrop();
    void SpawnSupplyDrop();
	void OnRedZone();
    void SpawnRedZone();

private:
	FTimerHandle SupplyDropTimerHandle;
	FTimerHandle RedZoneTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "GameEvent")
	TSubclassOf<ASupplyDrop> SupplyDropClass;  // 보급품 액터 클래스
	
	UPROPERTY(EditDefaultsOnly, Category = "GameEvent")
	TSubclassOf<ARedZone> RedZoneClass;  // 레드존 클래스
	
	UPROPERTY(EditDefaultsOnly, Category = "GameEvent")
	float SpawnHeight = 2000.0f;  // 보급품이 떨어질 높이

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector2D SupplyDropTimeRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector2D RedZoneTimeRange;
};



