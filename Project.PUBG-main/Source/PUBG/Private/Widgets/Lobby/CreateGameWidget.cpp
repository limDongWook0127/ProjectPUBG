// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Lobby/CreateGameWidget.h"

#include "Components/CheckBox.h"
#include "Components/EditableText.h"
#include "Widgets/Lobby/BaseButtonWidget.h"
#include "Controller/LobbyPlayerController.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"

UCreateGameWidget::UCreateGameWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UCreateGameWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	Button_Create->SetText(TEXT("Create"));	
}

void UCreateGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Create->OnButtonClicked.AddDynamic(this, &UCreateGameWidget::OnCreateButton_Clicked);
	Text_MaxPlayer->OnTextChanged.AddDynamic(this, &UCreateGameWidget::OnText_MaxPlayer_Changed);
	Text_SessionName->OnTextChanged.AddDynamic(this, &UCreateGameWidget::OnText_SessionName_Changed);
	CheckBox_EnableLan->OnCheckStateChanged.AddDynamic(this, &UCreateGameWidget::OnCheckState_Changed);		
}

void UCreateGameWidget::OnCreateButton_Clicked()
{	
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsLANMatch = bIsUseLan;  // LAN 테스트용 (Steam/EOS는 false)
			SessionSettings.bIsDedicated = false; // 리슨 서버
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bUsesPresence = true;
			SessionSettings.NumPublicConnections = MaxPlayer;
			SessionSettings.bAllowJoinInProgress = true;
			FString ServerName = SessionName;
			SessionSettings.Set(FName("SESSION_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineService);		
			FString MapName = OpenLevelName.ToString();	
			SessionSettings.Set(FName("MAP_NAME"), MapName, EOnlineDataAdvertisementType::ViaOnlineService);
			
	
			bool IsCreate = SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
	
			if (IsCreate)
			{
				UGameplayStatics::OpenLevel(GetWorld(), OpenLevelName, true, FString("listen"));
			}
		}
	}
}

void UCreateGameWidget::OnText_MaxPlayer_Changed(const FText& Text)
{
	FString Temp = Text.ToString();
	
	if (Temp.IsNumeric())
	{
		MaxPlayer = FCString::Atoi(*Temp);
	}
}

void UCreateGameWidget::OnText_SessionName_Changed(const FText& Text)
{
	SessionName = Text.ToString();
}

void UCreateGameWidget::OnCheckState_Changed(bool IsChecked)
{
	bIsUseLan = IsChecked;
}

