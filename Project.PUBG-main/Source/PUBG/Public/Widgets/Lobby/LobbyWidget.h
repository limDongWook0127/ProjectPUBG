// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "LobbyWidget.generated.h"

class UBaseButtonWidget;
class UWidgetSwitcher;
class UCreateGameWidget;
class UFindGameWidget;
/**
 * 
 */
UCLASS()
class PUBG_API ULobbyWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:		
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void OnCreateGameButton_Clicked();	
	UFUNCTION(BlueprintCallable)
	void OnFindGameButton_Clicked();	
	UFUNCTION(BlueprintCallable)
	void OnBackButton_Clicked();	

private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UBaseButtonWidget* Button_CreateGame;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UBaseButtonWidget* Button_FindGame;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UWidgetSwitcher* WidgetSwitcher_Session;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UCreateGameWidget* CreateGameWidget;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UFindGameWidget* FindGameWidget;	
};
