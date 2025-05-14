// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AsyncTaskCooldownChanged.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCooldownChanged, FGameplayTag, CooldownTag, float, TimeRemaining, float, Duration);

/**
 * Blueprint 노드는 Cooldown 태그 배열에 대한 변경 사항(Begin 및 End)에 대한 리스너를 자동으로 등록합니다.
 * UI에서 사용하면 유용합니다.
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class PUBG_API UAsyncTaskCooldownChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnCooldownChanged OnCooldownBegin;

	UPROPERTY(BlueprintAssignable)
	FOnCooldownChanged OnCooldownEnd;

	// 쿨다운 태그를 기반으로 GameplayEffects 쿨다운에 대한 변경 사항(시작 및 종료)을 수신합니다.
	// UseServerCooldown은 로컬 예측 쿨다운 외에 서버의 쿨다운이 반환되는지 여부를 결정합니다.
	// ServerCooldown을 사용하는 경우 TimeRemaining 및 Duration은 -1을 반환하여 로컬 예측 쿨다운이 시작되었음을 알립니다.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAsyncTaskCooldownChanged* ListenForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer CooldownTags, bool UseServerCooldown);

	// AsyncTask를 종료하려면 이 함수를 수동으로 호출해야 합니다.
	// UMG 위젯의 경우 위젯의 Destruct 이벤트에서 호출합니다.
	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	UAbilitySystemComponent* ASC;

	FGameplayTagContainer CooldownTags;

	bool UseServerCooldown;

	virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	virtual void CooldownTagChanged(const FGameplayTag CooldownTag, int32 NewCount);

	bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration);
	
};
