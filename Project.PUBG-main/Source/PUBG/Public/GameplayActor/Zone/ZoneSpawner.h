// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ZoneSpawner.generated.h"

UCLASS()
class PUBG_API AZoneSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZoneSpawner();
	
	UFUNCTION(BlueprintCallable,Category="Zone Spawner")
	void SpawnZone();

private:
	//자기장이 생성될 영역
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* SpawnArea;

	//스폰할 액터의 클래스
	UPROPERTY(EditAnywhere,Category = "Zone Spawner")
	TSubclassOf<AActor> ZoneActorClass;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
};
