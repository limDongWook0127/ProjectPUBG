// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Notification/NotificationWidget.h"
#include "Components/TextBlock.h"


void UNotificationWidget::PlayNotifyAnimation()
{
	if (NotifyAnimation)
	{
		StopAnimation(NotifyAnimation);  // 애니메이션 중지
		PlayAnimation(NotifyAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	}
}

void UNotificationWidget::ShowNotification(FText Message)
{
	Text_Notification->SetText(Message);
	PlayNotifyAnimation();
}

void UNotificationWidget::PlayKillLogAnimation()
{
	if (KillLogAnimation)
	{
		StopAnimation(KillLogAnimation);  // 애니메이션 중지
		PlayAnimation(KillLogAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	}
}

void UNotificationWidget::ShowKillLog(int32 KillCount)
{
	FString Text = FString::FromInt(KillCount);
	FText Num = FText::FromString(Text);
	Text_KillLog->SetText(Num);
	PlayKillLogAnimation();
}

void UNotificationWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
