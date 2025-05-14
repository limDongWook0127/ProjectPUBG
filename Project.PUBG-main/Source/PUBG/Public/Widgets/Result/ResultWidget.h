// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "ResultWidget.generated.h"

class UTextBlock;
class UBaseButtonWidget;
/**
 * 
 */
UCLASS()
class PUBG_API UResultWidget : public UWidgetBase
{
	GENERATED_BODY()
	
	
public:	
	UResultWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetText_ResultMessage(FString Text);
	UFUNCTION(BlueprintCallable)
	void SetText_Rank(int32 Rank);
	UFUNCTION(BlueprintCallable)
	void SetText_AllPlayerCount(int32 AllPlayerCount);
	UFUNCTION(BlueprintCallable)
	void SetText_KillCount(int32 KillCount);
	UFUNCTION(BlueprintCallable)
	void OnResultButton_Clicked();	
	
private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_ResultMessage;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_Rank_1;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_AllPlayerCount;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_Rank_2;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_KillCount;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UBaseButtonWidget* WBP_ResultButton;
	
};
