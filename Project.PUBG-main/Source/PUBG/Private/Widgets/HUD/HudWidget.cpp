// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/HudWidget.h"

#include "Widgets/DisplayMessage/DisplayMessageItemWidget.h"

void UHudWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WBP_DisplayMessageItem->SetVisibility(ESlateVisibility::Collapsed);

	
}
