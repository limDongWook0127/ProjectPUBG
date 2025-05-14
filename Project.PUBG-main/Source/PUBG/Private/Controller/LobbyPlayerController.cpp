// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/LobbyPlayerController.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "GameState/LobbyGameState.h"
#include "Widgets/Lobby/GameSessionWidget.h"


void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())  // 로컬 플레이어만 UI 표시
	{
		ShowLobby();
		UpdateWidget();
	}

	FInputModeUIOnly InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	
	SetInputMode(InputModeData);
	bShowMouseCursor = true;
}

void ALobbyPlayerController::ShowLobby()
{
	if (LobbyWidgetClass)  // 위젯 블루프린트가 설정되어 있으면 생성
	{
		LobbyWidget = CreateWidget<UGameSessionWidget>(this, LobbyWidgetClass);
		if (LobbyWidget)
		{
			LobbyWidget->AddToViewport();
		}
	}
}

void ALobbyPlayerController::UpdateWidget()
{
	if (ALobbyGameState* GS = Cast<ALobbyGameState>( GetWorld()->GetGameState()))
	{
		if (LobbyWidget)
		{
			LobbyWidget->UpdatePlayerList(GS->PlayerList);  // UI 업데이트 함수 호출
		}
	}
}

void ALobbyPlayerController::ServerMoveAllClientsToDedicatedServer_Implementation(const FString& ConnectIP,
                                                                                  const FString& ConnectMap)
{
	DedicatedConnectIP = ConnectIP;
	DedicatedConnectMap = ConnectMap;
	
	// 모든 클라이언트에게 DestroySession을 호출하고, 이동을 시작하게 함
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(It->Get());
		if (PC && !PC->IsLocalController()) // 클라이언트인 경우
		{
			// 클라이언트에서 세션을 종료하고 데디케이티드 서버로 이동
			PC->ClientMoveToDedicatedServer(ConnectIP, ConnectMap);
		}
	}
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ALobbyPlayerController::MoveToDedicatedServer, 1.0f, true);
}

bool ALobbyPlayerController::ServerMoveAllClientsToDedicatedServer_Validate(const FString& ConnectIP,
	const FString& ConnectMap)
{
	return true;
}


void ALobbyPlayerController::ClientMoveToDedicatedServer_Implementation(const FString& ConnectIP,
                                                                        const FString& ConnectMap)
{
	IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->DestroySession(NAME_GameSession);
		}
	}
	FString DedicatedServerAddress = ConnectIP; // 데디케이티드 서버 IP
	FString MapName = ConnectMap; // 데디케이티드 서버에서 실행 중인 맵 이름
	FString TravelURL = FString::Printf(TEXT("%s:7777"), *DedicatedServerAddress);

	ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
}

void ALobbyPlayerController::MoveToDedicatedServer()
{
	ALobbyGameState* GS = Cast<ALobbyGameState>(GetWorld()->GetGameState());
		
	if (GS->PlayerList.Num() == 1)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		
		IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
		if (OnlineSubsystem)
		{
			IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
			if (SessionInterface.IsValid())
			{
				SessionInterface->DestroySession(NAME_GameSession);
			}
		}
	
		FString DedicatedServerAddress = DedicatedConnectIP; // 데디케이티드 서버 IP
		FString MapName = DedicatedConnectMap; // 데디케이티드 서버에서 실행 중인 맵 이름
		FString TravelURL = FString::Printf(TEXT("%s:7777"), *DedicatedServerAddress);

		ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
	}
}
