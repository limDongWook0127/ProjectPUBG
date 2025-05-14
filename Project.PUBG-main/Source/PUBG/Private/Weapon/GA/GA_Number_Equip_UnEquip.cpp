// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GA/GA_Number_Equip_UnEquip.h"
#include "Character/PlayerCharacter.h"
#include "Component/Equipped/EquippedComponent.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Component/Movement/PlayerMovementComponent.h"

bool UGA_Number_Equip_UnEquip::EqualCurrentWeapon(AWeapon_Base* _Weapon)
{
	if (_Weapon)
	{
		APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
		if (PlayerCharacter)
		{
			if (PlayerCharacter->GetEquippedComponent()->GetCurrentWeapon() == _Weapon)
			{
				return true;
			}
		}
	}
	return false;
}

bool UGA_Number_Equip_UnEquip::IsValidCurrentWeapon()
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	if (PlayerCharacter)
	{
		if (PlayerCharacter->GetEquippedComponent()->GetCurrentWeapon() != nullptr)
		{
			return true;
		}
	}
	return false;
}

void UGA_Number_Equip_UnEquip::Attach_HandToBack_Weapon(FName _SocketName)
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	if (PlayerCharacter)
	{
		if (AWeapon_Base* Weapon_Base = PlayerCharacter->GetEquippedComponent()->GetCurrentWeapon())
		{
			//PlayerCharacter->GetInventoryComponent()->SetLastCurrentWeapon(PlayerCharacter->GetInventoryComponent()->GetCurrentWeapon());
			
			FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
			Weapon_Base->AttachToComponent(PlayerCharacter->GetMesh(), Rules, _SocketName);

			PlayerCharacter->GetEquippedComponent()->SetCurrentWeapon(nullptr);
			
		}
	}
}

void UGA_Number_Equip_UnEquip::Attach_BackToHand_Weapon(FName _SocketName, AWeapon_Base* _Weapon)
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	if (PlayerCharacter)
	{
		if (_Weapon)
		{
			FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
			_Weapon->AttachToComponent(PlayerCharacter->GetMesh(), Rules, _SocketName);
			
			PlayerCharacter->GetEquippedComponent()->SetCurrentWeapon(_Weapon);
			PlayerCharacter->GetEquippedComponent()->SetLastCurrentWeapon(PlayerCharacter->GetEquippedComponent()->GetCurrentWeapon());
		}
	}
}

void UGA_Number_Equip_UnEquip::setSocketName(FName NewName)
{
	this->SocketName = NewName;
}

bool UGA_Number_Equip_UnEquip::EqualCurrentWeaponToPrimaryWeaponSlot()
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	if (PlayerCharacter)
	{
		UEquippedComponent* EquippedItemComponent = PlayerCharacter->GetEquippedComponent();
		
		if (EquippedItemComponent && (EquippedItemComponent->GetCurrentWeapon() == EquippedItemComponent->GetPrimarySlotWeapon()))
		{
			SocketName = FName("slot_primarySocket");
			return true;
		}
	}
	SocketName = FName("slot_secondarySocket");
	return false;
}

bool UGA_Number_Equip_UnEquip::EqualLastCurrentWeaponToPrimaryWeaponSlot()
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	if (PlayerCharacter)
	{
		UEquippedComponent* EquippedItemComponent = PlayerCharacter->GetEquippedComponent();
		
		if (EquippedItemComponent && (EquippedItemComponent->GetLastCurrentWeapon() == EquippedItemComponent->GetPrimarySlotWeapon()))
		{
			SocketName = FName("slot_primarySocket");
			return true;
		}
	}
	SocketName = FName("slot_secondarySocket");
	return false;
}

UAnimMontage* UGA_Number_Equip_UnEquip::selectCurrentMontage_isProne(UAnimMontage* TrueMontage, UAnimMontage* FalseMontage)
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(PlayerCharacter->GetMovementComponent());
	
	if (MovementComponent->RequestToStartProne)
	{
		return TrueMontage;
	}
	return FalseMontage;
}


UAnimMontage* UGA_Number_Equip_UnEquip::selectCurrentMontage_CurrentWeapon(UAnimMontage* TrueMontage,
	UAnimMontage* FalseMontage)
{
	if (EqualCurrentWeaponToPrimaryWeaponSlot())
	{
		return TrueMontage;
	}
	return FalseMontage;
}


UAnimMontage* UGA_Number_Equip_UnEquip::selectCurrentMontage_LastCurrentWeapon(UAnimMontage* TrueMontage,
	UAnimMontage* FalseMontage)
{
	if (EqualLastCurrentWeaponToPrimaryWeaponSlot())
	{
		return TrueMontage;
	}
	return FalseMontage;
}

UAnimMontage* UGA_Number_Equip_UnEquip::selectCurrentMontage_isProne_PrimarySecondary(UAnimMontage* Primary_TrueMontage,
	UAnimMontage* Primary_falseMontage, UAnimMontage* Secondary_TrueMontage, UAnimMontage* Secondary_falseMontage, bool Selectbool)
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(PlayerCharacter->GetMovementComponent());
	
	if (MovementComponent->RequestToStartProne)
	{
		if (Selectbool)
		{
			return Primary_TrueMontage;
		}
		return Secondary_TrueMontage;
	}

	if (Selectbool)
	{
		return Primary_falseMontage;
	}
	return Secondary_falseMontage;
}

