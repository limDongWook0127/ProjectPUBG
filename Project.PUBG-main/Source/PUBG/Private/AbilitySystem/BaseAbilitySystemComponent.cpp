// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "BaseLibrary/BaseStructType.h"
#include "GameplayAbilitySpec.h"
#include "AbilitySystem/BaseGameplayTag.h"


void UBaseAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	
	if (!InputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (!Spec.DynamicAbilityTags.HasTagExact(InputTag)) continue;

		// if (InputTag.MatchesTag(BaseGameplayTag::InputTag_Toggle))
		// {
		// 	//InputTag_Toggle 태그가 있으면 Toggle On/Off 처리 
		// 	if (Spec.IsActive())
		// 	{
		// 		CancelAbilityHandle(Spec.Handle);
		// 	}
		// 	else
		// 	{
		// 		TryActivateAbility(Spec.Handle);
		// 	}
		// }
		// else
		{
			TryActivateAbility(Spec.Handle);
		}
	}
}

void UBaseAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *InputTag.ToString())
			CancelAbilityHandle(Spec.Handle);
		}
	}
}

void UBaseAbilitySystemComponent::GrantPlayerWeaponAbilities(const TArray<FPlayerAbilitySet>& WeaponAbilities,
	int32 Level, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (WeaponAbilities.IsEmpty())
	{
		return;
	}

	for (const FPlayerAbilitySet& WeaponAbilitySet : WeaponAbilities)
	{
		if (!WeaponAbilitySet.IsValid()) continue;
		
		FGameplayAbilitySpec Spec(WeaponAbilitySet.AbilityToGrant);
		Spec.SourceObject = GetAvatarActor();
		Spec.Level = Level;
		Spec.DynamicAbilityTags.AddTag(WeaponAbilitySet.InputTag);
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(Spec));
	}
}

void UBaseAbilitySystemComponent::RemoveGrantedPlayerWeaponAbilities(
	TArray<FGameplayAbilitySpecHandle>& SpecHandlesToRemove)
{	
	if (SpecHandlesToRemove.IsEmpty())
	{
		return;
	}

	for (FGameplayAbilitySpecHandle& SpecHandle : SpecHandlesToRemove)
	{

		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}

	SpecHandlesToRemove.Empty();
}

bool UBaseAbilitySystemComponent::TryActivateAbilityByTagToRandom(FGameplayTag Tag)
{
	check(Tag.IsValid());

	TArray<FGameplayAbilitySpec*> AbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(Tag.GetSingleTagContainer(), AbilitySpecs);

	if (!AbilitySpecs.IsEmpty())
	{
		const int32 RandomAbilityIndex = FMath::RandRange(0, AbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* AbilitySpec = AbilitySpecs[RandomAbilityIndex];

		check(AbilitySpec);

		if (!AbilitySpec->IsActive())
		{
			return TryActivateAbility(AbilitySpec->Handle);
		}
	}
		
	return false;
}

bool UBaseAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag Tag)
{
	check(Tag.IsValid());

	TArray<FGameplayAbilitySpec*> AbilitySpecs;

	// �� �Լ��� Ȱ��ȭ ������ ��� ���� �����Ƽ ������ �����´�
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(Tag.GetSingleTagContainer(), AbilitySpecs);

	// abilityspecs �迭�� ���� �����Ͱ� ������
	if (!AbilitySpecs.IsEmpty())
	{
		FGameplayAbilitySpec* AbilitySpec = AbilitySpecs[0];

		check(AbilitySpec);

		if (!AbilitySpec->IsActive())
		{
			return TryActivateAbility(AbilitySpec->Handle);
		}

	}
	return false;
}


void UBaseAbilitySystemComponent::TryCancelAbilityByTag(FGameplayTag Tag)
{
	check(Tag.IsValid());
	FGameplayTagContainer GameplayTagContainer = Tag.GetSingleTagContainer();
	CancelAbilities(&GameplayTagContainer);
}

void UBaseAbilitySystemComponent::GrantAbility(TSubclassOf<UPlayerGameplayAbility> AbilityToGrant, int32 Level,
	FGameplayAbilitySpecHandle& OutGrantedAbilitySpecHandles)
{
	FGameplayAbilitySpec Spec(AbilityToGrant);
	Spec.SourceObject = GetAvatarActor();
	Spec.Level = Level;
	OutGrantedAbilitySpecHandles = GiveAbility(Spec);
}

void UBaseAbilitySystemComponent::RemoveAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (!AbilityClass) return;  // 안전 체크

	// 해당 클래스의 어빌리티가 있는지 확인
	FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityClass);
	if (AbilitySpec)
	{
		// 어빌리티 제거
		ClearAbility(AbilitySpec->Handle);
	}
}

void UBaseAbilitySystemComponent::ResetAllGameplayEffects()
{
	// GetActiveEffects로 활성 게임 이펙트 핸들을 가져오기
	TArray<FActiveGameplayEffectHandle> ActiveEffectHandles = this->GetActiveEffects(FGameplayEffectQuery());

	// 모든 게임 이펙트 제거
	for (const FActiveGameplayEffectHandle& EffectHandle : ActiveEffectHandles)
	{
		this->RemoveActiveGameplayEffect(EffectHandle);
	}
}

void UBaseAbilitySystemComponent::ResetAllTags()
{
	// 부여된 모든 태그를 제거
	FGameplayTagContainer Tags;
	GetOwnedGameplayTags(Tags);

	for (const FGameplayTag& Tag : Tags)
	{
		SetLooseGameplayTagCount(Tag, 0);
	}
}

void UBaseAbilitySystemComponent::ResetAbilitySystem()
{
	ClearAllAbilities();
	ResetAllGameplayEffects();
	ResetAllTags();
}

void UBaseAbilitySystemComponent::ReceiveDamage(UBaseAbilitySystemComponent* SourceASC, float UnmitigatedDamage,
                                                float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}

void UBaseAbilitySystemComponent::ModifyGameplayEffectLevel(const FGameplayEffectSpecHandle& EffectHandle, float NewLevel)
{
	if (!EffectHandle.IsValid() || !EffectHandle.Data || !EffectHandle.Data->Def)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid GameplayEffectSpecHandle!"));
		return;
	}

	// 현재 활성화된 `GameplayEffect` 찾기
	FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAllEffectTags(FGameplayTagContainer());
	TArray<FActiveGameplayEffectHandle> ActiveEffects = GetActiveEffects(Query);

	for (const FActiveGameplayEffectHandle& Effect : ActiveEffects)
	{
		const FActiveGameplayEffect* ActiveEffect = GetActiveGameplayEffect(Effect);
		if (ActiveEffect && ActiveEffect->Spec.Def == EffectHandle.Data->Def)
		{
			SetActiveGameplayEffectLevel(Effect, NewLevel);

			UE_LOG(LogTemp, Warning, TEXT("Increased GameplayEffect Level: %f"), NewLevel);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No active GameplayEffect found matching the given handle."));
}
