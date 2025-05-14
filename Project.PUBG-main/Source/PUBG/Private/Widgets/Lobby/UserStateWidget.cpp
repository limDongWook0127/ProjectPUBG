// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Lobby/UserStateWidget.h"

#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "PlayerState/LobbyPlayerState.h"

void UUserStateWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	//Panel_ReadyState->SetVisibility(ESlateVisibility::Hidden);
}

void UUserStateWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUserStateWidget::SetReadyState(bool IsReady)
{
	
}

void UUserStateWidget::InitializeUserState(ALobbyPlayerState* LobbyPlayerState)
{
	if (LobbyPlayerState)
	{		
		if (LobbyPlayerState->IsHost())
		{
			Image_Host->SetVisibility(ESlateVisibility::Visible);
			Text_PlayerName->SetText(FText::FromString(TEXT("HOST")));
		}
		else
		{
			Image_Host->SetVisibility(ESlateVisibility::Hidden);
			Text_PlayerName->SetText(FText::FromString(TEXT("Player")));
		}


		if (LobbyPlayerState->bIsReady)
		{
			Panel_ReadyState->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Panel_ReadyState->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
}
