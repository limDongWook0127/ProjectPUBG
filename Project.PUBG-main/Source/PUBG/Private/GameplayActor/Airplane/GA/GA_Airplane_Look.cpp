// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor/Airplane/GA/GA_Airplane_Look.h"

UGA_Airplane_Look::UGA_Airplane_Look()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
}

void UGA_Airplane_Look::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetCanBeCanceled(false);


	
	EndAbility(Handle, ActorInfo, ActivationInfo, false, true);		
}
