// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/BaseGameState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseGameplayTag.h"
#include "Controller/BasePlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameplayActor/Airplane/GA/GA_Airplane_Fall.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/BasePlayerState.h"

ABaseGameState::ABaseGameState()
{
	CurrentPlayerCount = 0;
	Initialize();
}

ABasePlayerController* ABaseGameState::GetLocalController()
{
	if (LocalController)
	{
		return LocalController;
	}
	for (APlayerState* PS : PlayerArray)
	{
		if (PS && PS->GetOwner())  // PlayerState가 유효한 경우f
		{
			ABasePlayerController* PC = Cast<ABasePlayerController>(PS->GetOwner());
			if (PC && PC->IsLocalController())  // 로컬 컨트롤러 확인
			{
				LocalController = PC;
				return LocalController; 
			}
		}
	}

	return nullptr;
}

void ABaseGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Replicated 변수 등록
	DOREPLIFETIME(ABaseGameState, RemainingTime);
	DOREPLIFETIME(ABaseGameState, AllPlayerCount);
	DOREPLIFETIME(ABaseGameState, CurrentPlayerCount);
	DOREPLIFETIME(ABaseGameState, bIsGameStarted);
	DOREPLIFETIME(ABaseGameState, bIsPlaneBoarding);
	DOREPLIFETIME(ABaseGameState, CurrentZoneCenter);
	DOREPLIFETIME(ABaseGameState, CurrentZoneScale);
	DOREPLIFETIME(ABaseGameState, NextZoneCenter);
	DOREPLIFETIME(ABaseGameState, NextZoneScale);
	DOREPLIFETIME(ABaseGameState, bIsVisibiltyNextZone);
	DOREPLIFETIME(ABaseGameState, CurrentAirplaneLocation);
	DOREPLIFETIME(ABaseGameState, StartAirplanePoint);
	DOREPLIFETIME(ABaseGameState, EndAirplanePoint);
	DOREPLIFETIME(ABaseGameState, bIsVisibiltyAirplane);
	DOREPLIFETIME(ABaseGameState, RedZoneCenter);
	DOREPLIFETIME(ABaseGameState, RedZoneScale);
	DOREPLIFETIME(ABaseGameState, bIsVisibiltyRedZone);
	DOREPLIFETIME(ABaseGameState, LandScapeBoundingBox);
}

void ABaseGameState::SetGameStartNotification(bool bNewValue)
{
	if (bIsGameStarted != bNewValue)
	{
		bIsGameStarted = bNewValue;
		OnRep_GameStartNotification();//리슨서버용
	}	
}

void ABaseGameState::SetBoardPlaneNotification(bool bNewValue)
{
	if (bIsPlaneBoarding != bNewValue)
	{
		bIsPlaneBoarding = bNewValue;
		OnRep_BoardPlaneNotification();//리슨서버용
	}
}

void ABaseGameState::OnRep_PlayerCount()
{
	//UE_LOG(LogTemp, Warning, TEXT("OnRep_PlayerCount"));
	UpdatePlayerCountWidget();
}

void ABaseGameState::OnRep_RemainingTime()
{
	//UE_LOG(LogTemp, Warning, TEXT("RemainingTime = %d"), RemainingTime);
	FText Message;
	if (RemainingTime == 0)
	{
		Message = FText::FromString(TEXT("게임 시작합니다!"));	
	}
	else
	{
		Message = FText::FromString(FString::FromInt(RemainingTime) + TEXT("초 남았습니다"));		
	}
	
	ShowNotification(Message);
}

void ABaseGameState::OnRep_GameStartNotification()
{
	if (bIsGameStarted)
	{		
		UE_LOG(LogTemp, Log, TEXT("Game has started!"));
	}
	else if (!bIsGameStarted)
	{				
		UE_LOG(LogTemp, Log, TEXT("Game has finished!"));		
		//GetLocalController()->ShowResultWidget(CurrentPlayerCount);
	}
}

void ABaseGameState::OnRep_BoardPlaneNotification()
{
	//UE_LOG(LogTemp, Log, TEXT("Board the plane!"));
}


void ABaseGameState::OnRep_CurrentZoneScale()
{	
	if (StartNotify)
	{
		StartNotify = false;
		FText Message = FText::FromString(TEXT("자기장이 줄어듭니다"));		
		ShowNotification(Message);
	}

	if (GetLocalController())
	{
		LocalController->UpdateMapCurrentZone();
	}
}

void ABaseGameState::OnRep_IsVisibiltyNextZone()
{	
	if (bIsVisibiltyNextZone)
	{
		FText Message = FText::FromString(TEXT("잠시후 자기장이 줄어듭니다"));		
		ShowNotification(Message);
	}
	else
	{
		StartNotify = true;
	}
	
	if (GetLocalController())
	{
		LocalController->UpdateMapNextZone();
	}
}

void ABaseGameState::OnRep_CurrentAirplaneLocation()
{
	if (GetLocalController())
	{
		LocalController->UpdateAirplanebLocation();
	}
}

void ABaseGameState::OnRep_IsVisibiltyAirplane()
{	
	if (GetLocalController())
	{
		LocalController->UpdateAirplanebVisibilty();
	}
}

void ABaseGameState::OnRep_IsVisibiltyRedZone()
{
	if (GetLocalController())
	{
		LocalController->UpdateRedZone();
		if (bIsVisibiltyRedZone)
		{
			FText Message = FText::FromString(TEXT("레드존이 활성화 됩니다"));		
			ShowNotification(Message);		
		}
	}
}

void ABaseGameState::OnRep_LandScapeBoundingBoxXY()
{
}

void ABaseGameState::UpdatePlayerCountWidget()
{
	if (GetLocalController())
	{
		LocalController->UpdateCurrentPlayer(CurrentPlayerCount);
	}
}

void ABaseGameState::ShowNotification(FText Message)
{
	if (GetLocalController())
	{
		LocalController->ShowNotification(Message);		
	}
}

void ABaseGameState::UpdateRemainingTime(int32 NewTime)
{
	RemainingTime = NewTime;
	OnRep_RemainingTime(); //리슨 서버용
}

void ABaseGameState::SetAllPlayerCount()
{
	AllPlayerCount = PlayerArray.Num();
}

void ABaseGameState::UpdateLandScapeBoundingBoxXY(FBox NewLandScapeBoundingBox)
{
	LandScapeBoundingBox = NewLandScapeBoundingBox;
}

void ABaseGameState::UpdateCurrentZone(FVector NewCurrentZoneCenter, float NewCurrentZoneScale)
{
	CurrentZoneCenter = NewCurrentZoneCenter;
	CurrentZoneScale = NewCurrentZoneScale;
}

void ABaseGameState::UpdateNextZone(FVector NewNextZoneCenter, float NewNextZoneScale, bool NewbIsVisibiltyNextZone)
{
	NextZoneCenter = NewNextZoneCenter;
	NextZoneScale = NewNextZoneScale;
	bIsVisibiltyNextZone = NewbIsVisibiltyNextZone;
}

void ABaseGameState::UpdateCurrentAirplaneLocation(FVector NewCurrentAirplaneLocation)
{
	CurrentAirplaneLocation = NewCurrentAirplaneLocation;
}

void ABaseGameState::UpdateIsVisibiltyAirplane(FVector NewStartAirplanePoint, FVector NewEndAirplanePoint,
	bool NewbIsVisibiltyAirplane)
{
	StartAirplanePoint = NewStartAirplanePoint;
	EndAirplanePoint = NewEndAirplanePoint;
	bIsVisibiltyAirplane = NewbIsVisibiltyAirplane;
}

void ABaseGameState::UpdateRedZone(FVector NewRedZoneCenter, float NewRedZoneScale,
	bool NewbIsVisibiltyRedZone)
{
	RedZoneCenter = NewRedZoneCenter;
	RedZoneScale = NewRedZoneScale;
	bIsVisibiltyRedZone = NewbIsVisibiltyRedZone;
}

void ABaseGameState::FinishMoveAirplane()
{
	if (!HasAuthority())
	{
		return;
	}
	
	for (APlayerState* PlayerState : PlayerArray)
	{
		ABasePlayerState* PS = Cast<ABasePlayerState>(PlayerState);
		
		if (PS && PS->GetOwner())  
		{
			PS->GetAbilitySystemComponent()->TryActivateAbilityByClass(FallAbility, true );
		}
	}
}


int32 ABaseGameState::GetAlivePlayers()
{	
	int32 PlayerNum;
	
	if (AllPlayerCount != 0)
	{
		PlayerNum = AllPlayerCount;
	}
	else
	{
		PlayerNum = PlayerArray.Num();
	}

	for (APlayerState* PlayerState : PlayerArray)
	{
		ABasePlayerState* PS = Cast<ABasePlayerState>(PlayerState);
		
		if (PS && PS->HasDeadTag())  
		{
			PlayerNum--;
		}
	}
	
	return PlayerNum;
}

void ABaseGameState::FinishGame()
{
	bIsGameStarted = false;

	for (APlayerState* PlayerState : PlayerArray)
	{
		ABasePlayerState* PS = Cast<ABasePlayerState>(PlayerState);
		
		if (PS && !PS->HasDeadTag())  
		{
			PS->SetRank(CurrentPlayerCount);
		}
	}
}

void ABaseGameState::UpdatePlayerCount()
{
	if (HasAuthority())
	{		
		CurrentPlayerCount = GetAlivePlayers();
		OnRep_PlayerCount(); // 리슨 서버용
		
		if (CurrentPlayerCount == 1 && bIsPlaneBoarding)
		{
			FinishGame();			
		}
	}
}

int32 ABaseGameState::GetAllPlayerCount() const
{
	return AllPlayerCount;
}

int32 ABaseGameState::GetCurrentPlayerCount() const
{
	return CurrentPlayerCount;
}

bool ABaseGameState::GetIsGameStarted() const
{
	return bIsGameStarted;
}

FBox ABaseGameState::GetLandScapeBoundingBox() const
{
	return LandScapeBoundingBox;
}

void ABaseGameState::Initialize()
{
	// 기본 값 설정
	bIsGameStarted = false;
	bIsPlaneBoarding = false;
}
