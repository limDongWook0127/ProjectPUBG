// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/GA_Turn.h"
#include "AbilitySystem/AbilityTask/PlayMontageAndWaitForEvent.h"
#include "BaseLibrary/BaseFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseGameplayTag.h"

#include "Character/PlayerCharacter.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Component/Movement/PlayerMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UGA_Turn::UGA_Turn()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Turn::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo,
                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	FRotator ActorRot = PUBGPlayer->GetActorRotation();
	FRotator AimRot = PUBGPlayer->GetBaseAimRotation();
	FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(AimRot, ActorRot);
	PUBGPlayer->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	UPawnMovementComponent* MovementComponent = PUBGPlayer->GetMovementComponent();
	UPlayerMovementComponent* PlayerMovementComponent = Cast<UPlayerMovementComponent>(MovementComponent);
	 if (Delta.Yaw < 0) // Left 
	 {
	 	if (PUBGPlayer->bIsCrouched) //앉은상태
	 	{
	 		if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SR) ||
	 			UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SG) ||
	 			UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_DMR) ||
	 			UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SMG) ||
	 			UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_Pistol)||
	 			UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_AR))
	 		{
	 			SelectedMontagePlay(RifleLeftMontage, ETurnMontageType::Crouch);
	 		}
	 		else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Melee))
	 		{
	 			SelectedMontagePlay(MeleeLeftMontage, ETurnMontageType::Crouch);
	 		}
	 		else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Grenade))
	 		{
	 			SelectedMontagePlay(GrenadeLeftMontage, ETurnMontageType::Crouch);
	 		}
	 		else
	 		{
	 			SelectedMontagePlay(UnarmedLeftMontage, ETurnMontageType::Crouch);
	 		}
	 	}
	 	else if (PlayerMovementComponent->RequestToStartProne) //누운상태
	 	{
	 		if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SR) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SG) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_DMR) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SMG) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_Pistol)||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_AR))
	 		{
	 			SelectedMontagePlay(RifleLeftMontage, ETurnMontageType::Prone);
	 		}
	 		else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Melee))
	 		{
	 			SelectedMontagePlay(MeleeLeftMontage, ETurnMontageType::Prone);
	 		}
	 		else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Grenade))
	 		{
	 			SelectedMontagePlay(GrenadeLeftMontage, ETurnMontageType::Prone);
	 		}
	 		else
	 		{
	 			SelectedMontagePlay(UnarmedLeftMontage, ETurnMontageType::Prone);
	 		}
	 	}
	 	else //서있는 상태
	 	{
	 		if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SR) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SG) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_DMR) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SMG) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_Pistol)||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_AR))
	 		{
	 			SelectedMontagePlay(RifleLeftMontage, ETurnMontageType::Stand);
	 		}
	 		else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Melee))
	 		{
	 			SelectedMontagePlay(MeleeLeftMontage, ETurnMontageType::Stand);
	 		}
	 		else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Grenade))
	 		{
	 			SelectedMontagePlay(GrenadeLeftMontage, ETurnMontageType::Stand);
	 		}
	 		else
	 		{
	 			SelectedMontagePlay(UnarmedLeftMontage, ETurnMontageType::Stand);
	 		}
	 	}
	 }
	
	
	
	 else // Right
	 {
	 	if (PUBGPlayer->bIsCrouched) //앉은상태
	 	{
	 		if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SR) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SG) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_DMR) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SMG) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_Pistol)||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_AR))
	 		{
	 			SelectedMontagePlay(RifleRightMontage, ETurnMontageType::Crouch);
	 		}
	 		else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Melee))
	 		{
	 			SelectedMontagePlay(MeleeRightMontage, ETurnMontageType::Crouch);
	 		}
	 		else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Grenade))
	 		{
	 			SelectedMontagePlay(GrenadeRightMontage, ETurnMontageType::Crouch);
	 		}
	 		else
	 		{
	 			SelectedMontagePlay(UnarmedRightMontage, ETurnMontageType::Crouch);
	 		}
	 	}
	 	else if (PlayerMovementComponent->RequestToStartProne) //누운상태
	 	{
	 		if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SR) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SG) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_DMR) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SMG) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_Pistol)||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_AR))
	 		{
	 			SelectedMontagePlay(RifleRightMontage, ETurnMontageType::Prone);
	 		}
	 		else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Melee))
	 		{
	 			SelectedMontagePlay(MeleeRightMontage, ETurnMontageType::Prone);
	 		}
	 		else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Grenade))
	 		{
	 			SelectedMontagePlay(GrenadeRightMontage, ETurnMontageType::Prone);
	 		}
	 		else
	 		{
	 			SelectedMontagePlay(UnarmedRightMontage, ETurnMontageType::Prone);
	 		}
	 	}
	 	else //서있는 상태
	 	{
	 		if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SR) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SG) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_DMR) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SMG) ||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_Pistol)||
				 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_AR))
	 		{
	 			SelectedMontagePlay(RifleRightMontage, ETurnMontageType::Stand);
	 		}
	 		else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Melee))
	 		{
	 			SelectedMontagePlay(MeleeRightMontage, ETurnMontageType::Stand);
	 		}
	 		else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Grenade))
	 		{
	 			SelectedMontagePlay(GrenadeRightMontage, ETurnMontageType::Stand);
	 		}
	 		else
	 		{
	 			SelectedMontagePlay(UnarmedRightMontage, ETurnMontageType::Stand);
	 		}
	 	}
	 }
}

void UGA_Turn::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	 Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	 GetPlayerCharacterFromActorInfo()->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void UGA_Turn::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_Turn::SelectedMontagePlay(TArray<UAnimMontage*> Montage, ETurnMontageType Type)
{
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	if (UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(PUBGPlayer))
	{
		UAnimMontage* SelectedMontage = Montage[(uint8)Type];
		UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
			this, NAME_None, SelectedMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
		Task->OnBlendOut.AddDynamic(this, &UGA_Turn::OnCompleted);
		Task->OnCompleted.AddDynamic(this, &UGA_Turn::OnCompleted);

		Task->ReadyForActivation();
	}
}
