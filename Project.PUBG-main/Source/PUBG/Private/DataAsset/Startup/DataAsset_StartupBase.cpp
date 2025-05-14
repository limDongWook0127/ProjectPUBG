// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/Startup/DataAsset_StartupBase.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"

void UDataAsset_StartupBase::GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* ASC, int32 Level)
{
	//ASC정보가 유효한지 체크  
	check(ASC);

	//두 종류 모두 부여
	GrantAbilities(ActivateOnGivenAbilities, ASC, Level);
	GrantAbilities(ReactiveAbilities, ASC, Level);
	if (!StartupGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& BP_Effect : StartupGameplayEffects)
		{
			if (!BP_Effect) continue;

			UGameplayEffect* EffectCDO = BP_Effect->GetDefaultObject<UGameplayEffect>();

			UE_LOG(LogTemp, Warning, TEXT("%s"), *EffectCDO->GetName());
			ASC->ApplyGameplayEffectToSelf(EffectCDO, Level, ASC->MakeEffectContext());
		}
	}
	
}

void UDataAsset_StartupBase::GrantAbilities(const TArray<TSubclassOf<UBaseGameplayAbility>> GAs,
	UBaseAbilitySystemComponent* InASC, int32 Level)
{
	//게임플레이어빌리티가 없으면 탈출
	if (GAs.IsEmpty())
	{
		return;
	}

	for (const TSubclassOf<UBaseGameplayAbility> Ability : GAs)
	{
		//게임플레이 스펙을 만들어서 데이터 주입
		FGameplayAbilitySpec Spec(Ability);
		Spec.SourceObject = InASC->GetAvatarActor();
		Spec.Level = Level;

		//주입된 스펙을 사용할 어빌리티 시스템 컴포넌트에 전달
		InASC->GiveAbility(Spec);
	}
}
