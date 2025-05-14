// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/WeaponSlotWidget1.h"

#include "Component/Equipped/EquippedComponent.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Weapon/DataTable/DT_Weapon.h"
#include "Weapon/Guns/Gun_Base.h"
#include "Widgets/Inventory/WeaponPartsSlot.h"

FReply UWeaponSlotWidget1::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry,
                                                          const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	
	UE_LOG(LogTemp, Warning, TEXT("UWeaponSlotWidget1::NativeOnPreviewMouseButtonDown1"));

	
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWeaponSlotWidget1::NativeOnPreviewMouseButtonDown = EKeys::RightMouseButton"));
		
		if (!EquippedComponent)
		{
			return FReply::Unhandled();
		}
		
		if (WeaponIndex == 0 || WeaponIndex == 1)
		{
			AGun_Base* EquippedWeapon = Cast<AGun_Base>(EquippedComponent->GetEquippedItems()[WeaponIndex]);
			EquippedComponent->ServerDropMainWeapon(EquippedWeapon, WeaponIndex);
			//EquippedComponent->DropMainWeapon(EquippedWeapon, WeaponIndex);
		}
		else if (WeaponIndex == 2)
		{
			AGun_Base* EquippedWeapon = Cast<AGun_Base>(EquippedComponent->GetEquippedItems()[WeaponIndex]);
			EquippedComponent->ServerDropSUbWeapon(EquippedWeapon);
			//EquippedComponent->DropSUbWeapon(EquippedWeapon);
		}
	}

	return FReply::Unhandled();
}

void UWeaponSlotWidget1::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WBP_WeaponPartsSlot_Sight->SetPartsIndex(1);
	WBP_WeaponPartsSlot_Magazine->SetPartsIndex(2); 
	WBP_WeaponPartsSlot_Barrel->SetPartsIndex(3);
}

void UWeaponSlotWidget1::SetTextureParameter(UTextureRenderTarget2D* OutImage) const
{
	UMaterialInstanceDynamic* MaterialInstanceDynamic = Image_RenderTargetWeapon->GetDynamicMaterial();

	MaterialInstanceDynamic->SetTextureParameterValue(TEXT("RenderTarget"), OutImage);
	Image_RenderTargetWeapon->SetBrushFromMaterial(MaterialInstanceDynamic);
}

FString UWeaponSlotWidget1::SetBulletTypeTextBlock(EBulletType OutEBulletType)
{
	switch (OutEBulletType)
	{
	case EBulletType::B_7_62:
		return TEXT("7.62mm");
	case EBulletType::B_5_56:
		return TEXT("5.56mm");
	case EBulletType::B_9:
		return TEXT("9mm");
	case EBulletType::B_12G:
		return TEXT("12G");
	default:
		return TEXT("None");
	}
}

void UWeaponSlotWidget1::FindEquiablePartsSlot(AGun_Base* OutGun)
{
	FWeaponData WeaponData = OutGun->GetWeaponDataAsset();

	if (WeaponData.Installable_Parts.Muzzle == true)
	{
		SizeBox_Barrel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SizeBox_Barrel->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (WeaponData.Installable_Parts.Grip == true)
	{
		SizeBox_Grip->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SizeBox_Grip->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (WeaponData.Installable_Parts.Mag == true)
	{
		SizeBox_Magazine->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SizeBox_Magazine->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (WeaponData.Installable_Parts.Scope == true)
	{
		SizeBox_Sight->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SizeBox_Sight->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (WeaponData.Installable_Parts.Loops == true)
	{
		SizeBox_Stock->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SizeBox_Stock->SetVisibility(ESlateVisibility::Collapsed);
	}
}
