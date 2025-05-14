// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "Character/PlayerCharacter.h"
#include "Widgets/Inventory/InventoryWidget.h"
#include "GA_ToggleInventory.generated.h"

class UFindFarmingItem;
/**
 * 
 */
UCLASS()
class PUBG_API UGA_ToggleInventory : public UPlayerGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_ToggleInventory();

	//virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	// virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility);

	
	UFUNCTION()
	void FindFarmingItem(float DeltaTime);

private:
	UPROPERTY(EditDefaultsOnly,Category="Target Lock")
	float TraceDistance = 150.f;
	UPROPERTY(EditDefaultsOnly,Category="Target Lock")
	FVector TraceBoxSize = FVector(300.f, 300.f, 300.f);
	UPROPERTY(EditDefaultsOnly,Category="Target Lock")
	TArray<TEnumAsByte<	EObjectTypeQuery> > BoxTraceChannel;
	UPROPERTY(EditDefaultsOnly,Category="Target Lock")
	bool bIsDebugging;
	
	UPROPERTY()
	UFindFarmingItem* TickTask;
};
