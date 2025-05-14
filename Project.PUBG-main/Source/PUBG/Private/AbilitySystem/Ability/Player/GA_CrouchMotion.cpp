// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/GA_CrouchMotion.h"
#include "AbilitySystem/AbilityTask/PlayMontageAndWaitForEvent.h"
#include "BaseLibrary/BaseFunctionLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Component/PUBGSpringArmComponent.h"
#include "Component/Movement/PlayerMovementComponent.h"


UGA_CrouchMotion::UGA_CrouchMotion()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_CrouchMotion::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	SetCanBeCanceled(false);
	UE_LOG(LogTemp, Warning, TEXT("Crouch PLAY"));
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	UPlayerMovementComponent* MovementComponent = Cast<
		UPlayerMovementComponent>(PUBGPlayer->GetMovementComponent());
	if (MovementComponent->IsFalling())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	if (PUBGPlayer->GetMovementComponent()->IsCrouching()) //크라우칭 상태면
	{
		if (!(PUBGPlayer->GetMovementComponent()->Velocity.Size()>0.f))
		{
			FVector UnProneCameraOffset = FVector(0, 0, 40.f);

			PUBGPlayer->GetCameraBoom()->TimelineAddOffset(UnProneCameraOffset, 0.2f);
			PUBGPlayer->UnCrouch();

			if (UBaseFunctionLibrary::NativeActorHasTag(
				PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Gun"))))
			{
				CrouchRifleMontagePlay(ECrouchMontageType::CrouchtoStand);
			}
			else if (UBaseFunctionLibrary::NativeActorHasTag(
				PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Melee"))))
			{
				CrouchMeleeMontagePlay(ECrouchMontageType::CrouchtoStand);
			}
			else if (UBaseFunctionLibrary::NativeActorHasTag(
				PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Grenade"))))
			{
				CrouchGrenadeMontagePlay(ECrouchMontageType::CrouchtoStand);
			}
			else
			{
				CrouchUnarmedMontagePlay(ECrouchMontageType::CrouchtoStand);
			}
		}
		else
		{
			FVector UnProneCameraOffset = FVector(0, 0, 40.f);

			PUBGPlayer->GetCameraBoom()->TimelineAddOffset(UnProneCameraOffset, 0.2f);
			PUBGPlayer->UnCrouch();
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
		return;
	}

	if (MovementComponent->RequestToStartProne) //누워있으면
	{
			MovementComponent->StopProne();
			PUBGPlayer->Crouch();
			FVector ProneCameraOffset = FVector(0, 0, 40.f);
			PUBGPlayer->GetCameraBoom()->TimelineAddOffset(ProneCameraOffset, 0.2f);
			if (UBaseFunctionLibrary::NativeActorHasTag(
				PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Gun"))))
			{
			CrouchRifleMontagePlay(ECrouchMontageType::PronetoCrouch);
			}
			else if (UBaseFunctionLibrary::NativeActorHasTag(
				PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Melee"))))
			{
				CrouchMeleeMontagePlay(ECrouchMontageType::PronetoCrouch);
			}
			else if (UBaseFunctionLibrary::NativeActorHasTag(
				PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Grenade"))))
			{
				CrouchGrenadeMontagePlay(ECrouchMontageType::PronetoCrouch);
			}
			else
			{
				CrouchUnarmedMontagePlay(ECrouchMontageType::PronetoCrouch);
			}
		
		return;
	}

	if (!(PUBGPlayer->GetMovementComponent()->Velocity.Size()>0.f))
	{
		FVector ProneCameraOffset = FVector(0, 0, -40.f);
		PUBGPlayer->GetCameraBoom()->TimelineAddOffset(ProneCameraOffset, 0.2f); //카메라 오프셋 이동
		PUBGPlayer->Crouch();

		if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Gun"))))
		{
			CrouchRifleMontagePlay(ECrouchMontageType::StandtoCrouch);
		}
		else if (UBaseFunctionLibrary::NativeActorHasTag(
			PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Melee"))))
		{
			CrouchMeleeMontagePlay(ECrouchMontageType::StandtoCrouch);
		}
		else if (UBaseFunctionLibrary::NativeActorHasTag(
			PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Grenade"))))
		{
			CrouchGrenadeMontagePlay(ECrouchMontageType::StandtoCrouch);
		}
		else
		{
			CrouchUnarmedMontagePlay(ECrouchMontageType::StandtoCrouch);
		}
	}
	else
	{
		FVector ProneCameraOffset = FVector(0, 0, -40.f);
		PUBGPlayer->GetCameraBoom()->TimelineAddOffset(ProneCameraOffset, 0.2f); //카메라 오프셋 이동
		PUBGPlayer->Crouch();
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UGA_CrouchMotion::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                  bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_CrouchMotion::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_CrouchMotion::CrouchUnarmedMontagePlay(ECrouchMontageType MontageType)
{
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	if (UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(PUBGPlayer))
	{
		if (UnarmedMontage[(uint8)MontageType] != nullptr)
		{
			UAnimMontage* SelectedMontage = UnarmedMontage[(uint8)MontageType];
			UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
				this, NAME_None, SelectedMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
			Task->OnBlendOut.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->OnCompleted.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->OnCancelled.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->OnInterrupted.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->ReadyForActivation();
		}
	}
}

void UGA_CrouchMotion::CrouchRifleMontagePlay(ECrouchMontageType MontageType)
{
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	if (UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(PUBGPlayer))
	{
		if (RifleMontage[(uint8)MontageType] != nullptr)
		{
			UAnimMontage* SelectedMontage = RifleMontage[(uint8)MontageType];
			UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
				this, NAME_None, SelectedMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
			Task->OnBlendOut.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->OnCompleted.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->OnCancelled.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->OnInterrupted.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->ReadyForActivation();
		}
	}
}

void UGA_CrouchMotion::CrouchMeleeMontagePlay(ECrouchMontageType MontageType)
{
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	if (UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(PUBGPlayer))
	{
		if (MeleeMontage[(uint8)MontageType] != nullptr)
		{
			UAnimMontage* SelectedMontage = MeleeMontage[(uint8)MontageType];
			UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
				this, NAME_None, SelectedMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
			Task->OnBlendOut.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->OnCompleted.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->OnCancelled.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->OnInterrupted.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->ReadyForActivation();
		}
	}
}

void UGA_CrouchMotion::CrouchGrenadeMontagePlay(ECrouchMontageType MontageType)
{
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	if (UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(PUBGPlayer))
	{
		if (GrenadeMontage[(uint8)MontageType] != nullptr)
		{
			UAnimMontage* SelectedMontage = GrenadeMontage[(uint8)MontageType];
			UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
				this, NAME_None, SelectedMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
			Task->OnBlendOut.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->OnCompleted.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->OnCancelled.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->OnInterrupted.AddDynamic(this, &UGA_CrouchMotion::OnCompleted);
			Task->ReadyForActivation();
		}
	}
}
