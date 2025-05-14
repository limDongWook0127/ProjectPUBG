// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "CurrentPlayerWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PUBG_API UCurrentPlayerWidget : public UWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	/** 게임스테이트에서 플레이어 수를 가져오는 함수 */
	UFUNCTION()
	void UpdateCurrentPlayer(int32 CurrentPlayer);

protected:
	/** 바인딩할 텍스트 블록 */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_CurrentPlayer;
};
