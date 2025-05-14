// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/GameStatus/CurrentPlayerWidget.h"
#include "Components/TextBlock.h"
#include "GameState/BaseGameState.h"
#include "Kismet/GameplayStatics.h"

void UCurrentPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ABaseGameState* GS = Cast<ABaseGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		UpdateCurrentPlayer(GS->GetCurrentPlayerCount());
	}
}

void UCurrentPlayerWidget::UpdateCurrentPlayer(int32 CurrentPlayer)
{
	Text_CurrentPlayer->SetText(FText::FromString(FString::FromInt(CurrentPlayer)));
}

