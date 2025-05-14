// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ALobbyPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool IsHost() const;

	UFUNCTION()
	static void OnRep_Player_Name();  // UI 갱신

	// 클라이언트 준비 여부
	UFUNCTION()
	void OnRep_IsReady();  // 클라이언트에서 UI 갱신
	UFUNCTION()
	void OnRep_IsHost() const;  // 클라이언트에서 UI 갱신
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetReady(); // 서버에서 준비 상태 변경

	UPROPERTY(ReplicatedUsing=OnRep_Player_Name)
	FString PlayerName;  // 플레이어 이름
	
	UPROPERTY(ReplicatedUsing=OnRep_IsReady)
	bool bIsReady = false;
	UPROPERTY(ReplicatedUsing=OnRep_IsHost)
	bool bIsHost = false;
};
