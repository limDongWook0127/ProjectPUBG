// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/BasePlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "PlayerState/BasePlayerState.h"
#include "BaseGameState.generated.h"

class UPlayerGameplayAbility;

/**
 * 
 */
UCLASS()
class PUBG_API ABaseGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ABaseGameState();
	ABasePlayerController* GetLocalController();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetGameStartNotification(bool bNewValue);
	void SetBoardPlaneNotification(bool bNewValue);

	UFUNCTION()
	void OnRep_PlayerCount();  // UI 갱신
	UFUNCTION()
	void OnRep_RemainingTime();  // UI 갱신
	// 게임 시작 알림을 위한 리플리케이션 함수
	UFUNCTION()
	void OnRep_GameStartNotification();
	// 비행기 탑승 알림을 위한 리플리케이션 함수
	UFUNCTION()
	void OnRep_BoardPlaneNotification();
	UFUNCTION()
	void OnRep_CurrentZoneScale();
	UFUNCTION()
	void OnRep_IsVisibiltyNextZone();
	UFUNCTION()
	void OnRep_CurrentAirplaneLocation();
	UFUNCTION()
	void OnRep_IsVisibiltyAirplane();
	UFUNCTION()
	void OnRep_IsVisibiltyRedZone();
	UFUNCTION()
	void OnRep_LandScapeBoundingBoxXY();
	
	void UpdatePlayerCountWidget();
	void ShowNotification(FText Message);
	
	// 게임 상태 업데이트
	void UpdateRemainingTime(int32 NewTime);
	void SetAllPlayerCount();
	void UpdateLandScapeBoundingBoxXY(FBox NewLandScapeBoundingBox);
	void UpdateCurrentZone(FVector NewCurrentZoneCenter, float NewCurrentZoneScale);
	void UpdateNextZone(FVector NewNextZoneCenter, float NewNextZoneScale, bool NewbIsVisibiltyNextZone);
	void UpdateCurrentAirplaneLocation(FVector NewCurrentAirplaneLocation);
	void UpdateIsVisibiltyAirplane(FVector NewStartAirplanePoint, FVector NewEndAirplanePoint, bool NewbIsVisibiltyAirplane);
	void UpdateRedZone(FVector NewRedZoneCenter, float NewRedZoneScale, bool NewbIsVisibiltyRedZone);
	void FinishMoveAirplane();
	int32 GetAlivePlayers();

	void FinishGame();
	// 플레이어 수 업데이트
	void UpdatePlayerCount();
	int32 GetAllPlayerCount() const;
	int32 GetCurrentPlayerCount() const;
	
	bool GetIsGameStarted() const;
	FBox GetLandScapeBoundingBox() const;

	void Initialize();

//private:
	// 게임 시간
	UPROPERTY(ReplicatedUsing=OnRep_RemainingTime, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	int32 RemainingTime;

	// 게임에 참여한 플레이어 수
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	int32 AllPlayerCount;

	// 플레이어 수
	UPROPERTY(ReplicatedUsing=OnRep_PlayerCount, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	int32 CurrentPlayerCount;
		
	// 게임 시작 알림 (리플리케이션)
	UPROPERTY(ReplicatedUsing = OnRep_GameStartNotification, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	bool bIsGameStarted;

	// 비행기 탑승 알림 (리플리케이션)
	UPROPERTY(ReplicatedUsing = OnRep_BoardPlaneNotification, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	bool bIsPlaneBoarding;
	
	// 불러온 랜드스케이프 정보
	UPROPERTY(ReplicatedUsing = OnRep_LandScapeBoundingBoxXY, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	FBox LandScapeBoundingBox;

	/** 현재 자기장의 중심 위치 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	FVector CurrentZoneCenter;

	/** 현재 자기장의 반지름 */
	UPROPERTY(ReplicatedUsing=OnRep_CurrentZoneScale, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	float CurrentZoneScale;

	/** 다음 자기장의 중심 위치 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	FVector NextZoneCenter;

	/** 다음 자기장의 반지름 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	float NextZoneScale;
	
	// 다음 원을 표시할 것인지
	UPROPERTY(ReplicatedUsing = OnRep_IsVisibiltyNextZone, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	bool bIsVisibiltyNextZone;
	
	/** 현재 비행기의 위치 */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentAirplaneLocation, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	FVector CurrentAirplaneLocation;

	/** 비행기의 시작점 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	FVector StartAirplanePoint;
	/** 비행기의 끝점 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	FVector EndAirplanePoint;
	
	// 비행기를 표시할 것인지
	UPROPERTY(ReplicatedUsing = OnRep_IsVisibiltyAirplane, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	bool bIsVisibiltyAirplane;
	
	/** 레드존 센터 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	FVector RedZoneCenter;

	/** 레드존 스케일 */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	float RedZoneScale;
	
	// 레드존을 표시할 것인지
	UPROPERTY(ReplicatedUsing = OnRep_IsVisibiltyRedZone, BlueprintReadOnly, Category = "Game State", meta = (AllowPrivateAccess = "true"))
	bool bIsVisibiltyRedZone;
	
	bool StartNotify;

	UPROPERTY()
	ABasePlayerController* LocalController;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TSubclassOf<UPlayerGameplayAbility> FallAbility;
};

