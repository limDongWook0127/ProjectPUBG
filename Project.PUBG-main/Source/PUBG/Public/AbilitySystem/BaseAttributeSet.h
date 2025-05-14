// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class PUBG_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UBaseAttributeSet();

	// AttributeSet Overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
    // 현재 체력, 0이면 능력으로 막지 않는 한 소유자가 죽을 것으로 예상됩니다. MaxHealth로 제한됩니다.
    // 긍정적인 변화는 이것을 직접 사용할 수 있습니다.
    // 건강에 대한 부정적인 변화는 Damage 메타 속성을 거쳐야 합니다.
    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health)
    
    //MaxHealth는 GameplayEffects가 수정할 수 있으므로 자체 속성입니다.
    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth)
    
    //체력 재생률은 매초마다 체력을 수동적으로 증가시킵니다.
    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_HealthRegenRate)
    FGameplayAttributeData HealthRegenRate;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HealthRegenRate)
    	
    // 특수 능력을 실행하는 데 사용되는 현재 체력입니다. MaxStamina로 제한됩니다.
    UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_Stamina)
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Stamina)
    
     // MaxStamina는 GameplayEffects가 수정할 수 있으므로 자체 속성입니다.
    UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_MaxStamina)
    FGameplayAttributeData MaxStamina;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxStamina)
    
    // 스태미나 재생률은 매초마다 스태미나를 수동적으로 증가시킵니다.
    UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_StaminaRegenRate)
    FGameplayAttributeData StaminaRegenRate;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, StaminaRegenRate)
    
    // 방어구는 공격자가 입는 피해량을 감소시킵니다.
    UPROPERTY(BlueprintReadOnly, Category = "Armor", ReplicatedUsing = OnRep_Armor)
    FGameplayAttributeData Armor;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Armor)
    
    // Damage는 DamageExecution에서 최종 데미지를 계산하는 데 사용되는 메타 속성으로, 이후 -Health로 변환됩니다.
    // 서버에만 존재하는 임시값입니다. 복제되지 않았습니다.
    UPROPERTY(BlueprintReadOnly, Category = "Damage")
    FGameplayAttributeData Damage;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Damage)
    
    // MoveSpeed는 캐릭터가 얼마나 빨리 움직일 수 있는지에 영향을 미칩니다.
    UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", ReplicatedUsing = OnRep_MoveSpeed)
    FGameplayAttributeData MoveSpeed;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MoveSpeed)
	
    // Magazine 캐릭터가 현재 장착중인 총기에 장전된 총알 수
    UPROPERTY(BlueprintReadOnly, Category = "Magazine", ReplicatedUsing = OnRep_Magazine)
    FGameplayAttributeData Magazine;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Magazine)
	
    // MaxMagazine 캐릭터가 현재 장착중인 총기에 장전할 수 있는 최대 총알 수
    UPROPERTY(BlueprintReadOnly, Category = "Magazine", ReplicatedUsing = OnRep_MaxMagazine)
    FGameplayAttributeData MaxMagazine;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxMagazine)
	
    // 킬 수
    UPROPERTY(BlueprintReadOnly, Category = "KillCount", ReplicatedUsing = OnRep_KillCount)
    FGameplayAttributeData KillCount;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, KillCount)
	
    // 등수
    UPROPERTY(BlueprintReadOnly, Category = "Rank", ReplicatedUsing = OnRep_Rank)
    FGameplayAttributeData Rank;
    ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Rank)

protected:
    // 연관된 최대 속성이 변경될 때 속성 값을 비례적으로 조정하는 도우미 함수입니다.
	// (즉, MaxHealth가 증가하면 이전과 동일한 비율을 유지하는 만큼 Health가 증가합니다.)
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

	/**
	* 이러한 OnRep 기능은 복제 중에 능력 시스템 내부 표현이 올바르게 동기화되도록 하기 위해 존재합니다.
	**/

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	UFUNCTION()
	virtual void OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate);

	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);
	
	UFUNCTION()
	virtual void OnRep_Magazine(const FGameplayAttributeData& OldMagazine);
	
	UFUNCTION()
	virtual void OnRep_MaxMagazine(const FGameplayAttributeData& OldMaxMagazine);
	
	UFUNCTION()
	virtual void OnRep_KillCount(const FGameplayAttributeData& OldKillCount);
	
	UFUNCTION()
	virtual void OnRep_Rank(const FGameplayAttributeData& OldRank);
};
