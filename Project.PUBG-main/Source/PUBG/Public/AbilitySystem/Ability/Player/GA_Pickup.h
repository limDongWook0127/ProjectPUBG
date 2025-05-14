// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "GA_Pickup.generated.h"

/**
 * 
 */

UENUM()
enum class EMontageType : uint8
{
	StandHigh UMETA(DisplayName = "StandHigh"),
	StandMiddle UMETA(DisplayName = "StandMiddle"),
	StandLow UMETA(DisplayName = "StandLow"),
	CrouchHigh UMETA(DisplayName = "CrouchHigh"),
	CrouchMiddle UMETA(DisplayName = "CrouchMiddle"),
	CrouchLow UMETA(DisplayName = "CrouchLow"),
	ProneHigh UMETA(DisplayName = "ProneHigh"),
	ProneMiddle UMETA(DisplayName = "ProneMiddle"),
	ProneLow UMETA(DisplayName = "ProneLow")
};

UCLASS()
class PUBG_API UGA_Pickup : public UPlayerGameplayAbility
{
	GENERATED_BODY()
	UGA_Pickup();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PickupMontage")
	TArray<UAnimMontage*> UnarmedPickupMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PickupMontage")
	TArray< UAnimMontage*> RiflePickupMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PickupMontage")
	TArray<UAnimMontage*> GrenadePickupMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PickupMontage")
	TArray<UAnimMontage*> MeleePickupMontage;

	float DefaultStandHeight;
	float DefaultStandHeightHalf;
	float DefaultCrouchHeight;
	float DefaultCrouchHeightHalf;
	float DefaultProneHeight;
	float DefaultProneHeightHalf;
	
public:
	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	UFUNCTION()
	void UnarmedSelectedMontagePlay(EMontageType MontageType);
	void RifleSelectedMontagePlay(EMontageType MontageType);
	void GrenadeSelectedMontagePlay(EMontageType MontageType);
	void MeleeSelectedMontagePlay(EMontageType MontageType);

	
	
};
