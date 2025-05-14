// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/BaseAnimInstance.h"

#include "KismetAnimationLibrary.h"

#include "BaseLibrary/BaseDebugHelper.h"

#include "Kismet/KismetMathLibrary.h"
#include "VectorTypes.h"

#include "Character/BaseCharacter.h"
#include "BaseLibrary/BaseFunctionLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Component/Movement/PlayerMovementComponent.h"
#include "Net/UnrealNetwork.h"


void UBaseAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningMovementComponent = Cast<UPlayerMovementComponent>(OwningCharacter->GetCharacterMovement());
		OwningPlayer=Cast<APlayerCharacter>(OwningCharacter);
	}

}

void UBaseAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}
	
	Velocity = OwningCharacter->GetVelocity();
	GroundSpeed = Velocity.Size2D();
	Direction = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());
	
	if (OwningPlayer->GetController())
	{
		const FRotator MovementRotation = FRotator(OwningPlayer->GetController()->GetControlRotation().Pitch,0.f, 0.f);
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		YDirection = ForwardDirection.Z;
	}
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
	bHasAcceleration = GroundSpeed > 0.f;

	bIsRun = GroundSpeed > 350.f;

	bIsFalling = OwningMovementComponent->IsFalling();
	bIsCrouching = OwningMovementComponent->IsCrouching();
	bIsProne = OwningMovementComponent->RequestToStartProne;	
	
	//bIsProne = OwningPlayer->GetIsProne();
	if (bIsFalling)
	{
		FallingTime += DeltaSeconds;
	}
	else
	{	
		FallingTime =0.f;
	}
	//턴과 에임오프셋을 위한 Yaw,Pitch구하기

	FRotator AimRotation;
	

	if (OwningPlayer->IsZoom)
	{
		if (OwningPlayer->GetController())
		{
			AimRotation =OwningPlayer->GetController()->GetControlRotation();
		}
		else
		{
			AimRotation =OwningPlayer->GetBaseAimRotation();
		}
	}
	else
	{
		AimRotation =OwningPlayer->GetBaseAimRotation();
	}
	// FRotator AimRotation =OwningPlayer->GetBaseAimRotation();
	FRotator ActorRotation = OwningPlayer->GetActorRotation();
	
	FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation);
	Pitch = DeltaRotator.Pitch;
	Yaw = OwningPlayer->Yaw;
	
	LeaningPressedValue = OwningMovementComponent->LeaningValue;
	PlayerOntheVehicle = OwningPlayer->GetOnTheVehicle();
	VehicleFacetoBackward = OwningPlayer->GetVehicleFacetoBackward();
	//UE_LOG(LogTemp, Warning, TEXT("VehicleFacetoBackward: %hhd"), VehicleFacetoBackward);
	VehicleVelocityBackWard = OwningPlayer->GetVehicleVelocityBackWard();
	//UE_LOG(LogTemp, Warning, TEXT("VehicleVelocityBackWard: %hhd"), VehicleVelocityBackWard);
	InFreefalling = OwningPlayer->GetInFreefall();
	FreefallingMovementInputY = OwningPlayer->GetFreefallingMoveInputY();
	FreefallingMovementInputX = OwningPlayer->GetFreefallingMoveInputX();
	//UE_LOG(LogTemp, Warning, TEXT("InFreefalling: %hhd"), InFreefalling)

	IsSwimming = OwningPlayer->GetIsSwimming();
	OntheParachute = OwningPlayer->GetOntheParachute();
	
	
}

bool UBaseAnimInstance::OwnerHasTag(FGameplayTag Tag) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UBaseFunctionLibrary::NativeActorHasTag(OwningPawn, Tag);
	}
	return false;
}

void UBaseAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UBaseAnimInstance, Yaw);
	DOREPLIFETIME(UBaseAnimInstance, Pitch);
	DOREPLIFETIME(UBaseAnimInstance, OntheParachute);
	DOREPLIFETIME(UBaseAnimInstance, InFreefalling);
	
}
