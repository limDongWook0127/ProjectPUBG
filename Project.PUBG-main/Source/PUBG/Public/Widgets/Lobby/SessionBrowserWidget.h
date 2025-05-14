// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "SessionBrowserWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackButton_Clicked);

class UBaseButtonWidget;
/**
 * 
 */
UCLASS()
class PUBG_API USessionBrowserWidget : public UWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void OnBackButton_Clicked();
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnBackButton_Clicked OnBackButtonClicked;
	
private:	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UBaseButtonWidget* Button_Back;	
};
