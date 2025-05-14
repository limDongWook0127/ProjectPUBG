// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/GA_ProneMotion.h"
#include "AbilitySystem/AbilityTask/PlayMontageAndWaitForEvent.h"
#include "BaseLibrary/BaseFunctionLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Component/PUBGSpringArmComponent.h"
#include "Component/Movement/PlayerMovementComponent.h"


UGA_ProneMotion::UGA_ProneMotion()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_ProneMotion::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	SetCanBeCanceled(false);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	UPlayerMovementComponent* MovementComponent = Cast<
		UPlayerMovementComponent>(PUBGPlayer->GetMovementComponent());
	if (PUBGPlayer->GetMovementComponent()->IsFalling())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
		
	}
	if (MovementComponent->RequestToStartProne) //누워있는 상태면
	{
			FVector UnProneCameraOffset = FVector(0, 0, 80.f);

			PUBGPlayer->GetCameraBoom()->TimelineAddOffset(UnProneCameraOffset, 0.2f);
			MovementComponent->StopProne();
			
			if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Gun"))))
			{
				ProneRifleMontagePlay(EProneMontageType::PronetoStand);
			}
			else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Melee"))))
			{
				ProneMeleeMontagePlay(EProneMontageType::PronetoStand);
			}
			else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Grenade"))))
			{
			ProneGrenadeMontagePlay(EProneMontageType::PronetoStand);
			}
			else
			{
				ProneUnarmedMontagePlay(EProneMontageType::PronetoStand);
			}
		
		return;
	}

	if (MovementComponent->IsCrouching())
	{
			PUBGPlayer->UnCrouch();
			MovementComponent->StartProne();
			FVector ProneCameraOffset = FVector(0, 0, -40.f);
			PUBGPlayer->GetCameraBoom()->TimelineAddOffset(ProneCameraOffset, 0.2f);
			if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Gun"))))
			{
				ProneRifleMontagePlay(EProneMontageType::CrouchtoProne);
			}
			else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Melee"))))
			{
				ProneMeleeMontagePlay(EProneMontageType::CrouchtoProne);
			}
			else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Grenade"))))
			{
				ProneGrenadeMontagePlay(EProneMontageType::CrouchtoProne);
			}
			else
			{
				ProneUnarmedMontagePlay(EProneMontageType::CrouchtoProne);
			}
		
	
		return;
	}
	
		FVector ProneCameraOffset = FVector(0, 0, -80.f);
		PUBGPlayer->GetCameraBoom()->TimelineAddOffset(ProneCameraOffset, 0.2f); //카메라 오프셋 이동
		MovementComponent->StartProne();
		if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Gun"))))
		{
			ProneRifleMontagePlay(EProneMontageType::StandtoProne);
		}
		else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Melee"))))
		{
			ProneMeleeMontagePlay(EProneMontageType::StandtoProne);
		}
		else if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Grenade"))))
		{
			ProneGrenadeMontagePlay(EProneMontageType::StandtoProne);
		}
		else
		{
			ProneUnarmedMontagePlay(EProneMontageType::StandtoProne);
		}
		
	
}

void UGA_ProneMotion::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_ProneMotion::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


void UGA_ProneMotion::ProneUnarmedMontagePlay(EProneMontageType MontageType)
{
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	if (UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(PUBGPlayer))
	{
		if (UnarmedMontage[(uint8)MontageType] != nullptr)
		{
			UAnimMontage* SelectedMontage = UnarmedMontage[(uint8)MontageType];
			UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
				this, NAME_None, SelectedMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
			Task->OnBlendOut.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->OnCompleted.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->OnCancelled.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->OnInterrupted.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->ReadyForActivation();
		}
	}
}

void UGA_ProneMotion::ProneRifleMontagePlay(EProneMontageType MontageType)
{
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	if (UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(PUBGPlayer))
	{
		if (RifleMontage[(uint8)MontageType] != nullptr)
		{
			UAnimMontage* SelectedMontage = RifleMontage[(uint8)MontageType];
			UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
				this, NAME_None, SelectedMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
			Task->OnBlendOut.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->OnCompleted.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->OnCancelled.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->OnInterrupted.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->ReadyForActivation();
		}
	}
}

void UGA_ProneMotion::ProneMeleeMontagePlay(EProneMontageType MontageType)
{
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	if (UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(PUBGPlayer))
	{
		if (MeleeMontage[(uint8)MontageType] != nullptr)
		{
			UAnimMontage* SelectedMontage = MeleeMontage[(uint8)MontageType];
			UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
				this, NAME_None, SelectedMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
			Task->OnBlendOut.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->OnCompleted.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->OnCancelled.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->OnInterrupted.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->ReadyForActivation();
		}
	}
}

void UGA_ProneMotion::ProneGrenadeMontagePlay(EProneMontageType MontageType)
{
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	if (UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(PUBGPlayer))
	{
		if (GrenadeMontage[(uint8)MontageType] != nullptr)
		{
			UAnimMontage* SelectedMontage = GrenadeMontage[(uint8)MontageType];
			UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
				this, NAME_None, SelectedMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
			Task->OnBlendOut.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->OnCompleted.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->OnCancelled.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->OnInterrupted.AddDynamic(this, &UGA_ProneMotion::OnCompleted);
			Task->ReadyForActivation();
		}
	}
}
