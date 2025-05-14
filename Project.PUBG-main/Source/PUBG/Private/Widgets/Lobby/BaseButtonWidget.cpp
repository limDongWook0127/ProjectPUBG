// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Lobby/BaseButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


UBaseButtonWidget::UBaseButtonWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Button_Base = nullptr;	
	Text_Base = nullptr;	
}

void UBaseButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UBaseButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Base->OnClicked.AddDynamic(this, &UBaseButtonWidget::OnButton_Clicked);
}

void UBaseButtonWidget::OnButton_Clicked()
{
	OnButtonClicked.Broadcast();
}

void UBaseButtonWidget::SetText(FString Text)
{
	Text_Base->SetText(FText::FromString(Text));
}
