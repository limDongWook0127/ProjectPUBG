// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "KillCountWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PUBG_API UKillCountWidget : public UWidgetBase
{
	GENERATED_BODY()
		
public:
	/** 게임스테이트에서 플레이어 수를 가져오는 함수 */
	UFUNCTION()
	void UpdateKillCount(int32 KillCount);

protected:
	/** 바인딩할 텍스트 블록 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_KillCount;	
};
