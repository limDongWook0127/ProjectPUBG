// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "BaseAnimInstance.generated.h"

class ABaseCharacter;
class APlayerCharacter;
class UPlayerMovementComponent;
/**
 * 
 */
UCLASS()
class PUBG_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

	UPROPERTY()
	ABaseCharacter* OwningCharacter;
	UPROPERTY()
	APlayerCharacter* OwningPlayer;
	UPROPERTY()
	APlayerController* OwningPlayerController;
	UPROPERTY()
	UPlayerMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	float Direction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	FVector Velocity;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	bool bHasAcceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	bool bIsRun;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	bool bIsFalling;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	bool bIsCrouching;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	bool bIsProne;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	float FallingTime;

	// UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AimOffset")
	// FRotator AimRotation;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "AimOffset")
	float Yaw;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "AimOffset")
	float Pitch;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AimOffset")
	float LeaningPressedValue;
	//차량
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Vehicle")
	bool PlayerOntheVehicle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Vehicle")
	bool VehicleFacetoBackward;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Vehicle")
	bool VehicleVelocityBackWard;
	//freefalling
	UPROPERTY(VisibleDefaultsOnly,Replicated, BlueprintReadOnly, Category = "Freefalling")
	bool InFreefalling;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Freefalling")
	float FreefallingMovementInputY;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Freefalling")
	float FreefallingMovementInputX;
	
	//Swim
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Swimming")
	bool IsSwimming;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Swimming")
	float YDirection;

	//Parachute
	UPROPERTY(VisibleDefaultsOnly, Replicated, BlueprintReadOnly, Category = "Parachute")
	bool OntheParachute;
	
protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool OwnerHasTag(FGameplayTag Tag) const;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// UFUNCTION(Server, Reliable)
	// void ServerSetAimRotation(FRotator NewAimRotation);
	// UFUNCTION(NetMulticast, Reliable)
	// void MulticastSetAimRotation(FRotator NewAimRotation);
};
