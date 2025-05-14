// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RedZone.generated.h"

class USphereComponent;
class ARedZoneBomb;

UCLASS()
class PUBG_API ARedZone : public AActor
{
	GENERATED_BODY()
	
public:    
	ARedZone();
	virtual void BeginPlay() override;

	void StartExplosions();
	void UpdateRedZone(bool IsVisibility);
	void SpawnExplosion();
	FVector FindGroundZ(UWorld* World, FVector StartLocation);


private:
	UPROPERTY(EditAnywhere, Category="Red Zone")
	float Radius = 1000.0f; // 레드존 반경

	UPROPERTY(EditAnywhere, Category="Red Zone")
	float ExplosionInterval = 1.0f; // 폭발 간격

	UPROPERTY(EditAnywhere, Category="Red Zone")
	int32 ExplosionCount = 10; // 폭발 개수
	
	UPROPERTY(EditAnywhere, Category="Red Zone")
	USphereComponent* SphereComponent; // 스폰할 위치를 정할 콜리전
	
	UPROPERTY(EditAnywhere, Category="Red Zone", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARedZoneBomb> RedZoneBomb; // 스폰할 액터

	FTimerHandle ExplosionTimer;
};
