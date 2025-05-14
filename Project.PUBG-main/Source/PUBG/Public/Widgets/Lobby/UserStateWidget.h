// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "UserStateWidget.generated.h"

class ALobbyPlayerState;
class UGridPanel;
class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class PUBG_API UUserStateWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:		
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void SetReadyState(bool IsReady);	
	UFUNCTION(BlueprintCallable)
	void InitializeUserState(ALobbyPlayerState* LobbyPlayerState);	
	
private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UGridPanel* Panel_ReadyState;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_PlayerName;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_Host;	
};
