// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor//SupplyDrop/SupplyDrop.h"
#include "Components/BoxComponent.h"
#include "PUBG/Public/GameplayActor/ItemSpawn/ItemSpawnerComponent.h"
#include "Item/ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

//#include "Landscape.h"

// Sets default values
ASupplyDrop::ASupplyDrop()
{
	PrimaryActorTick.bCanEverTick = false;

	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DropMesh"));
	RootComponent = DropMesh;
	
	CollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBoxComponent"));
	CollisionBoxComponent->SetIsReplicated(true);
	CollisionBoxComponent->SetupAttachment(DropMesh);
	
	InteractionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionComponent"));
	InteractionComponent->SetIsReplicated(true);
	InteractionComponent->SetupAttachment(DropMesh);

	FVector NewBoxCollisionComponentBoxExtent(150.0f, 120.0f, 90.0f);
	CollisionBoxComponent->SetBoxExtent(NewBoxCollisionComponentBoxExtent);

	FVector NewInteractionComponentBoxExtent(37.0f, 43.0f, 18.0f);
	InteractionComponent->SetBoxExtent(NewInteractionComponentBoxExtent);

	
	//충돌
	DropMesh->SetSimulatePhysics(true);
	DropMesh->SetEnableGravity(true);	
	DropMesh->SetNotifyRigidBodyCollision(true);	
	DropMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	
	DropMesh->SetLinearDamping(5.0f);
	DropMesh->SetAngularDamping(1.0f);

	DropMesh->BodyInstance.bLockXTranslation = true;
	DropMesh->BodyInstance.bLockYTranslation = true;
	DropMesh->BodyInstance.bLockZTranslation = false;

	//회전 잠금 (X, Y, Z 회전 고정)
	DropMesh->BodyInstance.bLockXRotation = true;
	DropMesh->BodyInstance.bLockYRotation = true;
	DropMesh->BodyInstance.bLockZRotation = true;

	//스폰컴포넌트
	ItemSpawnerComponent = CreateDefaultSubobject<UItemSpawnerComponent>(TEXT("ItemSpawnerComponent"));
	
}

// Called when the game starts or when spawned
void ASupplyDrop::BeginPlay()
{
	Super::BeginPlay();


	if (HasAuthority())
	{
		SpawnSupplyDropItem();
	}
	
	DropMesh->OnComponentHit.AddDynamic(this, &ASupplyDrop::OnHit);
}


void ASupplyDrop::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
   FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this) 
	{
		UE_LOG(LogTemp, Warning, TEXT("SupplyDrop hit something: %s"), *OtherActor->GetName());

		DropMesh->SetSimulatePhysics(false); 
		if (ParticleEffect)
		{
			  UParticleSystemComponent* ParticleSystemComponent= UGameplayStatics::SpawnEmitterAttached(
				ParticleEffect,
				DropMesh,       
				NAME_None,      
				FVector::ZeroVector, 
				FRotator::ZeroRotator, 
				EAttachLocation::KeepRelativeOffset,
				true
				
			);

			if (ParticleSystemComponent)
			{				
				DropMesh->OnComponentHit.RemoveDynamic(this, &ASupplyDrop::OnHit);	
			}
		}
	}
}

FName ASupplyDrop::GetRandomItemRowName()
{
	TArray<FName> RowNames = ItemDataTable->GetRowNames();
	FName RandomRowName = RowNames[FMath::RandRange(0,RowNames.Num()-1)];
	
	return RandomRowName;
}

void ASupplyDrop::SpawnSupplyDropItem()
{
	ItemSpawnerComponent->SpawnItems(true,this);
}

FText ASupplyDrop::LookAt()
{
	UE_LOG(LogTemp, Warning, TEXT("ASupplyDropLookAt"));
	return FText::FromString(TEXT("보급품 열기"));
}

UTexture2D* ASupplyDrop::SetKeyTexture()
{
	return TabKeyTexture;
}



