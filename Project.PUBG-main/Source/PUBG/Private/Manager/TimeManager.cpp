// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/TimeManager.h"

#include "GameInstance/BaseGameInstance.h"
#include "GameState/BaseGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/AirplaneManager.h"
#include "Manager/GameEventManager.h"
#include "Manager/ZoneManager.h"

UTimeManager::UTimeManager()
{
	TickTime = 1.f;
}

void UTimeManager::InitializeManager()
{
	UE_LOG(LogTemp, Log, TEXT("TimeManager Initialized"));
	
	GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);
	
	TickTime = 1.f;
	CurrentGameTime = StartGameTime;
}

void UTimeManager::StartGameTimer()
{
	NotifyClientsToStartGame();
}

void UTimeManager::CountDownGameTimer()
{
	// 게임 시작 타이머 시작 (예: 5분 후 게임 시작)
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &UTimeManager::GameStartCountdown, TickTime, true, 0.0f);
}

void UTimeManager::NotifyClientsToStartGame()
{
	// 서버에서 게임 시작 알림을 게임 스테이트에 전달
	ABaseGameState* GameState = GetWorld()->GetGameState<ABaseGameState>();
	if (GameState && GameState->HasAuthority())
	{
		GameState->SetGameStartNotification(true); // 게임 시작 알림 설정
		CountDownGameTimer();
	}
}

void UTimeManager::NotifyClientsToBoardPlane()
{
	// 서버에서 비행기 탑승 알림을 게임 스테이트에 전달
	ABaseGameState* GameState = GetWorld()->GetGameState<ABaseGameState>();
	if (GameState && GameState->HasAuthority())
	{
		GameState->SetBoardPlaneNotification(true); // 비행기 탑승 알림 설정
	}
}

void UTimeManager::NotifyClientsToArrivePlane()
{
	ABaseGameState* GameState = GetWorld()->GetGameState<ABaseGameState>();
	if (GameState && GameState->HasAuthority())
	{
		GameState->FinishMoveAirplane(); 
	}
	
	UBaseGameInstance* GI = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GI)
	{
		// 자기장 생성
		UZoneManager* ZoneManager = GI->GetZoneManager();
		if (ZoneManager && ActivateZone)
		{
			ZoneManager->SpawnZone();
		}

		// 이벤트 생성
		UGameEventManager* GameEventManager  = GI->GetGameEventManager();
		if (GameEventManager && ActivateGameEvent)
		{
			GameEventManager->StartGameEventTimer();
		}
	}
}

void UTimeManager::GameStartCountdown()
{
	CurrentGameTime -= TickTime;

	ABaseGameState* GS = GetWorld()->GetGameState<ABaseGameState>();
	if (GS && GS->HasAuthority())
	{
		GS->UpdateRemainingTime(CurrentGameTime); // 비행기 탑승 알림 설정
		
		if (CurrentGameTime <= 0)
		{		
			GS->SetAllPlayerCount();
			InitializeManager();
			StartGameFlow();
		}
	}
}

void UTimeManager::StartGameFlow()
{	
	UBaseGameInstance* GI = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GI)
	{
		
		NotifyClientsToBoardPlane();
        // 비행기 탑승
		UAirplaneManager* AirplaneManager = GI->GetAirplaneManager();
		if (AirplaneManager)
		{
			AirplaneManager->NotifyStartToMoveAirplane();
		}
	}	
}
