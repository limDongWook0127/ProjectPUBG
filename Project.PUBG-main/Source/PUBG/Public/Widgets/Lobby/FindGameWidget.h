// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Widgets/Lobby/SessionBrowserWidget.h"
#include "FindGameWidget.generated.h"

class UBaseButtonWidget;
class UCheckBox;
class UScrollBox;
class UServerBrowserItemWidget;

/**
 * 
 */
UCLASS()
class PUBG_API UFindGameWidget : public USessionBrowserWidget
{
	GENERATED_BODY()
	
public:		
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	
	UFUNCTION(BlueprintCallable)
	void OnRefreshButton_Clicked();	
	UFUNCTION(BlueprintCallable)
	void OnCheckState_Changed(bool IsChecked);
	UFUNCTION(BlueprintCallable)
	void OnFindSessionsComplete(bool bWasSuccessful);
	
private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UBaseButtonWidget* Button_Refresh;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UCheckBox* CheckBox_EnableLan;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UScrollBox* ScrollBox_Game;	
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int MaxPlayer;	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bIsUseLAN;		
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<UServerBrowserItemWidget> ServerBrowserItemWidgetClass;		
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
