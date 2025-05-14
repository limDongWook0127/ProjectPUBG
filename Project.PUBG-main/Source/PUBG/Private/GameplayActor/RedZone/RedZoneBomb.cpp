// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor/RedZone/RedZoneBomb.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BaseGameplayTag.h"
#include "Character/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Controller/BasePlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARedZoneBomb::ARedZoneBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	

	ExplosionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionSphere"));
	RootComponent = ExplosionSphere;
	ExplosionSphere->InitSphereRadius(ExplosionRadius);
	ExplosionSphere->SetCollisionProfileName(TEXT("Trigger"));	
	
	bReplicates = true;
	//SetReplicates(true);
	ExplosionSphere->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void ARedZoneBomb::BeginPlay()
{
	Super::BeginPlay();

	// 오버랩 이벤트 바인딩
	ExplosionSphere->OnComponentBeginOverlap.AddDynamic(this, &ARedZoneBomb::OnExplosionOverlapBegin);
	
}

void ARedZoneBomb::StartExplosion()
{	
	MulticastSpawnExplosionEffect();	
	SetLifeSpan(1.0f);
}

void ARedZoneBomb::MulticastSpawnExplosionEffect_Implementation()
{	
	if (ParticleEffect)
	{
	// {
	// 	  UParticleSystemComponent* ParticleComp = UGameplayStatics::SpawnEmitterAttached(
	// 	  ParticleEffect,
	// 	  ExplosionSphere,       
	// 	  NAME_None,      
	// 	  FVector::ZeroVector, 
	// 	  FRotator::ZeroRotator, 
	// 	  EAttachLocation::KeepRelativeOffset,
	// 	  true				
	//   );
		UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		ParticleEffect,
		GetActorLocation(), // 폭발 위치
		FRotator::ZeroRotator,
		true // 네트워크에서 실행될 수 있도록 true
	);
	}
}

void ARedZoneBomb::OnExplosionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
		
	if (PlayerCharacter)
	{
		UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
	
		if (ASC)
		{
			UGameplayEffect* GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();  
			//적용할 대상, 적용할 게임 이펙트, 레벨, 이펙트 컨텍스트 
			ASC->ApplyGameplayEffectToSelf(GameplayEffect, 1, ASC->MakeEffectContext());				
		}
	}
}

