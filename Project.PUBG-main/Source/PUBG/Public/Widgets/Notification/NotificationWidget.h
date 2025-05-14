// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "NotificationWidget.generated.h"

class UWidgetAnimation;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PUBG_API UNotificationWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	// 애니메이션을 찾아서 재생하는 함수 선언
	UFUNCTION(BlueprintCallable)
	void PlayNotifyAnimation();
	UFUNCTION(BlueprintCallable)
	void ShowNotification(FText Message);
	UFUNCTION(BlueprintCallable)
	void PlayKillLogAnimation();
	UFUNCTION(BlueprintCallable)
	void ShowKillLog(int32 KillCount);

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* NotifyAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* KillLogAnimation;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Notification;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_KillLog;

	
};
