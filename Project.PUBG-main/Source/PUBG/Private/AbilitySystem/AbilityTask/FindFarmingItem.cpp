// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/FindFarmingItem.h"

#include "UObject/FastReferenceCollector.h"

UFindFarmingItem::UFindFarmingItem()
{
	bTickingTask = true;
}

UFindFarmingItem* UFindFarmingItem::ExecuteTaskTick(UGameplayAbility* OwningAbility)
{
	UFindFarmingItem* Node = NewAbilityTask<UFindFarmingItem>(OwningAbility);
	return Node;
}

void UFindFarmingItem::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
	
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnAbilityTaskTick.Broadcast(DeltaTime);
	}
	else
	{
		EndTask(); // Task 종료
	}
}

