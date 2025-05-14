// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Lobby/SessionBrowserWidget.h"
#include "CreateGameWidget.generated.h"

class UCheckBox;
class UEditableText;
class UBaseButtonWidget;

/**
 * 
 */
UCLASS()
class PUBG_API UCreateGameWidget : public USessionBrowserWidget
{
	GENERATED_BODY()
	
public:	
	UCreateGameWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	
	UFUNCTION(BlueprintCallable)
	void OnCreateButton_Clicked();	
	UFUNCTION(BlueprintCallable)
	void OnText_SessionName_Changed(const FText& Text);
	UFUNCTION(BlueprintCallable)
	void OnText_MaxPlayer_Changed(const FText& Text);
	UFUNCTION(BlueprintCallable)
	void OnCheckState_Changed(bool IsChecked);
	
private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UBaseButtonWidget* Button_Create;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UCheckBox* CheckBox_EnableLan;	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UEditableText* Text_SessionName;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UEditableText* Text_MaxPlayer;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString SessionName;	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int MaxPlayer;	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bIsUseLan;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName OpenLevelName;
	
};
