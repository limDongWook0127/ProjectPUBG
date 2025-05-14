// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "GA_ProneMotion.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EProneMontageType : uint8
{
	PronetoStand UMETA(DisplayName = "PronetoStand"),
	CrouchtoProne UMETA(DisplayName = "CrouchtoProne"),
	StandtoProne UMETA(DisplayName = "StandtoProne")
};
UCLASS()
class PUBG_API UGA_ProneMotion : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_ProneMotion();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	void ProneUnarmedMontagePlay(EProneMontageType MontageType);
	void ProneRifleMontagePlay(EProneMontageType MontageType);
	void ProneMeleeMontagePlay(EProneMontageType MontageType);
	void ProneGrenadeMontagePlay(EProneMontageType MontageType);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TranspostureMontage")
	TArray<UAnimMontage*> UnarmedMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TranspostureMontage")
	TArray<UAnimMontage*> RifleMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TranspostureMontage")
	TArray<UAnimMontage*> MeleeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TranspostureMontage")
	TArray<UAnimMontage*> GrenadeMontage;
	
	
};
