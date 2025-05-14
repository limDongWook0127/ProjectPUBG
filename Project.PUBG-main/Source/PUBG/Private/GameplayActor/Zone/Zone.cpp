// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor/Zone/Zone.h"
#include "Character/PlayerCharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Components/PostProcessComponent.h"
#include "BaseLibrary/BaseFunctionLibrary.h"
#include "GameState/BaseGameState.h"

// Sets default values
AZone::AZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	ZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>("Sphere Zone Mesh");
	RootComponent = ZoneMesh;
	
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));

	ShrinkFactor = 0.7f;	
	bIsVisibiltyNextZone = false;
	
	bReplicates = true;
	//SetReplicates(true);
	AActor::SetReplicateMovement(true);
	ZoneMesh->SetIsReplicated(true);
}

void AZone::NotifySize()
{
	//다음 원의 반지름
	NextScale = FMath::Clamp(CurrentScale * ShrinkFactor,0,CurrentScale);
	//다음 원의 중심
	NextLocation = GetRandomPointInCircle(CurrentLocation,GetCurrentRadius() * (1 - ShrinkFactor) );		
	bIsVisibiltyNextZone = true;

	if(CurrentScale <= 0)
	{
		return;
	}
	
	UpdateNextZone();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AZone::StartShirnkZone, ShirnkTime, false);
}

void AZone::StartShirnkZone()
{	
	if (CurveFloat)
	{
		TimelineComponent->PlayFromStart();
	}
}

float AZone::GetMeshWorldScale() const
{
	if (ZoneMesh)
	{
		return GetActorScale3D().X;
	}

	return 1.0f;
}

void AZone::UpdateZoneDamage(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		UBaseAbilitySystemComponent* ASC = Cast<UBaseAbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent());

		FGameplayEffectSpecHandle GameplayEffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level,ASC->MakeEffectContext());
		ASC->ModifyGameplayEffectLevel(GameplayEffectSpecHandle, Level);		
	}
}

void AZone::UpdateShrinkZone(float Value)
{
	if (HasAuthority())
	{
		float TempSize = FMath::Lerp(CurrentScale, NextScale, Value);
		FVector TempLocation = FMath::Lerp(CurrentLocation, NextLocation, Value);

		SetActorScale3D(FVector(TempSize, TempSize ,GetActorScale3D().Z));
		SetActorLocation(TempLocation);

		
		//UE_LOG(LogTemp, Warning, TEXT("Update!  TempSize: %f") , TempSize);
		//UE_LOG(LogTemp, Warning, TEXT("Update!  TempLocation: %s") , *TempLocation.ToString());
		UpdateCurrentZone();
	}
}

void AZone::TimelineFinishedFunction()
{
	CurrentLocation = NextLocation;
	CurrentScale = NextScale;
	
	NextLocation = FVector::ZeroVector;
	NextScale = 0.0f;
	
	bIsVisibiltyNextZone = false;

	UpdateCurrentZone();
	UpdateNextZone();

	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AZone::NotifySize, ShirnkTime, false);

	Level++;

	TArray<APlayerCharacter*> PlayerCharacters = UBaseFunctionLibrary::GetAllPlayersInWorld(GetWorld());
	for (APlayerCharacter* PlayerCharacter : PlayerCharacters)
	{
		UpdateZoneDamage(PlayerCharacter);		
	}	
}

FVector AZone::GetRandomPointInCircle(FVector OriginCenter, float RandomRange) //랜덤위치
{
	float DistanceFromCenter = FMath::RandRange(0.0f, RandomRange);

	float Theta = FMath::RandRange(0.0f, 2 * PI);

	float X = OriginCenter.X + DistanceFromCenter * FMath::Cos(Theta);
	float Y = OriginCenter.Y + DistanceFromCenter * FMath::Sin(Theta);

	FVector RandomPoint = FVector(X, Y, OriginCenter.Z);
	
	return RandomPoint;
}

float AZone::GetCurrentRadius()
 {
	float Radius = ZoneMesh->GetStaticMesh()->GetBoundingBox().Max.X * CurrentScale;
	
 	return Radius;
 }

void AZone::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s has overlapped with the component!"), *OtherActor->GetName());

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

	if (PlayerCharacter)
	{
		UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
		
		if (ASC)
		{
			//적용할 대상, 적용할 게임 이펙트, 레벨, 이펙트 컨텍스트 
			ASC->RemoveActiveGameplayEffectBySourceEffect(GameplayEffectClass,nullptr,-1);

			if (PlayerCharacter->IsLocallyControlled())
			{
				PlayerCharacter->PostProcessComponent->SetVisibility(false);
			}
		}
	}
}

void AZone::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s has first Endoverlapped with the component!"), *OtherActor->GetName());
	
	 	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
			if (ASC)
			{
				UGameplayEffect* GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();  
				//적용할 대상, 적용할 게임 이펙트, 레벨, 이펙트 컨텍스트 
				ASC->ApplyGameplayEffectToSelf(GameplayEffect, Level, ASC->MakeEffectContext());
				
				if (PlayerCharacter->IsLocallyControlled())
				{
					PlayerCharacter->PostProcessComponent->SetVisibility(true);
				}
			}
		}
}

// Called when the game starts or when spawned
void AZone::BeginPlay()
{
	Super::BeginPlay();

	//이벤트 연결
	ZoneMesh->OnComponentBeginOverlap.AddDynamic(this,&AZone::OnComponentBeginOverlap);
	ZoneMesh->OnComponentEndOverlap.AddDynamic(this,&AZone::OnComponentEndOverlap);

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this,&AZone::UpdateShrinkZone);
		TimelineComponent->AddInterpFloat(CurveFloat,TimelineProgress);
		
		FOnTimelineEvent TimelineFinishedEvent;
		TimelineFinishedEvent.BindDynamic(this,&AZone::TimelineFinishedFunction);
		TimelineComponent->SetTimelineFinishedFunc(TimelineFinishedEvent);

		TimelineComponent->SetTimelineLength(ShirnkTime);
	}
}

void AZone::UpdateCurrentZone()
{
	if (HasAuthority())
	{
		FVector Location = GetActorLocation();
		float Scale = GetMeshWorldScale();
		ABaseGameState* GameState = GetWorld()->GetGameState<ABaseGameState>();
		if (GameState && GameState->HasAuthority())
		{
			GameState->UpdateCurrentZone(Location, Scale); 
		}
	}
}

void AZone::UpdateNextZone()
{
	if (HasAuthority())
	{		
		ABaseGameState* GameState = GetWorld()->GetGameState<ABaseGameState>();
		if (GameState && GameState->HasAuthority())
		{
			GameState->UpdateNextZone(NextLocation, NextScale, bIsVisibiltyNextZone); 
		}
	}
}

void AZone::StartShrinkTimer()
{	
	CurrentLocation = GetActorLocation();
	CurrentScale = GetMeshWorldScale();
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AZone::NotifySize, ShirnkTime, false);
}

void AZone::SetShirnkTime(int NewShirnkTime)
{
	ShirnkTime = NewShirnkTime;
}



