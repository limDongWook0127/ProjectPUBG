// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "GA_Throw_Grenade.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API UGA_Throw_Grenade : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	UAnimMontage* selectCurrentMontage_isProne_Throw(UAnimMontage* Throw_TrueMontage,UAnimMontage* Throw_falseMontage);

	
};
