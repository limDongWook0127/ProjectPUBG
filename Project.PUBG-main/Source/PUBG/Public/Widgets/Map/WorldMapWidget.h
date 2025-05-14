// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "WorldMapWidget.generated.h"

class UMapWidget;
class UBackgroundBlur;
class UScaleBox;
class UGridPanel;

/**
 * 
 */
UCLASS()
class PUBG_API UWorldMapWidget : public UWidgetBase
{
	GENERATED_BODY()

public:	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void SwitchWidget();
	
	UMapWidget* GetWBP_Map();
	FVector2D GetPanelMinimapSize();

	UFUNCTION()
	UMapWidget* GetMapWidget() const { return WBP_Map; }	
	
private:
	bool bIsMiniMapState = true;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UMapWidget* WBP_Map;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UBackgroundBlur* BackgroundBlur;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UScaleBox* ScaleBox_WorldMap;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UGridPanel* Panel_Minimap;
};
