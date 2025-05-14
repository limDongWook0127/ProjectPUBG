// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PUBG/Public/GameplayActor/ItemSpawn/ItemSpawnerComponent.h"
#include "AHouse.generated.h"

UCLASS()
class PUBG_API AAHouse : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAHouse();

	virtual void BeginPlay() override;

	//스폰존컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	UItemSpawnerComponent* ItemSpawnerComponent;

	//집 메쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "House")
	UStaticMeshComponent* HouseMesh;

};
