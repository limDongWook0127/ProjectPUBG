// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "GA_Airplane_Fall.generated.h"

class AAirplane;
/**
 * 
 */
UCLASS()
class PUBG_API UGA_Airplane_Fall : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Airplane_Fall();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<TSubclassOf<UPlayerGameplayAbility>> PlayerGameplayAbilities;
	
	
};
