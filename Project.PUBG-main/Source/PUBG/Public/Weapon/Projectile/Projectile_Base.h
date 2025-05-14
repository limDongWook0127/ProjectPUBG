// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/DataTable/DT_Weapon.h"
#include "Weapon/Guns/Gun_Base.h"
#include "Projectile_Base.generated.h"


class USphereComponent;
class UProjectileMovementComponent;
class APlayerCharacter;
class UBulletSystemComponent;

UCLASS()
class PUBG_API AProjectile_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditDefaultsOnly, category = Projectile)
	UBulletSystemComponent* BulletSystemComponent;

	UPROPERTY(EditDefaultsOnly, category = Projectile)
	AGun_Base* ShootingBulletOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	EGunType GunType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	float BulletDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	float BulletSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	float BulletMass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	float BulletCrossSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	float DragCoefficiency;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ExposeOnSpawn = "true"))
	FVector VelocityVector;

	UFUNCTION(BlueprintNativeEvent, Category = "MyCategory")
	void MyFunction(AActor* TargetActor, FGameplayTag EventTag, FGameplayEventData Payload);
};
