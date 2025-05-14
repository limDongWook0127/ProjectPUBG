// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor/ItemSpawn/DropSpawnVolume.h"
#include "GameplayActor/SupplyDrop/SupplyDrop.h"

// Sets default values
ADropSpawnVolume::ADropSpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	DropSpawnArea = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	DropSpawnArea->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADropSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ADropSpawnVolume::SetDropLocation, 2.0f, true);
}

void ADropSpawnVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	//AActor* SpawnedZone = GetWorld()->SpawnActor<AActor>(zoneActorClass, GetRandomLandscapeLocation());
}

FVector ADropSpawnVolume::GetRandomLandscapeLocation()
{
		if (!DropSpawnArea) //스폰시킬 위치
		{
			UE_LOG(LogTemp, Error, TEXT("DropSpawnArea is nullptr"));
			return FVector::ZeroVector;
		}

		//스폰 박스의 중심과 크기 가져오기
		FVector Origin = DropSpawnArea->GetComponentLocation();
		FVector BoxExtent = DropSpawnArea->GetScaledBoxExtent();  // Half-Extent 값
	
		float RandX = FMath::RandRange(Origin.X - BoxExtent.X, Origin.X + BoxExtent.X);
		float RandY = FMath::RandRange(Origin.Y - BoxExtent.Y, Origin.Y + BoxExtent.Y);

		//Z 위치는 박스의 중심 또는 일정 높이 추가
		float RandZ = Origin.Z + 1000.0f;

		return FVector(RandX, RandY, RandZ);

}

void ADropSpawnVolume::SetDropLocation()
{
	UWorld* World = GetWorld();

	//스폰
	ASupplyDrop* SupplyDrop = World->SpawnActor<ASupplyDrop>(BP_SupplyDrop,GetRandomLandscapeLocation(),FRotator::ZeroRotator);
}

// Called every frame
void ADropSpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

