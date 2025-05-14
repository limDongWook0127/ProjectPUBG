// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Grenade/Grenade_Base.h"
#include "Character/PlayerCharacter.h"
#include "Component/Equipped/EquippedComponent.h"
#include "Controller/BasePlayerController.h"
#include "Net/UnrealNetwork.h"

AGrenade_Base::AGrenade_Base()
{
	// 물리 활성화
	//WeaponSkeletalMeshComponent->SetSimulatePhysics(true);

	GrenadeSphereCollision = CreateDefaultSubobject<USphereComponent>("GrenadeSphereCollision");
	GrenadeSphereCollision->SetupAttachment(WeaponSkeletalMeshComponent, FName("CollisionSocket"));
	GrenadeSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GrenadeSphereCollision->SetSphereRadius(260.0f);
}

void AGrenade_Base::BeginPlay()
{
	Super::BeginPlay();

	if (GrenadeSphereCollision != nullptr)
	{
		GrenadeSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AGrenade_Base::OnComponentBeginOverlap);
	}
  	else
  	{
  		//UE_LOG(LogTemp, Error, TEXT("No Collision"));
  	}

	if (OwningPlayerCharacter != nullptr)
	{
		// UE_LOG(LogTemp, Error, TEXT("%s"), *OwningPlayerCharacter->GetName());
	}
	else
	{
	// 	UE_LOG(LogTemp, Error, TEXT("No Owner"));
	}
	
}

// 활성화
void AGrenade_Base::SetHitCollisionActivate()
{
	GrenadeSphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetWorld()->GetTimerManager().SetTimer(CollisionSettingTimerHandle, this, &AGrenade_Base::SetHitCollisionDeActivate, 0.2f, false);
}

// 비활성화
void AGrenade_Base::SetHitCollisionDeActivate()
{
	GrenadeSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->Destroy();
}


void AGrenade_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AGrenade_Base, OwningPlayerCharacter);
	
}

void AGrenade_Base::SettingThrowable()
{
	OwningPlayerCharacter->GetEquippedComponent()->SetCurrentWeapon(nullptr);
	OwningPlayerCharacter->GetEquippedComponent()->GetEquippedItems()[4] = nullptr;
}

// 피격시 
void AGrenade_Base::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
			ABasePlayerController* playercontroller = Cast<ABasePlayerController>(OwningPlayerCharacter->GetController());
			if (!HasAuthority())
			{
				if (playercontroller != nullptr)
				{
					FGameplayTag _EventTag = FGameplayTag::RequestGameplayTag("Player.Event.Action.Hit");
			
					// payload 설정
					FGameplayEventData Payload;
					Payload.Instigator = OwningPlayerCharacter;
					Payload.Target = OtherActor;
					Payload.EventMagnitude = GetWeaponDataAsset().Gun_Damage;
			
					playercontroller->HitEventServer(OtherActor, _EventTag, Payload);
				}
			}

			//UE_LOG(LogTemp, Error, TEXT("%s"), *this->GetOwner()->GetName());
	}
}

