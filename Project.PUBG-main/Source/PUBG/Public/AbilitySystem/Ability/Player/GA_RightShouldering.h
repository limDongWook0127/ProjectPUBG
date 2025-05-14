// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "GA_RightShouldering.generated.h"

class UPUBGSpringArmComponent;
/**
 * 
 */
UCLASS()
class PUBG_API UGA_RightShouldering : public UPlayerGameplayAbility
{
	GENERATED_BODY()
	UGA_RightShouldering();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData);
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UPROPERTY()
	UPUBGSpringArmComponent* PlayerCameraBoom;
	UPROPERTY()
	bool CameraLeftMoved = false;
};
