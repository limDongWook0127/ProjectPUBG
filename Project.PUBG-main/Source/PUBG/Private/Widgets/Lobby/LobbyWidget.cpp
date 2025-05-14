// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Lobby/LobbyWidget.h"

#include "Components/WidgetSwitcher.h"
#include "Widgets/Lobby/BaseButtonWidget.h"
#include "Widgets/Lobby/CreateGameWidget.h"
#include "Widgets/Lobby/FindGameWidget.h"

void ULobbyWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	Button_CreateGame->SetText(TEXT("Create"));
	Button_FindGame->SetText(TEXT("Find"));	
	WidgetSwitcher_Session->SetVisibility(ESlateVisibility::Hidden);
}

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_CreateGame->OnButtonClicked.AddDynamic(this, &ULobbyWidget::OnCreateGameButton_Clicked);
	Button_FindGame->OnButtonClicked.AddDynamic(this, &ULobbyWidget::OnFindGameButton_Clicked);
	CreateGameWidget->OnBackButtonClicked.AddDynamic(this, &ULobbyWidget::OnBackButton_Clicked);
	FindGameWidget->OnBackButtonClicked.AddDynamic(this, &ULobbyWidget::OnBackButton_Clicked);
}

void ULobbyWidget::OnCreateGameButton_Clicked()
{
	WidgetSwitcher_Session->SetActiveWidget(CreateGameWidget);
	WidgetSwitcher_Session->SetVisibility(ESlateVisibility::Visible);
}

void ULobbyWidget::OnFindGameButton_Clicked()
{
	WidgetSwitcher_Session->SetActiveWidget(FindGameWidget);
	WidgetSwitcher_Session->SetVisibility(ESlateVisibility::Visible);
}

void ULobbyWidget::OnBackButton_Clicked()
{
	WidgetSwitcher_Session->SetActiveWidget(nullptr);
	WidgetSwitcher_Session->SetVisibility(ESlateVisibility::Hidden);
}
