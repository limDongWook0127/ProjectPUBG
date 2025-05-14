// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Weapon/Weapon_Base.h"
#include "Grenade_Base.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API AGrenade_Base : public AWeapon_Base
{
	GENERATED_BODY()

public:
	AGrenade_Base();
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Grenade")
	void SetHitCollisionActivate();
	
	void SetHitCollisionDeActivate();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* GrenadeSphereCollision;

	UPROPERTY(EditAnywhere, Replicated)
	APlayerCharacter* OwningPlayerCharacter;
	
	FTimerHandle GrenadeTimer;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void SettingThrowable();
	
private:
	UPROPERTY()
	FTimerHandle CollisionSettingTimerHandle;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							 const FHitResult& SweepResult);
	
};

