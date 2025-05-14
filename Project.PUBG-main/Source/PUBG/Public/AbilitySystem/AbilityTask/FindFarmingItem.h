// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "FindFarmingItem.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskTickDelegate, float, DeltaTime);

UCLASS()
class PUBG_API UFindFarmingItem : public UAbilityTask
{
	GENERATED_BODY()


public:
	UFindFarmingItem();
	
	static UFindFarmingItem* ExecuteTaskTick(UGameplayAbility* OwningAbility);

	virtual void TickTask(float DeltaTime) override;
	
	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskTickDelegate OnAbilityTaskTick;
	
};
