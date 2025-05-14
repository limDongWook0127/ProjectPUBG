// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor/ItemSpawn/AHouse.h"

// Sets default values
AAHouse::AAHouse()
{
	PrimaryActorTick.bCanEverTick = false;
	
	//스폰컴포넌트
	ItemSpawnerComponent = CreateDefaultSubobject<UItemSpawnerComponent>(TEXT("ItemSpawnerComponent"));
}

void AAHouse::BeginPlay()
{
	Super::BeginPlay();
	
	if(HasAuthority())
	{		
		ItemSpawnerComponent->SpawnItems(false, this);
	}
}