// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "BulletSystemComponent.generated.h"

class APlayerCharacter;
class AProjectile_Base;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUBG_API UBulletSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBulletSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	float Gravity;

	UPROPERTY()	
	FVector BulletVelocity;

	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector EndLocation;

	UPROPERTY()
	bool IsBulletInitialized;

	UPROPERTY()
	float DeltaSeconds;

	UPROPERTY()
	float AirDensity;

	UPROPERTY()
	AProjectile_Base* Projectile;
	
	FTimerHandle TimerHandle;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintPure, category = BulletSystem)
	float GetDragForce(float Air_Density, float Bullet_Speed, float Cross_Section, float Drag_Coefficiency);

	UFUNCTION(BlueprintPure, category = BulletSystem)
	FVector CalculateGravity(FVector& Velocity ,float DeltaSecond, float Air_Density);

	UFUNCTION(BlueprintCallable, category = BulletSystem)
	void Spawn_ShootBullet(FVector& Velocity, float Bullet_Speed);

	UFUNCTION(BlueprintCallable, category = BulletSystem)
	void SpawnLineTrace_HitResult(float DeltaSecond);
	
	void DestoryBullet();

	UFUNCTION()
	float FindHitLocation_Multiplier(FName _HitBodyName);

	UFUNCTION()
	float FindShootingGunType_multiplier(FName _HitBodyName);
	
	UPROPERTY()
	FGameplayTag _EventTag;
		
};
