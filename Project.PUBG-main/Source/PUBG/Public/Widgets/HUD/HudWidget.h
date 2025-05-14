// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "HudWidget.generated.h"

class UKillCountWidget;
class UDisplayMessageItemWidget;
class UCurrentPlayerWidget;
class UPlayerStatusWidget;
class UWorldMapWidget;
/**
 * 
 */
UCLASS()
class PUBG_API UHudWidget : public UWidgetBase
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized();

private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UPlayerStatusWidget* WBP_PlayerStatus;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UCurrentPlayerWidget* WBP_CurrentPlayer;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UKillCountWidget* WBP_KillCount;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UDisplayMessageItemWidget* WBP_DisplayMessageItem;
    UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UWorldMapWidget* WBP_WorldMap;

public:
	UFUNCTION(BlueprintCallable)
	UPlayerStatusWidget* GetPlayerStatusWidget() const { return WBP_PlayerStatus; }
	UFUNCTION()
	UCurrentPlayerWidget* GetCurrentPlayerWidget() const { return WBP_CurrentPlayer; }
	UFUNCTION()
	UKillCountWidget* GetKillCountWidget() const { return WBP_KillCount; }
	UFUNCTION()
	UDisplayMessageItemWidget* GetDisplayMessageItemWidget() const { return WBP_DisplayMessageItem; }
	UFUNCTION()
	UWorldMapWidget* GetWorldMapWidget() const { return WBP_WorldMap; }
};
