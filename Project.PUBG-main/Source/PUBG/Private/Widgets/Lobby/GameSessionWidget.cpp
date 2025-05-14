// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Lobby/GameSessionWidget.h"
#include "Components/HorizontalBox.h"
#include "Widgets/Lobby/BaseButtonWidget.h"
#include "Widgets/Lobby/UserStateWidget.h"

#include "GameState/LobbyGameState.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Components/EditableText.h"
#include "Components/GridPanel.h"
#include "Controller/LobbyPlayerController.h"
#include "GameMode/LobbyGameMode.h"
#include "PlayerState/LobbyPlayerState.h"


void UGameSessionWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Button_Quit->SetText(TEXT("Quit"));
}

void UGameSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	Button_Quit->OnButtonClicked.AddDynamic(this, &UGameSessionWidget::UGameSessionWidget::OnQuitButton_Clicked);

	APlayerController* PC = GetOwningPlayer();
	if (PC->HasAuthority())
	{
		Button_Ready->SetText(TEXT("Start"));
		Button_Ready->OnButtonClicked.AddDynamic(this, &UGameSessionWidget::UGameSessionWidget::OnStartButton_Clicked);
		Panel_ConnectIP->SetVisibility(ESlateVisibility::Visible);
		Text_ConnectIP->OnTextChanged.AddDynamic(this,  &UGameSessionWidget::UGameSessionWidget::UGameSessionWidget::OnText_ConnectIP_Changed);
		ConnectIP = TEXT("127.0.0.1");
	}
	else
	{
		Button_Ready->SetText(TEXT("Ready"));
		Button_Ready->OnButtonClicked.AddDynamic(this, &UGameSessionWidget::UGameSessionWidget::OnReadyButton_Clicked);		
		Panel_ConnectIP->SetVisibility(ESlateVisibility::Hidden);
	}

	if (IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(FOnDestroySessionCompleteDelegate::CreateUObject(this, &UGameSessionWidget::OnDestroySessionComplete));
			SessionInterface->AddOnSessionFailureDelegate_Handle(FOnSessionFailureDelegate::CreateUObject(this, &UGameSessionWidget::HandleSessionFailure));
		}
	}
}

void UGameSessionWidget::OnStartButton_Clicked()
{
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(GetOwningPlayer());

	if (PC)
	{
		if (PC->HasAuthority())
		{
			UE_LOG(LogTemp, Warning, TEXT("OnStartButton_Clicked"));

			ALobbyGameState* GS = Cast<ALobbyGameState>(GetWorld()->GetGameState());
			if (GS)
			{
				if (GS->CheckAllPlayersReady())
				{
					UE_LOG(LogTemp, Warning, TEXT("All Ready"));
					//PC->MoveToDedicatedServer(ConnectIP, ConnectMap);
					PC->ServerMoveAllClientsToDedicatedServer(ConnectIP, ConnectMap);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Not All Ready"));
				}
			}		
		}
	}
	
}

void UGameSessionWidget::OnReadyButton_Clicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ALobbyPlayerState* PS = PC->GetPlayerState<ALobbyPlayerState>())
		{
			PS->ServerSetReady();
		}
	}
}

void UGameSessionWidget::OnText_ConnectIP_Changed(const FText& Text)
{
	ConnectIP = Text.ToString();
}

void UGameSessionWidget::LeaveSession()
{
	if (IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		
		if (SessionInterface.IsValid())
		{
			SessionInterface->DestroySession(NAME_GameSession);
		}
	}
}

void UGameSessionWidget::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("OnDestroySessionComplete"));
        
		// 클라이언트를 로비나 메인 메뉴로 이동
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			PC->ClientTravel(LeaveMapName, TRAVEL_Absolute);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionFailed"));
	}
}

void UGameSessionWidget::HandleSessionFailure(const FUniqueNetId& UserId, ESessionFailure::Type FailureType)
{
	if (FailureType == ESessionFailure::ServiceConnectionLost)
	{
		UE_LOG(LogTemp, Warning, TEXT("Network error detected. Leaving session..."));
		LeaveSession();
	}
}

void UGameSessionWidget::OnQuitButton_Clicked()
{
	LeaveSession();
}

void UGameSessionWidget::UpdatePlayerList(TArray<ALobbyPlayerState*> PlayerList)
{
	APlayerController* PC = GetOwningPlayer();
	
	UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerList"));
	HorizontalBox_PlayerList->ClearChildren(); // 기존 리스트 제거
	for (ALobbyPlayerState* PS : PlayerList)
	{
		UUserStateWidget* UserStateWidget = CreateWidget<UUserStateWidget>(PC, UserStateWidgetClass);
		UserStateWidget->InitializeUserState(PS);
		HorizontalBox_PlayerList->AddChild(UserStateWidget);
	}
}




