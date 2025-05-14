// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "BasePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API ABasePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABasePlayerState();
	
	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	class UBaseAttributeSet* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	bool IsAlive() const;

	/**
	* GDAttributeSetBase의 속성에 대한 게터입니다. 별도로 지정하지 않는 한 현재 값을 반환합니다.
	*/
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealthRegenRate() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetStaminaRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetArmor() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMoveSpeed() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMagazine() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxMagazine() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetKillCount() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetRank() const;
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void SetRank(float Rank) const;
	
	UFUNCTION(BlueprintCallable, Category = "Tag")
	bool HasDeadTag() const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	class UBaseAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	class UBaseAttributeSet* AttributeSetBase;

	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle HealthRegenRateChangedDelegateHandle;
	FDelegateHandle StaminaChangedDelegateHandle;
	FDelegateHandle MaxStaminaChangedDelegateHandle;
	FDelegateHandle StaminaRegenRateChangedDelegateHandle;
	FDelegateHandle KillCountChangedDelegateHandle;
	FDelegateHandle RankChangedDelegateHandle;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void HealthRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxStaminaChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void KillCountChanged(const FOnAttributeChangeData& Data);
	virtual void RankChanged(const FOnAttributeChangeData& Data);
};

