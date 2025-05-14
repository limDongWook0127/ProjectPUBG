// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "GA_Zoom.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API UGA_Zoom : public UPlayerGameplayAbility
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintCallable)
	void ActivatedZoom(USkeletalMeshComponent* GunMesh, UCameraComponent* ActivateCamera, UCameraComponent* DeActivateCamera);
	
	UFUNCTION(BlueprintCallable)
	void DeActivatedZoom(UCameraComponent* ActivateCamera, UCameraComponent* DeActivateCamera);
	
	
};
