// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile/Projectile_Base.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "Weapon/Component/BulletSystemComponent.h"

// Sets default values
AProjectile_Base::AProjectile_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 프로젝타일 메쉬 설정
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;

	// bulletSystemComponent
	BulletSystemComponent = CreateDefaultSubobject<UBulletSystemComponent>(TEXT("BulletSystemComponent"));
	
}

// Called when the game starts or when spawned
void AProjectile_Base::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile_Base::MyFunction_Implementation(AActor* TargetActor, FGameplayTag EventTag, FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("MyFunction called in C++!"));
}
