// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UBaseGameplayAbility::UBaseGameplayAbility()
{
}

void UBaseGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EBaseAbilityActivationPolicy::OnGiven)
	{
		//액터의 정보를 받아올 수 있거나 해당 어빌리티가 활성화 되지 않았으면
		if (ActorInfo && !Spec.IsActive())
		{
			//현재 가지고 있는 어빌리티를 발동시킨다.
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UBaseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCanceled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCanceled);

	if (AbilityActivationPolicy == EBaseAbilityActivationPolicy::OnGiven)
	{
		//어빌리티가 유효한지 체크
		if (ActorInfo)
		{
			//클리어
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UBaseAbilitySystemComponent* UBaseGameplayAbility::GetBaseAbilitySystemComponentFromActorInfo() const
{
    return Cast<UBaseAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UBaseGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& SpecHandle) const
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	check(ASC);
	check(SpecHandle.IsValid());

	return GetBaseAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, ASC);
}

FActiveGameplayEffectHandle UBaseGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& SpecHandle, EBaseSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, SpecHandle);
	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EBaseSuccessType::Success : EBaseSuccessType::Failed;

	return ActiveGameplayEffectHandle;
}
