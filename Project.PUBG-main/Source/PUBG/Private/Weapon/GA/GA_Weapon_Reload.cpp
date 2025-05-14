// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GA/GA_Weapon_Reload.h"
#include "Character/PlayerCharacter.h"
#include "Component/Equipped/EquippedComponent.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Component/Movement/PlayerMovementComponent.h"
#include "Controller/BasePlayerController.h"
#include "Widgets/HUD/HudWidget.h"
#include "Widgets/HUD/PlayerStatus/PlayerStatusWidget.h"

UAnimMontage* UGA_Weapon_Reload::selectCurrentMontage_isProne_Reload(UAnimMontage* ReloadCharge_TrueMontage,
                                                                     UAnimMontage* ReloadCharge_falseMontage, UAnimMontage* ReloadTectical_TrueMontage, UAnimMontage* ReloadTectical_falseMontage,
                                                                     bool Selectbool)
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(PlayerCharacter->GetMovementComponent());
	
	if (MovementComponent->RequestToStartProne)
	{
		if (Selectbool)
		{
			return ReloadCharge_TrueMontage;
		}
		return ReloadTectical_TrueMontage;
	}

	if (Selectbool)
	{
		return ReloadCharge_falseMontage;
	}
	return ReloadTectical_falseMontage;
}

void UGA_Weapon_Reload::CheckZoom()
{
	if (APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo())
	{
		PlayerCharacter->CallCheckZoomAbility();
	}
}

float UGA_Weapon_Reload::EquippedMagPart_BulletCalulate(APlayerCharacter* PlayerCharacter, AGun_Base* CurrentWeapon, float CurrentBullet,
	float IsMag_changed_BulletValue, FName BulletName)
{
	float bulletArmo = CurrentWeapon->GetBulletArmo();
	float InventoryBulletQuantity = PlayerCharacter->GetInventoryComponent()->GetContent()[PlayerCharacter->GetInventoryComponent()->FindItemSlot(BulletName)].Quantity;
	if (CurrentWeapon->GetEquippedMag())
	{
		if (InventoryBulletQuantity < CurrentBullet + IsMag_changed_BulletValue)
		{
			return InventoryBulletQuantity;
		}
		if (bulletArmo != 0.0f) return CurrentBullet + IsMag_changed_BulletValue - bulletArmo;
		if (bulletArmo == 0.0f) return CurrentBullet + IsMag_changed_BulletValue;
		
		return 0;
	}
	else
	{
		if (InventoryBulletQuantity < CurrentBullet)
		{
			return InventoryBulletQuantity;
		}
		if (bulletArmo != 0.0f) return CurrentBullet - bulletArmo;
		if (bulletArmo == 0.0f) return CurrentBullet;
		return 0;
	}
}

void UGA_Weapon_Reload::SetReloadBullet_CalulateBullet(AGun_Base* CurrentWeapon, float CurrentBullet,
	float IsMag_changed_BulletValue, FName BulletName)
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	FName BulletType = CheckBulletTypeName(BulletName);
	float ChangeValue = PlayerCharacter->GetEquippedComponent()->GetCurrentWeapon_GunBase()->GetWeaponParts()[2].ChangeValue;

	float GetArom = EquippedMagPart_BulletCalulate(PlayerCharacter, CurrentWeapon, CurrentBullet, ChangeValue, BulletType);
	// 대탄 파츠 장착 여부 판탄 후 인벤에서 총알 감소
	PlayerCharacter->GetInventoryComponent()->ServerRemoveItem(	PlayerCharacter->GetInventoryComponent()->FindItemSlot(BulletType),
		GetArom);

	if (CurrentWeapon->GetEquippedMag())
	{
		CurrentWeapon->SetBulletArom(CurrentWeapon->GetBulletArmo() + GetArom);
		//UE_LOG(LogTemp, Error, TEXT("asdfffffffffffffffffff"));
	}
	else
	{
		CurrentWeapon->SetBulletArom(CurrentWeapon->GetBulletArmo() + GetArom);
		//UE_LOG(LogTemp, Error, TEXT("assssssssssssssssssssssssss"));
	}

	if (PlayerCharacter->GetInventoryComponent()->FindItemSlot(BulletType) != -1)
	{
		int32 Index = PlayerCharacter->GetInventoryComponent()->FindItemSlot(BulletType);
		int32 RemainQuantity = PlayerCharacter->GetInventoryComponent()->GetContent()[Index].Quantity;
		UE_LOG(LogTemp, Warning, TEXT("RemainQuantity %d"), RemainQuantity);
		
		
		// if (!PlayerCharacter->HasAuthority())
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("SetReloadBullet_CalulateBullet : Execute Client %d"), RemainQuantity);
		//	GetPlayerControllerFromActorInfo()->GetHudWidget()->GetPlayerStatusWidget()->ClientSetText_Ammo(RemainQuantity);
		// }
	}
	
}

bool UGA_Weapon_Reload::CheckCurrentBullet(AGun_Base* CurrentWeapon, float DT_BulletArmo, FName BulletName)
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	float Changevalue = CurrentWeapon->GetWeaponParts()[2].ChangeValue;

	FName BulletType = CheckBulletTypeName(BulletName);
// 	UE_LOG(LogTemp, Error, TEXT("%s"), *BulletType.ToString())

	if (PlayerCharacter->GetInventoryComponent()->FindItemSlot(BulletType) == -1)
	{
		return false;
	}
	
	if (CurrentWeapon->GetEquippedMag())
	{
		if (CurrentWeapon->GetBulletArmo() == DT_BulletArmo + Changevalue) return false;
		if (CurrentWeapon->GetBulletArmo() != DT_BulletArmo + Changevalue) return true;
		return false;
	}
	else
	{
		if (CurrentWeapon->GetBulletArmo() == DT_BulletArmo) return false;
		if (CurrentWeapon->GetBulletArmo() != DT_BulletArmo) return true;
		return false;
	}
	
}

FName UGA_Weapon_Reload::CheckBulletTypeName(FName BulletName)
{

	
	if (BulletName == "EBulletType::B_7_62") return "7.62mm";
	if (BulletName == "EBulletType::B_5_56") return "5.56mm";
	if (BulletName == "EBulletType::B_9") return "9mm";
	if (BulletName == "EBulletType::B_12G") return "12G";
	return "none";
}

bool UGA_Weapon_Reload::CheckBullet_SG(AGun_Base* CurrentWeapon, float DT_BulletArmo, FName BulletName)
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	FName BulletType = CheckBulletTypeName(BulletName);

	if (PlayerCharacter->GetInventoryComponent()->FindItemSlot(BulletType) == -1)
	{
		return false;
	}

	if (CurrentWeapon->GetBulletArmo() == DT_BulletArmo)
	{
		return false;
	}
	return true;
}

void UGA_Weapon_Reload::ReloadLooping(AGun_Base* CurrentWeapon, FName BulletName)
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	FName BulletType = CheckBulletTypeName(BulletName);
	
	//UE_LOG(LogTemp, Error, TEXT("Check_bulletname : %s"), *BulletName.ToString())
	//UE_LOG(LogTemp, Error, TEXT("Check_bulletType : %s"), *BulletType.ToString())
	
	if (PlayerCharacter->GetInventoryComponent()->FindItemSlot(BulletType) != -1)
	{
		CurrentWeapon->SetBulletArom(CurrentWeapon->GetBulletArmo() + 1.0f);
		PlayerCharacter->GetInventoryComponent()->ServerRemoveItem(	PlayerCharacter->GetInventoryComponent()->FindItemSlot(BulletType),1);
		// UE_LOG(LogTemp, Error, TEXT("trueeeeeee"));
	}
	else
	{
	// 	UE_LOG(LogTemp, Error, TEXT("falseeeeeeeeeeeeeeeeeeeeeeee"));
	}
}

void UGA_Weapon_Reload::ReloadEnd(AGun_Base* CurrentWeapon , FName BulletName)
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	FName BulletType = CheckBulletTypeName(BulletName);
	
	if (PlayerCharacter->GetInventoryComponent()->FindItemSlot(BulletType) != -1)
	{
		CurrentWeapon->SetBulletArom(CurrentWeapon->GetBulletArmo() + 1.0f);
		PlayerCharacter->GetInventoryComponent()->ServerRemoveItem(	PlayerCharacter->GetInventoryComponent()->FindItemSlot(BulletType),1);
	}
}

void UGA_Weapon_Reload::CheckMaxReloadBullet(AGun_Base* CurrentWeapon, float Maxbullet)
{
	if (Maxbullet == (CurrentWeapon->GetBulletArmo() + 1.0f))
	{
		MontageJumpToSection("ReloadEnd");
	}
}
