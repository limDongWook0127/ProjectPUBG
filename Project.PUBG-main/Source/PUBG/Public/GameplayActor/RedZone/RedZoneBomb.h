// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RedZoneBomb.generated.h"

class USphereComponent;
class UParticleSystem;
class UGameplayEffect;


UCLASS()
class PUBG_API ARedZoneBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARedZoneBomb();
	virtual void BeginPlay() override;
	
	void StartExplosion();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnExplosionEffect();
	
	// 오버랩 시작 시 호출될 함수
	UFUNCTION()
	void OnExplosionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	UPROPERTY(EditAnywhere, Category="Explosion", meta = (AllowPrivateAccess = "true"))
	float ExplosionRadius = 500.0f; // 폭발 반경

	// 콜리전 컴포넌트 (구체형 콜리전)
	UPROPERTY(VisibleAnywhere, Category="Explosion", meta = (AllowPrivateAccess = "true"))
	USphereComponent* ExplosionSphere;
	
	UPROPERTY(EditAnywhere, Category = "Explosion", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ParticleEffect;

	UPROPERTY(EditAnywhere, Category="Explosion", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
};
