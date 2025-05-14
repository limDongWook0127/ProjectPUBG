// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
//#include "Weapon/Guns/Gun_Base.h"
#include "PlayerGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API UPlayerGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Ability")
	APlayerCharacter* GetPlayerCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Ability")
	ABasePlayerController* GetPlayerControllerFromActorInfo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability")
	void OnAbilityStarted();

	
private:
	TWeakObjectPtr<APlayerCharacter> CachedPlayerCharacter;
	TWeakObjectPtr<ABasePlayerController> CachedPlayerController;
};
