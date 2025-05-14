// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BaseGameMode.h"

#include "GameInstance/BaseGameInstance.h"
#include "GameState/BaseGameState.h"
#include "Manager/LandscapeManager.h"
#include "Manager/TimeManager.h"

ABaseGameMode::ABaseGameMode()
{
}

void ABaseGameMode::StartPlay()
{
	Super::StartPlay();
}

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UBaseGameInstance* GI = GetGameInstance<UBaseGameInstance>();
	
	if (GI )
	{
		GI->GetLandscapeManager()->FindLandscape();
	}
}

void ABaseGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (HasAuthority() && GameStart) // 서버에서만 실행
	{
		ABaseGameState* GS = Cast<ABaseGameState>(GameState);
		if (GS && !GS->GetIsGameStarted())
		{
			UBaseGameInstance* GI = GetGameInstance<UBaseGameInstance>();

			if (GI)
			{
				// 게임 시작 타이머 실행
				GI->GetTimeManager()->StartGameTimer();
			}
		}	
	}

	
	UpdatePlayerCount();
}

void ABaseGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	UpdatePlayerCount();
}

void ABaseGameMode::UpdatePlayerCount() const
{
	ABaseGameState* BaseGameState = Cast<ABaseGameState>(GameState);
	BaseGameState->UpdatePlayerCount();
}
