// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor/Zone/ZoneSpawner.h"

// Sets default values
AZoneSpawner::AZoneSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	SpawnArea = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	SpawnArea->SetupAttachment(RootComponent);

}

void AZoneSpawner::SpawnZone()
{
	if (ZoneActorClass)
	{
		FVector BoxLocation = SpawnArea->GetComponentLocation();
		FVector BoxExtent = SpawnArea->GetUnscaledBoxExtent();

		//박스 내 랜덤한 위치 생성
		FVector RandomLocation = BoxLocation + FMath::RandPointInBox(FBox(-BoxExtent,BoxExtent));

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(RandomLocation);
		SpawnTransform.SetRotation(FQuat::Identity);
		SpawnTransform.SetScale3D(FVector(30.0f, 30.0f, 30.0f));

		//액터 스폰
		AActor* SpawnedZone = GetWorld()->SpawnActor<AActor>(ZoneActorClass, SpawnTransform);

		if (SpawnedZone)
		{
			
		}

		Destroy();
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ZoneActorClass is not set!"));
	}
	
}

// Called when the game starts or when spawned
void AZoneSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnZone();
}


