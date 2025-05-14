// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/LobbyPlayerState.h"

#include "Controller/LobbyPlayerController.h"
#include "GameState/LobbyGameState.h"
#include "Net/UnrealNetwork.h"

ALobbyPlayerState::ALobbyPlayerState()
{
	PlayerName = GetPlayerName();
	bReplicates = true;

	NetUpdateFrequency = 100.0f; // 기본값: 1~30, 값을 높이면 리플리케이션이 더 자주 발생
	MinNetUpdateFrequency = 30.0f;
}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ALobbyPlayerState, PlayerName);
    DOREPLIFETIME(ALobbyPlayerState, bIsReady);
    DOREPLIFETIME(ALobbyPlayerState, bIsHost);
}

bool ALobbyPlayerState::IsHost() const
{
	return bIsHost;
}

void ALobbyPlayerState::OnRep_Player_Name()
{
}

void ALobbyPlayerState::OnRep_IsReady()
{
	UE_LOG(LogTemp, Log, TEXT("Client : bIsReady : %s"), bIsReady ? TEXT("True") : TEXT("False"));

	if (ALobbyGameState* GS = Cast<ALobbyGameState>( GetWorld()->GetGameState()))
	{
		const int Index = GS->PlayerList.Find((this));
		
		if (Index != -1)
		{
			GS->SetIndexPlayerToList(this, Index);
		}
	}
}

void ALobbyPlayerState::OnRep_IsHost() const
{
	UE_LOG(LogTemp, Log, TEXT("Client : bIsHost : %s"), bIsHost ? TEXT("True") : TEXT("False"));
}

void ALobbyPlayerState::ServerSetReady_Implementation()
{
	bIsReady = !bIsReady;
	OnRep_IsReady();
}

bool ALobbyPlayerState::ServerSetReady_Validate()
{
	return true;
}
