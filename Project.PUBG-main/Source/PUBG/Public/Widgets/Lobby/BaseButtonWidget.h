// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "BaseButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButton_Clicked);

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PUBG_API UBaseButtonWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	UBaseButtonWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void OnButton_Clicked();
	UFUNCTION(BlueprintCallable)
	void SetText(FString Text);
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnButton_Clicked OnButtonClicked;

private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UButton* Button_Base;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_Base;	
	
};
