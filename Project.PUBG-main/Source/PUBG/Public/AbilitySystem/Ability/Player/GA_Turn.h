// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "GA_Turn.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETurnMontageType : uint8
{
	Stand UMETA(DisplayName = "Stand"),
	Crouch UMETA(DisplayName = "Crouch"),
	Prone UMETA(DisplayName = "Prone")
};
UCLASS()
class PUBG_API UGA_Turn : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Turn();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TurnMontage")
	TArray<UAnimMontage*> UnarmedRightMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TurnMontage")
	TArray<UAnimMontage*> UnarmedLeftMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TurnMontage")
	TArray< UAnimMontage*> RifleRightMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TurnMontage")
	TArray< UAnimMontage*> RifleLeftMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TurnMontage")
	TArray<UAnimMontage*> GrenadeRightMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TurnMontage")
	TArray<UAnimMontage*> GrenadeLeftMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TurnMontage")
	TArray<UAnimMontage*> MeleeRightMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TurnMontage")
	TArray<UAnimMontage*> MeleeLeftMontage;
public:
	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
	void SelectedMontagePlay(TArray<UAnimMontage*> Montage, ETurnMontageType Type);

};
