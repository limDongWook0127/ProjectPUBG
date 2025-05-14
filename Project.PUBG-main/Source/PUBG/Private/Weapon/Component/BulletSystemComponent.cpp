// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Component/BulletSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon/Projectile/Projectile_Base.h"
#include "Controller/BasePlayerController.h"

// Sets default values for this component's properties
UBulletSystemComponent::UBulletSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// 기본값 초기화
	Gravity = 9.8;
	BulletVelocity = FVector(0, 0, 0);
	StartLocation = FVector(0, 0, 0);
	EndLocation = FVector(0, 0, 0);
	IsBulletInitialized = false;
	AirDensity = 1.225;
}


// Called when the game starts
void UBulletSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	Projectile = Cast<AProjectile_Base>(GetOwner());
	Spawn_ShootBullet(Projectile->VelocityVector, Projectile->BulletSpeed);
	
}


// Called every frame
void UBulletSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SpawnLineTrace_HitResult(DeltaTime);
}

float UBulletSystemComponent::GetDragForce(float Air_Density, float Bullet_Speed, float Cross_Section,
	float Drag_Coefficiency)
{
	return (((((Air_Density * (Bullet_Speed * Bullet_Speed)) * Cross_Section) * Drag_Coefficiency) / 2.0) * -1.0);
}

FVector UBulletSystemComponent::CalculateGravity(FVector& Velocity, float DeltaSecond, float Air_Density)
{
	float DrageForce = GetDragForce(AirDensity, UKismetMathLibrary::VSize(Velocity) / 100, Projectile->BulletCrossSection, Projectile->DragCoefficiency);

	FVector NormalizedVelocity = Velocity;
	UKismetMathLibrary::Vector_Normalize(NormalizedVelocity, 0.0001f);
	
	FVector SecondVector = (((DrageForce / Projectile->BulletMass) * 100.0f) * NormalizedVelocity) * DeltaSecond;
	FVector ThirdVector = UKismetMathLibrary::MakeVector(0, 0, (Gravity * -100.0f) * DeltaSecond);

	return Velocity + SecondVector + ThirdVector;
}

void UBulletSystemComponent::Spawn_ShootBullet(FVector& Velocity, float Bullet_Speed)
{
	BulletVelocity = Velocity * Bullet_Speed;

	StartLocation = GetOwner()->GetActorLocation();
	IsBulletInitialized = true;
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBulletSystemComponent::DestoryBullet, 10.0f, false);
}

void UBulletSystemComponent::SpawnLineTrace_HitResult(float DeltaSecond)
{
	if (IsBulletInitialized)
	{
		DeltaSeconds = DeltaSecond;
		StartLocation = GetOwner()->GetActorLocation();
		EndLocation = (BulletVelocity * DeltaSeconds) + StartLocation;

		FHitResult HitResult;
		ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3);
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwner());
		ActorsToIgnore.Add(Projectile->PlayerCharacter);

		UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLocation, EndLocation, TraceType, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true, FLinearColor(1, 0, 0, 0),
												  FLinearColor(0, 1, 0, 1), 0.05);
		
		// 충돌 판정
		if (HitResult.bBlockingHit)
		{
			float HitLocationmultiplier = FindHitLocation_Multiplier(HitResult.BoneName);
			float HitGuntypeMultiplier = FindShootingGunType_multiplier(HitResult.BoneName);
			float CaclulateDamage = Projectile->BulletDamage * HitLocationmultiplier * HitGuntypeMultiplier; // 마지막 거리별 데미지 curve로 계산
			// tag 설정
			_EventTag = FGameplayTag::RequestGameplayTag("Player.Event.Action.Hit");
			
			// payload 설정
			FGameplayEventData Payload;
			Payload.Instigator = Projectile->PlayerCharacter;
			Payload.Target = HitResult.GetActor();
			Payload.EventMagnitude = CaclulateDamage;

			// UE_LOG(LogTemp, Error, TEXT("Hit_Actor : %s"), *HitResult.BoneName.ToString());

			ABasePlayerController* Playercontroller = Cast<ABasePlayerController>(Projectile->PlayerCharacter->GetController());

			if (Playercontroller && Cast<APlayerCharacter>(HitResult.GetActor()))
			{
				Playercontroller->HitEventServer(HitResult.GetActor(), _EventTag, Payload);
			}
			
			GetOwner()->Destroy();
		}
		else
		{
			GetOwner()->SetActorLocation(EndLocation);
			BulletVelocity = CalculateGravity(BulletVelocity, DeltaSeconds, AirDensity);
		}
	}
}

void UBulletSystemComponent::DestoryBullet()
{
	GetOwner()->Destroy();
}

float UBulletSystemComponent::FindHitLocation_Multiplier(FName _HitBodyName)
{
	if (_HitBodyName == FName(TEXT("head")) || _HitBodyName == FName(TEXT("spine_01"))) return 1.0f;
	if (_HitBodyName == FName(TEXT("spine_03"))) return 1.1f;
	if (_HitBodyName == FName(TEXT("pelvis"))) return 0.9f;
	if (_HitBodyName == FName(TEXT("upperarm_l")) || _HitBodyName == FName(TEXT("upperarm_r"))) return 0.6f;
	if (_HitBodyName == FName(TEXT("lowerarm_l")) || _HitBodyName == FName(TEXT("lowerarm_r"))) return 0.5f;
	if (_HitBodyName == FName(TEXT("hand_l")) || _HitBodyName == FName(TEXT("hand_r"))) return 0.3f;
	if (_HitBodyName == FName(TEXT("thigh_l")) || _HitBodyName == FName(TEXT("thigh_r"))) return 0.6f;
	if (_HitBodyName == FName(TEXT("calf_l")) || _HitBodyName == FName(TEXT("calf_r"))) return 0.5f;
	if (_HitBodyName == FName(TEXT("foot_l")) || _HitBodyName == FName(TEXT("foot_r"))) return 0.3f;
	
	// UE_LOG(LogTemp, Error, TEXT("Hit body name not found!"));
	return 0.0f;
}


float UBulletSystemComponent::FindShootingGunType_multiplier(FName _HitBodyName)
{
	if (Projectile->GunType == EGunType::AR)
	{
		if (_HitBodyName == FName(TEXT("head"))) return 2.35f;
		if (_HitBodyName == FName(TEXT("spine_01")) || _HitBodyName == FName(TEXT("pelvis")) || _HitBodyName == FName(TEXT("spine_03"))) return 1.0f;
		if (_HitBodyName == FName(TEXT("upperarm_l")) || _HitBodyName == FName(TEXT("upperarm_r"))
			|| _HitBodyName == FName(TEXT("lowerarm_l")) || _HitBodyName == FName(TEXT("lowerarm_r"))
			|| _HitBodyName == FName(TEXT("hand_l")) || _HitBodyName == FName(TEXT("hand_r"))
			|| _HitBodyName == FName(TEXT("thigh_l")) || _HitBodyName == FName(TEXT("thigh_r"))
			|| _HitBodyName == FName(TEXT("calf_l")) || _HitBodyName == FName(TEXT("calf_r"))
			|| _HitBodyName == FName(TEXT("foot_l")) || _HitBodyName == FName(TEXT("foot_r"))) return 0.9f;
	}

	if (Projectile->GunType == EGunType::SR)
	{
		if (_HitBodyName == FName(TEXT("head"))) return 2.5f;
		if (_HitBodyName == FName(TEXT("spine_01")) || _HitBodyName == FName(TEXT("pelvis")) || _HitBodyName == FName(TEXT("spine_03"))) return 1.3f;
		if (_HitBodyName == FName(TEXT("upperarm_l")) || _HitBodyName == FName(TEXT("upperarm_r"))
			|| _HitBodyName == FName(TEXT("lowerarm_l")) || _HitBodyName == FName(TEXT("lowerarm_r"))
			|| _HitBodyName == FName(TEXT("hand_l")) || _HitBodyName == FName(TEXT("hand_r"))
			|| _HitBodyName == FName(TEXT("thigh_l")) || _HitBodyName == FName(TEXT("thigh_r"))
			|| _HitBodyName == FName(TEXT("calf_l")) || _HitBodyName == FName(TEXT("calf_r"))
			|| _HitBodyName == FName(TEXT("foot_l")) || _HitBodyName == FName(TEXT("foot_r"))) return 0.9f;
	}

	if (Projectile->GunType == EGunType::DMR)
	{
		if (_HitBodyName == FName(TEXT("head"))) return 2.35f;
		if (_HitBodyName == FName(TEXT("spine_01")) || _HitBodyName == FName(TEXT("pelvis")) || _HitBodyName == FName(TEXT("spine_03"))) return 1.05f;
		if (_HitBodyName == FName(TEXT("upperarm_l")) || _HitBodyName == FName(TEXT("upperarm_r"))
			|| _HitBodyName == FName(TEXT("lowerarm_l")) || _HitBodyName == FName(TEXT("lowerarm_r"))
			|| _HitBodyName == FName(TEXT("hand_l")) || _HitBodyName == FName(TEXT("hand_r"))
			|| _HitBodyName == FName(TEXT("thigh_l")) || _HitBodyName == FName(TEXT("thigh_r"))
			|| _HitBodyName == FName(TEXT("calf_l")) || _HitBodyName == FName(TEXT("calf_r"))
			|| _HitBodyName == FName(TEXT("foot_l")) || _HitBodyName == FName(TEXT("foot_r"))) return 0.95f;
	}

	if (Projectile->GunType == EGunType::SG)
	{
		if (_HitBodyName == FName(TEXT("head"))) return 1.5f;
		if (_HitBodyName == FName(TEXT("spine_01")) || _HitBodyName == FName(TEXT("pelvis")) || _HitBodyName == FName(TEXT("spine_03"))) return 0.9f;
		if (_HitBodyName == FName(TEXT("upperarm_l")) || _HitBodyName == FName(TEXT("upperarm_r"))
			|| _HitBodyName == FName(TEXT("lowerarm_l")) || _HitBodyName == FName(TEXT("lowerarm_r"))
			|| _HitBodyName == FName(TEXT("hand_l")) || _HitBodyName == FName(TEXT("hand_r"))
			|| _HitBodyName == FName(TEXT("thigh_l")) || _HitBodyName == FName(TEXT("thigh_r"))
			|| _HitBodyName == FName(TEXT("calf_l")) || _HitBodyName == FName(TEXT("calf_r"))
			|| _HitBodyName == FName(TEXT("foot_l")) || _HitBodyName == FName(TEXT("foot_r"))) return 1.2f;
	}

	if (Projectile->GunType == EGunType::SMG)
	{
		if (_HitBodyName == FName(TEXT("head"))) return 2.1f;
		if (_HitBodyName == FName(TEXT("spine_01")) || _HitBodyName == FName(TEXT("pelvis")) || _HitBodyName == FName(TEXT("spine_03"))) return 1.05f;
		if (_HitBodyName == FName(TEXT("upperarm_l")) || _HitBodyName == FName(TEXT("upperarm_r"))
			|| _HitBodyName == FName(TEXT("lowerarm_l")) || _HitBodyName == FName(TEXT("lowerarm_r"))
			|| _HitBodyName == FName(TEXT("hand_l")) || _HitBodyName == FName(TEXT("hand_r"))
			|| _HitBodyName == FName(TEXT("thigh_l")) || _HitBodyName == FName(TEXT("thigh_r"))
			|| _HitBodyName == FName(TEXT("calf_l")) || _HitBodyName == FName(TEXT("calf_r"))
			|| _HitBodyName == FName(TEXT("foot_l")) || _HitBodyName == FName(TEXT("foot_r"))) return 1.3f;
	}

	if (Projectile->GunType == EGunType::Pistol)
	{
		if (_HitBodyName == FName(TEXT("head"))) return 1.5f;
		if (_HitBodyName == FName(TEXT("spine_01")) || _HitBodyName == FName(TEXT("pelvis")) || _HitBodyName == FName(TEXT("spine_03"))) return 1.0f;
		if (_HitBodyName == FName(TEXT("upperarm_l")) || _HitBodyName == FName(TEXT("upperarm_r"))
			|| _HitBodyName == FName(TEXT("lowerarm_l")) || _HitBodyName == FName(TEXT("lowerarm_r"))
			|| _HitBodyName == FName(TEXT("hand_l")) || _HitBodyName == FName(TEXT("hand_r"))
			|| _HitBodyName == FName(TEXT("thigh_l")) || _HitBodyName == FName(TEXT("thigh_r"))
			|| _HitBodyName == FName(TEXT("calf_l")) || _HitBodyName == FName(TEXT("calf_r"))
			|| _HitBodyName == FName(TEXT("foot_l")) || _HitBodyName == FName(TEXT("foot_r"))) return 1.05f;
	}

	if (Projectile->GunType == EGunType::Melee)
	{
		if (_HitBodyName == FName(TEXT("head"))) return 1.5f;
		if (_HitBodyName == FName(TEXT("spine_01")) || _HitBodyName == FName(TEXT("pelvis")) || _HitBodyName == FName(TEXT("spine_03"))) return 1.0f;
		if (_HitBodyName == FName(TEXT("upperarm_l")) || _HitBodyName == FName(TEXT("upperarm_r"))
			|| _HitBodyName == FName(TEXT("lowerarm_l")) || _HitBodyName == FName(TEXT("lowerarm_r"))
			|| _HitBodyName == FName(TEXT("hand_l")) || _HitBodyName == FName(TEXT("hand_r"))
			|| _HitBodyName == FName(TEXT("thigh_l")) || _HitBodyName == FName(TEXT("thigh_r"))
			|| _HitBodyName == FName(TEXT("calf_l")) || _HitBodyName == FName(TEXT("calf_r"))
			|| _HitBodyName == FName(TEXT("foot_l")) || _HitBodyName == FName(TEXT("foot_r"))) return 1.2f;
	}
		return 0;
}

