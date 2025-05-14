// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor/InteractDoor.h"

#include "BaseLibrary/BaseDebugHelper.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractDoor::AInteractDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	InteractionTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionTriggerBox"));
	InteractionTriggerBox->SetupAttachment(RootComponent);
	DoorCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorCollisionBox"));
	DoorCollisionBox->SetupAttachment(RootComponent);
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);
}

void AInteractDoor::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindDynamic(this,&AInteractDoor::OpenDoor);
		TimeLine.AddInterpFloat(CurveFloat,TimeLineProgress);
	}
}

void AInteractDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeLine.TickTimeline(DeltaTime);
}

void AInteractDoor::InteractWith_Implementation(APlayerCharacter* Character)
{
	IInteractInterface::InteractWith_Implementation(Character);
	if (IsOpen == false)
	{		
		SetDoorOnSameSide(Character);
		TimeLine.Play();
		
		IsOpen = true;
	}
	else
	{
		TimeLine.Reverse();
		IsOpen = false;
	}

	if (HasAuthority())
	{
		Multicast_PlayDoorSound();
	}
}

FText AInteractDoor::LookAt()
{
	return FText::FromString(TEXT("문 열기"));
}

void AInteractDoor::OpenDoor(float Value)
{
	float Angle = bDoorOnSameSide ? -DoorRotateAngle : DoorRotateAngle;
	DoorMesh->SetRelativeRotation(FRotator(0.0f, Angle * Value, 0.0f));
}

void AInteractDoor::Multicast_PlayDoorSound_Implementation()
{
	if (DoorSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorSound, GetActorLocation(), 1.0f, 1.0f, 0.0f, DoorSoundAttenuation);
	}
}

void AInteractDoor::SetDoorOnSameSide(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		FVector PlayerCharacterFV = PlayerCharacter->GetActorForwardVector();
		FVector DoorFV = DoorMesh->GetForwardVector();

		bDoorOnSameSide = (FVector::DotProduct(PlayerCharacterFV, DoorFV) >= 0);

		if(bDoorOnSameSide)
			Debug::Print(TEXT("True"));
		else
		{
			Debug::Print(TEXT("False"));
		}
			
	}
}

	
	
	



