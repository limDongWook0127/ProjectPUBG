// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor/RedZone/RedZone.h"

#include "Components/SphereComponent.h"
#include "GameplayActor/RedZone/RedZoneBomb.h"
#include "GameState/BaseGameState.h"

// Sets default values
ARedZone::ARedZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 스피어 콜리전 생성 및 기본 설정
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
}

// Called when the game starts or when spawned
void ARedZone::BeginPlay()
{
	Super::BeginPlay();
}

void ARedZone::StartExplosions()
{
	UpdateRedZone(true);
	GetWorldTimerManager().SetTimer(ExplosionTimer, this, &ARedZone::SpawnExplosion, ExplosionInterval, true);
}

void ARedZone::UpdateRedZone(bool IsVisibility)
{
	if (HasAuthority())
	{
		FVector Location = GetActorLocation();
		float Scale = Radius;
		ABaseGameState* GameState = GetWorld()->GetGameState<ABaseGameState>();
		if (GameState && GameState->HasAuthority())
		{
			GameState->UpdateRedZone(Location, Scale, IsVisibility); 
		}
	}
}

void ARedZone::SpawnExplosion()
{
	if (ExplosionCount <= 0)
	{
		UpdateRedZone(false);
		GetWorldTimerManager().ClearTimer(ExplosionTimer);
		Destroy(); // 레드존 삭제
		return;
	}

	FVector RedZoneCenter = GetActorLocation();
	float RandX = FMath::RandRange(-Radius, Radius);
	float RandY = FMath::RandRange(-Radius, Radius);
	
	FVector ExplosionLocation = RedZoneCenter + FVector(RandX, RandY, 0);
	ExplosionLocation = FindGroundZ(GetWorld(), ExplosionLocation);
	
	ARedZoneBomb* SpawnActor = GetWorld()->SpawnActor<ARedZoneBomb>(RedZoneBomb,ExplosionLocation,FRotator::ZeroRotator);
	SpawnActor->StartExplosion();
	
	ExplosionCount--;
}

FVector ARedZone::FindGroundZ(UWorld* World, FVector StartLocation)
{
	FVector EndLocation = StartLocation - FVector(0, 0, 10000); // 아래로 레이 트레이스
	FHitResult HitResult;

	FCollisionQueryParams QueryParams;

	if (World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
	{
		return HitResult.ImpactPoint; // 충돌 지점 반환
	}

	return StartLocation; // 충돌 없으면 원래 높이 유지
}


