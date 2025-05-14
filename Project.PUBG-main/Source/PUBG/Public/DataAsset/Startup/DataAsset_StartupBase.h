// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "DataAsset_StartupBase.generated.h"

class UBaseGameplayAbility;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class PUBG_API UDataAsset_StartupBase : public UDataAsset
{
	GENERATED_BODY()
		
public:
	//부여하는 ASC Level은 단계 1단계 기본부여
	virtual void GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* ASC, int32 Level = 1);

protected:
	//부여받은 직후 활성화 되는 주어진 능력들
	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray<TSubclassOf<UBaseGameplayAbility>> ActivateOnGivenAbilities;

	//특정 조건에 대한 적중반응이나 사망같은 능력들
	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray<TSubclassOf<UBaseGameplayAbility>> ReactiveAbilities;

	//능력부여
	void GrantAbilities(const TArray<TSubclassOf<UBaseGameplayAbility>> GAs, UBaseAbilitySystemComponent* InASC, int32 Level);

	UPROPERTY(EditDefaultsOnly, Category = "StartupData")
	TArray<TSubclassOf<UGameplayEffect>> StartupGameplayEffects;
};
