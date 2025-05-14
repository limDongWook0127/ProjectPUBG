// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Lobby/SessionBrowserWidget.h"
#include "Widgets/Lobby/BaseButtonWidget.h"

void USessionBrowserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	Button_Back->SetText(TEXT("Back"));	
}

void USessionBrowserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Back->OnButtonClicked.AddDynamic(this, &USessionBrowserWidget::OnBackButton_Clicked);
}

void USessionBrowserWidget::OnBackButton_Clicked()
{
	OnBackButtonClicked.Broadcast();
}
