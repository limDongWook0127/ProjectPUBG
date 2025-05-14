// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Lobby/ServerBrowserItemWidget.h"

#include "Widgets/Lobby/BaseButtonWidget.h"
#include "Components/TextBlock.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"

#include "BaseLibrary/BaseDebugHelper.h"
#include "Kismet/GameplayStatics.h"

void UServerBrowserItemWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	Button_Join->SetText(TEXT("Join"));
}

void UServerBrowserItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Join->OnButtonClicked.AddDynamic(this, &UServerBrowserItemWidget::OnJoinButton_Clicked);
}

void UServerBrowserItemWidget::OnJoinButton_Clicked()
{
	if (IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
									FOnJoinSessionCompleteDelegate::CreateUObject(this, &UServerBrowserItemWidget::OnJoinSessionsComplete)
								);
		
		if (SessionInterface.IsValid())
		{
			FString ServerName = TEXT("Unknown");			
			if (SearchResult.Session.SessionSettings.Get(FName("SESSION_NAME"), ServerName))
			{
				bool JoinSuccess = SessionInterface->JoinSession(0, NAME_GameSession, SearchResult);
				
				if (JoinSuccess)
				{
					Debug::Print(TEXT("Join Success"));
				}
				else
				{
					Debug::Print(TEXT("Join Failed"));
				}
			}
		}
	}	
}

void UServerBrowserItemWidget::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{	
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionsComplete"));
	
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		if (IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
		{
			IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
			if (SessionInterface.IsValid())
			{
				FString ConnectInfo;
				if (SessionInterface->GetResolvedConnectString(SessionName, ConnectInfo))
				{
					UE_LOG(LogTemp, Warning, TEXT("Connecting to Server: %s"), *ConnectInfo);

					// 서버의 맵으로 이동 (서버 주소를 포함)
					APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
					if (PlayerController)
					{
						PlayerController->ClientTravel(ConnectInfo, ETravelType::TRAVEL_Absolute);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to get resolved connection string."));
				}				
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to join session."));
	}
}

void UServerBrowserItemWidget::SetSession(const FOnlineSessionSearchResult& _SearchResult)
{	
	SearchResult = _SearchResult;

	// 서버 이름 가져오기
	FString ServerName = TEXT("Unknown");
	if (SearchResult.Session.SessionSettings.Get(FName("SESSION_NAME"), ServerName))
	{	
	}
	Text_SessionName->SetText(FText::FromString(ServerName));

	// 현재 참가한 인원 계산
	int32 CurrentPlayers = SearchResult.Session.SessionSettings.NumPublicConnections +
						   SearchResult.Session.SessionSettings.NumPrivateConnections -
						   SearchResult.Session.NumOpenPublicConnections -
						   SearchResult.Session.NumOpenPrivateConnections;
	// 최대 인원 가져오기
	int32 MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections +
					   SearchResult.Session.SessionSettings.NumPrivateConnections;
	FText PlayerText = FText::Format(NSLOCTEXT("GameNamespace", "PlayerCountFormat", "{0} / {1} Players"),
								 FText::AsNumber(CurrentPlayers), FText::AsNumber(MaxPlayers));
	Text_Players->SetText(PlayerText);
	
	
	
	// 핑 값 가져오기
	int32 Ping = SearchResult.PingInMs;	
	FText PingText = FText::Format(NSLOCTEXT("GameNamespace", "PingFormat", "{0} ms"),
								   FText::AsNumber(Ping));
	Text_Ping->SetText(PingText);
}
