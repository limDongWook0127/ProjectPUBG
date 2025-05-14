// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

	
public:
	ABaseGameMode();

	virtual void StartPlay() override; // 게임 시작 시 호출
	virtual void BeginPlay() override; 
	virtual void PostLogin(APlayerController* NewPlayer) override; // 플레이어 접속 시 호출
	virtual void Logout(AController* Exiting) override; // 플레이어 접속 해제 시 호출

	void UpdatePlayerCount() const;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameMode", meta = (AllowPrivateAccess = "true"))
	bool GameStart;
};

