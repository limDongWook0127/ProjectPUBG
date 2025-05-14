// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/GA_Pickup.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseGameplayTag.h"
#include "BaseLibrary/BaseFunctionLibrary.h"
#include "Component/Movement/PlayerMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PlayerCharacter.h"
#include "Component/Inventory/InventoryComponent.h"
#include "AbilitySystem/AbilityTask/PlayMontageAndWaitForEvent.h"
#include "BaseLibrary/BaseFunctionLibrary.h"
#include "Item/ItemBase.h"

UGA_Pickup::UGA_Pickup()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	DefaultStandHeight = 168.f;
	DefaultStandHeightHalf = 84.f;
	DefaultCrouchHeight = 101.f;
	DefaultCrouchHeightHalf = 50.5f;
	DefaultProneHeight = 37.f;
	DefaultProneHeightHalf = 18.5f;
}

void UGA_Pickup::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData)
{
	SetCanBeCanceled(false);
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	float PlayerDefualtHeight = PUBGPlayer->GetActorLocation().Z-93.f;
	float StandHeight = DefaultStandHeight + PlayerDefualtHeight;
	float StandHeightHalf=DefaultStandHeightHalf + PlayerDefualtHeight;
	float CrouchHeight=DefaultCrouchHeight + PlayerDefualtHeight;
	float CrouchHeightHalf=DefaultCrouchHeightHalf + PlayerDefualtHeight;
	float ProneHeight=DefaultProneHeight + PlayerDefualtHeight;
	float ProneHeightHalf=DefaultProneHeightHalf + PlayerDefualtHeight;
	UInventoryComponent* InventoryComponent = PUBGPlayer->GetInventoryComponent();
	if (!InventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent is null!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}
	InventoryComponent->ServerGetItem();
	AItemBase* Item = InventoryComponent->GetItem();
	if (!Item)
	{
		// if (GetPlayerCharacterFromActorInfo()->HasAuthority())
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("Execute Server : GA_Pickup"));
		//
		// }
		// else
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("Execute Client : GA_Pickup"));
		//
		// }
		//
		// UE_LOG(LogTemp, Warning, TEXT("Item is null!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}
	float ItemOfZ = Item->GetItemOfZ();
	if (UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SR) ||
			 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SG) ||
			 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_DMR) ||
			 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_SMG) ||
			 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_Pistol)||
			 UBaseFunctionLibrary::NativeActorHasTag(PUBGPlayer, BaseGameplayTag::Weapon_Gun_AR))
	{
		UPlayerMovementComponent* MovementComponent = Cast<
			UPlayerMovementComponent>(PUBGPlayer->GetMovementComponent());
		if (!MovementComponent->IsFalling())
		{UE_LOG(LogTemp, Warning, TEXT("Rifle!"));
			if (MovementComponent->RequestToStartProne)
			{
				if (ItemOfZ <= ProneHeightHalf) //하단 STAND모션
				{
					RifleSelectedMontagePlay(EMontageType::ProneLow);
				}
				else if (ItemOfZ < ProneHeight && ItemOfZ > ProneHeightHalf)
				{
					UE_LOG(LogTemp, Warning, TEXT("Item of Z is out of range!"));

					RifleSelectedMontagePlay(EMontageType::ProneMiddle);
				}
				else
				{
					RifleSelectedMontagePlay(EMontageType::ProneHigh);
				}
			}
			else if (MovementComponent->bWantsToCrouch)
			{
				if (ItemOfZ <= CrouchHeightHalf) //하단 STAND모션
				{
					RifleSelectedMontagePlay(EMontageType::CrouchLow);
				}
				else if (ItemOfZ < CrouchHeight && ItemOfZ > CrouchHeightHalf)
				{
					UE_LOG(LogTemp, Warning, TEXT("Item of Z is out of range!"));

					RifleSelectedMontagePlay(EMontageType::CrouchMiddle);
				}
				else
				{
					RifleSelectedMontagePlay(EMontageType::CrouchHigh);
				}
			}
			else if (!MovementComponent->RequestToStartProne && !MovementComponent->bWantsToCrouch)
			{
				if (ItemOfZ <= StandHeightHalf) //하단 STAND모션
				{
					RifleSelectedMontagePlay(EMontageType::StandLow);
				}
				else if (ItemOfZ < StandHeight &&ItemOfZ > StandHeightHalf)
				{
					UE_LOG(LogTemp, Warning, TEXT("Item of Z is out of range!"));

					RifleSelectedMontagePlay(EMontageType::StandMiddle);
				}
				else
				{
					RifleSelectedMontagePlay(EMontageType::StandHigh);
				}
			}
		}
	}
	else if (UBaseFunctionLibrary::NativeActorHasTag(
		PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Melee"))))
	{
		UPlayerMovementComponent* MovementComponent = Cast<
			UPlayerMovementComponent>(PUBGPlayer->GetMovementComponent());
		if (!MovementComponent->IsFalling())
		{
			if (MovementComponent->RequestToStartProne)
			{
				if (ItemOfZ <= ProneHeightHalf) //하단 STAND모션
				{
					MeleeSelectedMontagePlay(EMontageType::ProneLow);
				}
				else if (ItemOfZ < ProneHeight && ItemOfZ > ProneHeightHalf)
				{
					UE_LOG(LogTemp, Warning, TEXT("Item of Z is out of range!"));

					MeleeSelectedMontagePlay(EMontageType::ProneMiddle);
				}
				else
				{
					MeleeSelectedMontagePlay(EMontageType::ProneHigh);
				}
			}
			else if (MovementComponent->bWantsToCrouch)
			{
				if (ItemOfZ <= CrouchHeightHalf) //하단 STAND모션
				{
					MeleeSelectedMontagePlay(EMontageType::CrouchLow);
				}
				else if (ItemOfZ < CrouchHeight && ItemOfZ > CrouchHeightHalf)
				{
					UE_LOG(LogTemp, Warning, TEXT("Item of Z is out of range!"));

					MeleeSelectedMontagePlay(EMontageType::CrouchMiddle);
				}
				else
				{
					MeleeSelectedMontagePlay(EMontageType::CrouchHigh);
				}
			}
			else if (!MovementComponent->RequestToStartProne && !MovementComponent->bWantsToCrouch)
			{
				if (ItemOfZ <= StandHeightHalf) //하단 STAND모션
				{
					MeleeSelectedMontagePlay(EMontageType::StandLow);
				}
				else if (ItemOfZ < StandHeight &&ItemOfZ > StandHeightHalf)
				{
					UE_LOG(LogTemp, Warning, TEXT("Item of Z is out of range!"));

					MeleeSelectedMontagePlay(EMontageType::StandMiddle);
				}
				else
				{
					MeleeSelectedMontagePlay(EMontageType::StandHigh);
				}
			}
		}
	}
	else if (UBaseFunctionLibrary::NativeActorHasTag(
		PUBGPlayer, FGameplayTag::RequestGameplayTag(FName("Weapon.Grenade"))))
	{
		UPlayerMovementComponent* MovementComponent = Cast<
			UPlayerMovementComponent>(PUBGPlayer->GetMovementComponent());
		if (!MovementComponent->IsFalling())
		{
			if (MovementComponent->RequestToStartProne)
			{
				if (ItemOfZ <= ProneHeightHalf) //하단 STAND모션
				{
					GrenadeSelectedMontagePlay(EMontageType::ProneLow);
				}
				else if (ItemOfZ < ProneHeight && ItemOfZ > ProneHeightHalf)
				{
					UE_LOG(LogTemp, Warning, TEXT("Item of Z is out of range!"));

					GrenadeSelectedMontagePlay(EMontageType::ProneMiddle);
				}
				else
				{
					GrenadeSelectedMontagePlay(EMontageType::ProneHigh);
				}
			}
			else if (MovementComponent->bWantsToCrouch)
			{
				if (ItemOfZ <= CrouchHeightHalf) //하단 STAND모션
				{
					GrenadeSelectedMontagePlay(EMontageType::CrouchLow);
				}
				else if (ItemOfZ < CrouchHeight && ItemOfZ > CrouchHeightHalf)
				{
					UE_LOG(LogTemp, Warning, TEXT("Item of Z is out of range!"));

					GrenadeSelectedMontagePlay(EMontageType::CrouchMiddle);
				}
				else
				{
					GrenadeSelectedMontagePlay(EMontageType::CrouchHigh);
				}
			}
			else if (!MovementComponent->RequestToStartProne && !MovementComponent->bWantsToCrouch)
			{
				if (ItemOfZ <= StandHeightHalf) //하단 STAND모션
				{
					GrenadeSelectedMontagePlay(EMontageType::StandLow);
				}
				else if (ItemOfZ < StandHeight &&ItemOfZ > StandHeightHalf)
				{
					UE_LOG(LogTemp, Warning, TEXT("Item of Z is out of range!"));

					GrenadeSelectedMontagePlay(EMontageType::StandMiddle);
				}
				else
				{
					GrenadeSelectedMontagePlay(EMontageType::StandHigh);
				}
			}
		}
	}
	else
	{
		UPlayerMovementComponent* MovementComponent = Cast<
			UPlayerMovementComponent>(PUBGPlayer->GetMovementComponent());
		if (!MovementComponent->IsFalling())
		{
			if (MovementComponent->RequestToStartProne)
			{
				if (ItemOfZ <= ProneHeightHalf) //하단 STAND모션
				{
					UnarmedSelectedMontagePlay(EMontageType::ProneLow);
				}
				else if (ItemOfZ < ProneHeight && ItemOfZ > ProneHeightHalf)
				{
					UE_LOG(LogTemp, Warning, TEXT("Item of Z is out of range!"));

					UnarmedSelectedMontagePlay(EMontageType::ProneMiddle);
				}
				else
				{
					UnarmedSelectedMontagePlay(EMontageType::ProneHigh);
				}
			}
			else if (MovementComponent->bWantsToCrouch)
			{
				if (ItemOfZ <= CrouchHeightHalf) //하단 STAND모션
				{
					UnarmedSelectedMontagePlay(EMontageType::CrouchLow);
				}
				else if (ItemOfZ < CrouchHeight && ItemOfZ > CrouchHeightHalf)
				{
					UE_LOG(LogTemp, Warning, TEXT("Item of Z is out of range!"));

					UnarmedSelectedMontagePlay(EMontageType::CrouchMiddle);
				}
				else
				{
					UnarmedSelectedMontagePlay(EMontageType::CrouchHigh);
				}
			}
			else if (!MovementComponent->RequestToStartProne && !MovementComponent->bWantsToCrouch)
			{
				if (ItemOfZ <= StandHeightHalf) //하단 STAND모션
				{
					UnarmedSelectedMontagePlay(EMontageType::StandLow);
				}
				else if (ItemOfZ < StandHeight &&ItemOfZ > StandHeightHalf)
				{
					UE_LOG(LogTemp, Warning, TEXT("Item of Z is out of range!"));

					UnarmedSelectedMontagePlay(EMontageType::StandMiddle);
				}
				else
				{
					UnarmedSelectedMontagePlay(EMontageType::StandHigh);
					UE_LOG(LogTemp, Warning, TEXT("high!"));
				}
			}
		}
	}
	//EndAbility(Handle,ActorInfo, ActivationInfo, false, true);
}


void UGA_Pickup::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UGA_Pickup::UnarmedSelectedMontagePlay(EMontageType MontageType)
{
	APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	if (UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(PUBGPlayer))
	{
		if (UnarmedPickupMontage[(uint8)MontageType] != nullptr)
		{
			UAnimMontage* UnarmedSelectedMontage = UnarmedPickupMontage[(uint8)MontageType];
			UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
				this, NAME_None, UnarmedSelectedMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
			Task->OnBlendOut.AddDynamic(this, &UGA_Pickup::OnCompleted);
			Task->OnCompleted.AddDynamic(this, &UGA_Pickup::OnCompleted);
			//Task->Oncancel
			Task->ReadyForActivation();
		}
	}
}

void UGA_Pickup::RifleSelectedMontagePlay(EMontageType MontageType)
{APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	if (UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(PUBGPlayer))
	{
		if (UnarmedPickupMontage[(uint8)MontageType] != nullptr)
		{
			UAnimMontage* RifleSelectedMontage = RiflePickupMontage[(uint8)MontageType];
			UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
				this, NAME_None, RifleSelectedMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
			Task->OnBlendOut.AddDynamic(this, &UGA_Pickup::OnCompleted);
			Task->OnCompleted.AddDynamic(this, &UGA_Pickup::OnCompleted);

			Task->ReadyForActivation();
		}
	}
}

void UGA_Pickup::GrenadeSelectedMontagePlay(EMontageType MontageType)
{APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	if (UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(PUBGPlayer))
	{
		if (UnarmedPickupMontage[(uint8)MontageType] != nullptr)
		{
			UAnimMontage* GrenadeSelectedMontage =  GrenadePickupMontage[(uint8)MontageType];
			UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
				this, NAME_None, GrenadeSelectedMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
			Task->OnBlendOut.AddDynamic(this, &UGA_Pickup::OnCompleted);
			Task->OnCompleted.AddDynamic(this, &UGA_Pickup::OnCompleted);

			Task->ReadyForActivation();
		}
	}
}

void UGA_Pickup::MeleeSelectedMontagePlay(EMontageType MontageType)
{APlayerCharacter* PUBGPlayer = GetPlayerCharacterFromActorInfo();
	if (UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(PUBGPlayer))
	{
		if (UnarmedPickupMontage[(uint8)MontageType] != nullptr)
		{
			UAnimMontage* MeleeSelectedMontage = MeleePickupMontage[(uint8)MontageType];
			UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
				this, NAME_None, MeleeSelectedMontage, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
			Task->OnBlendOut.AddDynamic(this, &UGA_Pickup::OnCompleted);
			Task->OnCompleted.AddDynamic(this, &UGA_Pickup::OnCompleted);

			Task->ReadyForActivation();
		}
	}
}
