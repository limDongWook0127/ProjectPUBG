// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/WeaponSlotWidget.h"

#include "Components/SizeBox.h"
#include "Weapon/DataTable/DT_Weapon.h"
#include "Weapon/Guns/Gun_Base.h"

FReply UWeaponSlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);

	UE_LOG(LogTemp, Warning, TEXT("UWeaponSlotWidget::NativeOnPreviewMouseButtonDown"));

	return FReply::Handled();
}

FString UWeaponSlotWidget::SetBulletTypeTextBlock(EBulletType OutEBulletType)
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

void UWeaponSlotWidget::FindEquiablePartsSlot(AGun_Base* OutGun)
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
