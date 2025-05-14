// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "SupplyDrop.generated.h"

class UItemSpawnerComponent;
class AItemBase;
class UBoxComponent;

UCLASS()
class PUBG_API ASupplyDrop : public AActor , public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASupplyDrop();

	virtual void BeginPlay() override;	

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			   FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FName GetRandomItemRowName();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnSupplyDropItem();

	virtual FText LookAt() override;
	virtual UTexture2D* SetKeyTexture() override;

private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DropMesh;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects",meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects",meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ParticleEffect;
	
	FTimerHandle StopEffectHandle;

	//충돌체
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBoxComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* InteractionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupplyDropItem",meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AItemBase> ItemClass;

	UPROPERTY()
	AItemBase* SpawnedItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D* TabKeyTexture;

	//스폰컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner",meta = (AllowPrivateAccess = "true"))
	UItemSpawnerComponent* ItemSpawnerComponent;

	
};
