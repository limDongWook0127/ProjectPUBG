// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "DropSpawnVolume.generated.h"

class ALandscape;
class ASupplyDrop;

UCLASS()
class PUBG_API ADropSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropSpawnVolume();

	//스폰시킬BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<ASupplyDrop> BP_SupplyDrop;

private:
	//스폰지정구역
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* DropSpawnArea;

	//스폰타이머
	FTimerHandle SpawnTimerHandle;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	//랜드스케이프 랜덤 위치 지정
	FVector GetRandomLandscapeLocation();

	//드롭 위치 세팅
	void SetDropLocation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
