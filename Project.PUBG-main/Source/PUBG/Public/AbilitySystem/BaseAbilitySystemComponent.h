// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "BaseAbilitySystemComponent.generated.h"

class UPlayerGameplayAbility;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UBaseAbilitySystemComponent*, SourceASC, float,
                                               UnmitigatedDamage, float, MitigatedDamage);


struct FPlayerAbilitySet;

/**
 * 
 */
UCLASS()
class PUBG_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void OnAbilityInputPressed(const FGameplayTag& InputTag);
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void OnAbilityInputReleased(const FGameplayTag& InputTag);
	
	UFUNCTION(BlueprintCallable, Category = "Ability", meta = (Level = "1"))
	void GrantPlayerWeaponAbilities(const TArray<FPlayerAbilitySet>& WeaponAbilities, int32 Level, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void RemoveGrantedPlayerWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& SpecHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool TryActivateAbilityByTagToRandom(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool TryActivateAbilityByTag(FGameplayTag Tag);
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void TryCancelAbilityByTag(FGameplayTag Tag);

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, category = "Ability", meta = (AllowPrivateAccess = "true"))
	TArray<FGameplayAbilitySpecHandle> RecentGrantedAbilitySpecHandles;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void GrantAbility(TSubclassOf<UPlayerGameplayAbility> AbilityToGrant, int32 Level, FGameplayAbilitySpecHandle& OutGrantedAbilitySpecHandles);

	void RemoveAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void ResetAllGameplayEffects();
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void ResetAllTags();
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void ResetAbilitySystem();

	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamage;

	// GDDamageExecCalculation에서 호출됩니다. 이 ASC가 피해를 받을 때마다 ReceiverdDamage를 방송합니다.
	virtual void ReceiveDamage(UBaseAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
	void ModifyGameplayEffectLevel(const FGameplayEffectSpecHandle& EffectHandle, float NewLevel);
};
