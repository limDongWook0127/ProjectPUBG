// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/ApplyDamageToActor.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BaseGameplayTag.h"

UApplyDamageToActor* UApplyDamageToActor::ApplyDamageToActor(UGameplayAbility* OwningAbility, FName TaskInstanceName, AActor* InSourceActor, AActor* InTargetActor, TSubclassOf<UGameplayEffect> InDamageGameplayEffect, float InDamage)
{
	UApplyDamageToActor* MyTask = NewAbilityTask<UApplyDamageToActor>(OwningAbility, TaskInstanceName);
	MyTask->SourceActor = InSourceActor;
	MyTask->TargetActor = InTargetActor;
	MyTask->DamageGameplayEffect = InDamageGameplayEffect;
	MyTask->Damage = InDamage;

	return MyTask;
}

void UApplyDamageToActor::Activate()
{	
	if (!SourceActor || !TargetActor || !DamageGameplayEffect)
	{
		EndTask(); // 조건에 맞지 않으면 작업 종료
		return;
	}

	// 소스 및 타겟의 AbilitySystemComponent 가져오기
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (!SourceASC || !TargetASC)
	{
		EndTask(); // ASC가 없으면 작업 종료
		return;
	}

	if (TargetASC->HasMatchingGameplayTag(BaseGameplayTag::Player_State_Dead))
	{
		EndTask(); // ASC가 없으면 작업 종료
		return;
	}

	// GameplayEffectSpec 생성
	FGameplayEffectSpecHandle DamageEffectSpecHandle = SourceASC->MakeOutgoingSpec(DamageGameplayEffect, 1, SourceASC->MakeEffectContext());
	if (!DamageEffectSpecHandle.IsValid())
	{
		EndTask();
		return;
	}

	// SetByCaller로 피해량 설정
	DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), Damage);

	// 타겟에 GameplayEffect 적용
	TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());

	// 블루프린트에서 완료 이벤트 실행
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCompleted.Broadcast();
	}

	EndTask(); // 작업 종료
}