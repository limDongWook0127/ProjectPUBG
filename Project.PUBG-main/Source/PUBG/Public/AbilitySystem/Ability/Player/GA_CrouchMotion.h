// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "GA_CrouchMotion.generated.h"

UENUM(BlueprintType)
enum class ECrouchMontageType : uint8
{
	CrouchtoStand UMETA(DisplayName = "CrouchtoStand"),
	PronetoCrouch UMETA(DisplayName = "PronetoCrouch"),
	StandtoCrouch UMETA(DisplayName = "StandtoCrouch")
};
/**
 * 
 */
UCLASS()
class PUBG_API UGA_CrouchMotion : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_CrouchMotion();
	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	void CrouchUnarmedMontagePlay(ECrouchMontageType MontageType);
	void CrouchRifleMontagePlay(ECrouchMontageType MontageType);
	void CrouchMeleeMontagePlay(ECrouchMontageType MontageType);
	void CrouchGrenadeMontagePlay(ECrouchMontageType MontageType);

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
