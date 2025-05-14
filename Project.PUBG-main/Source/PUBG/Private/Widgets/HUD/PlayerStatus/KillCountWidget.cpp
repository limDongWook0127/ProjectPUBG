// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/PlayerStatus/KillCountWidget.h"
#include "Components/TextBlock.h"

void UKillCountWidget::UpdateKillCount(int32 KillCount)
{	
	Text_KillCount->SetText(FText::FromString(FString::FromInt(KillCount)));
}

