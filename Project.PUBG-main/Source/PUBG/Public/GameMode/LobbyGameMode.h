// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:	
	virtual void PostLogin(APlayerController* NewPlayer) override; // 플레이어 접속 시 호출
	virtual void Logout(AController* ExitingPlayer) override; // 플레이어 접속 해제 시 호출

};

