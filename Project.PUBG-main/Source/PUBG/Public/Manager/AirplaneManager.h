// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AirplaneManager.generated.h"

class AAirplane;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PUBG_API UAirplaneManager : public UObject
{
	GENERATED_BODY()
	
public:
	// 매니저 초기화
	void InitializeManager();
    // 비행기의 로직을 시작한다고 알림
	void NotifyStartToMoveAirplane();
    // 비행기 스폰
	void SpwanAirplane();
	// 비행기의 타임라인 시작
	void StartToMoveAirplane();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaneManager", meta = (AllowPrivateAccess = "true"))
	AAirplane* SpawnedPlane;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaneManager", meta = (AllowPrivateAccess = "true"))
	float Duration;
	
private:
	// 비행기 클래스 (Blueprint에서 설정 가능)
	UPROPERTY(EditAnywhere, Category = "PlaneManager")
	TSubclassOf<AAirplane> PlaneClass;	
};
