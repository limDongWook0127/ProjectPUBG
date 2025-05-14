// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/LobbyGameState.h"

#include "Controller/LobbyPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/LobbyPlayerState.h"

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyGameState, PlayerList);
}

void ALobbyGameState::AddPlayerToList(ALobbyPlayerState* PlayerState)
{	
	PlayerList.Add(PlayerState);
	ForceNetUpdate();	
	OnRep_PlayerList();
}

void ALobbyGameState::RemovePlayerToList(ALobbyPlayerState* PlayerState)
{
	PlayerList.Remove(PlayerState);
	ForceNetUpdate();	
	OnRep_PlayerList();
}

void ALobbyGameState::SetIndexPlayerToList(ALobbyPlayerState* PlayerState, int Index)
{
	PlayerList[Index] = PlayerState;
	OnRep_PlayerList();
}

void ALobbyGameState::OnRep_PlayerList()
{
	// 로비 위젯 업데이트
	// 게임 상태에서 모든 플레이어가 준비되었는지 확인	
	//CheckAllPlayersReady();	
	UpdateLobbyWidget();
}

void ALobbyGameState::UpdateLobbyWidget()
{
	for (ALobbyPlayerState* PS : PlayerList)
	{
		if (PS && PS->GetOwner())  // PlayerState가 유효한 경우
		{
			ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(PS->GetOwner());
			if (PC && PC->IsLocalController())  // 로컬 컨트롤러 확인
			{
				PC->UpdateWidget();
			}
		}
	}
}

bool ALobbyGameState::CheckAllPlayersReady()
{
	bool bAllReady = true;

	for (ALobbyPlayerState* PS : PlayerList)
	{
		if (PS->bIsHost == false)
		{
			if (!PS->bIsReady)
			{
				bAllReady = false;
				break;
			}				
		}	
	}

	return bAllReady;
}

