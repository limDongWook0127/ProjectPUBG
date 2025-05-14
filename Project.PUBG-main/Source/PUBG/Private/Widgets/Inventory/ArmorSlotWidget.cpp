// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/ArmorSlotWidget.h"

#include "Component/Equipped/EquippedComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UArmorSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Image_Normal->SetVisibility(ESlateVisibility::Collapsed);
	Image_Hovered->SetVisibility(ESlateVisibility::Collapsed);
	Image_Pressed->SetVisibility(ESlateVisibility::Collapsed);
	Image_Disabled->SetVisibility(ESlateVisibility::Collapsed);
	Image_Armor->SetVisibility(ESlateVisibility::Collapsed);
	
	Button_Armor->OnHovered.AddDynamic(this, &UArmorSlotWidget::OnButton_ArmorSlotHovered);
	Button_Armor->OnUnhovered.AddDynamic(this, &UArmorSlotWidget::OnButton_ArmorSlotUnHovered);
	UE_LOG(LogTemp, Warning, TEXT("UArmorSlotWidget::NativeConstruct"));
	
}

FReply UArmorSlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);

	UE_LOG(LogTemp, Warning, TEXT("UArmorSlotWidget::NativeOnPreviewMouseButtonDown"));

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (EquippedComponent)
		{
			EquippedComponent->ServerDropArmor(ArmorIndex);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UArmorSlotWidget::NativeOnPreviewMouseButtonDown = EquippedComponent None"));
		}
	}
	return FReply::Handled();
}

void UArmorSlotWidget::InitArmorSlot()
{
	Image_Normal->SetVisibility(ESlateVisibility::Visible);
	Image_Hovered->SetVisibility(ESlateVisibility::Collapsed);
	Image_Pressed->SetVisibility(ESlateVisibility::Collapsed);
	Image_Disabled->SetVisibility(ESlateVisibility::Collapsed);
	Image_Armor->SetVisibility(ESlateVisibility::Collapsed);
}

void UArmorSlotWidget::OnButton_ArmorSlotHovered()
{
	UE_LOG(LogTemp, Warning, TEXT("UArmorSlotWidget::OnButton_ArmorSlotHovered"));
	Image_Normal->SetVisibility(ESlateVisibility::Collapsed);
	Image_Hovered->SetVisibility(ESlateVisibility::Visible);
}

void UArmorSlotWidget::OnButton_ArmorSlotUnHovered()
{
	Image_Normal->SetVisibility(ESlateVisibility::Visible);
	Image_Hovered->SetVisibility(ESlateVisibility::Collapsed);
}
