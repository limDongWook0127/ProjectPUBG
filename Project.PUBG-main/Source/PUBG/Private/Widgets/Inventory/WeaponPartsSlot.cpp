// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/WeaponPartsSlot.h"

#include "Component/Inventory/InventoryComponent.h"
#include "Components/Image.h"
#include "Weapon/Guns/Gun_Base.h"


void UWeaponPartsSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UE_LOG(LogTemp, Warning, TEXT("UWeaponPartsSlot::NativeOnInitialized"));
	Image_Hovered->SetVisibility(ESlateVisibility::Hidden);
	Image_Pressed->SetVisibility(ESlateVisibility::Hidden);
	Image_Disabled->SetVisibility(ESlateVisibility::Hidden);
	Image_Parts->SetVisibility(ESlateVisibility::Hidden);
	Image_Equipable->SetVisibility(ESlateVisibility::Hidden);
	//Image_Normal->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UWeaponPartsSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (Gun)
		{
			FName PartsName = Gun->GetWeaponParts()[PartsIndex].PartsName;
			FString PartsNameStr = PartsName.ToString();
			
			FString GunName = Gun->GetWeaponDataAsset().GunName;
			PartsNameStr.ReplaceInline(*FString(TEXT("_") + GunName), TEXT(""));

			FName NewPartsName = FName(*PartsNameStr);

			int32 Weight = Gun->GetWeaponParts()[PartsIndex].Weight;
			EItemCategory Category = Gun->GetWeaponParts()[PartsIndex].ItemCategory;

			UE_LOG(LogTemp, Warning, TEXT("UWeaponPartsSlot::NativeOnMouseButtonDown = PartsIndex %d"), PartsIndex);
			UE_LOG(LogTemp, Warning, TEXT("UWeaponPartsSlot::NativeOnMouseButtonDown = NewPartsName %s"), *NewPartsName.ToString());
			UE_LOG(LogTemp, Warning, TEXT("UWeaponPartsSlot::NativeOnMouseButtonDown = GunName %s"), *Gun->GetActorNameOrLabel());
			
			
 			Gun->GetWeaponParts()[PartsIndex].StaticMesh = nullptr;
 			Gun->GetWeaponParts()[PartsIndex].PartsName = NAME_None;
			Gun->ServerSetParts(PartsIndex, NAME_None, -1.0f, EItemCategory::FullBody, nullptr, -1.0f);
			 
			InventoryComponent->ServerAddToInventory(NewPartsName, 1, Weight, Category);
		}
	}

	
	return FReply::Handled();
}

// FReply UWeaponPartsSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
// {
// 	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
//
// 	UE_LOG(LogTemp, Warning, TEXT("UWeaponPartsSlot::NativeOnPreviewMouseButtonDown"));
//
//
// 	return FReply::Handled();
// }
