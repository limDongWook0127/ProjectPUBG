// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Inventory/GA_ToggleWorldMap.h"

#include "Controller/BasePlayerController.h"


UGA_ToggleWorldMap::UGA_ToggleWorldMap()
{	
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
}

void UGA_ToggleWorldMap::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetCanBeCanceled(false);

	GetPlayerControllerFromActorInfo()->ToggleMapWidget();
	EndAbility(Handle,ActorInfo, ActivationInfo, false, true);	
}

void UGA_ToggleWorldMap::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
