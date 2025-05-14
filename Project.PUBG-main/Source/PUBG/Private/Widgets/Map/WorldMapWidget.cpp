// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Map/WorldMapWidget.h"

#include "RewindData.h"
#include "Components/BackgroundBlur.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridPanel.h"
#include "Components/ScaleBox.h"
#include "Widgets/Map/MapWidget.h"


UMapWidget* UWorldMapWidget::GetWBP_Map()
{
	return WBP_Map;
}

void UWorldMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsMiniMapState)
	{
		FVector2D PlayerLocation = WBP_Map->GetPlayerLocation();
		FVector2D WorldMapImageSize = WBP_Map->GetWorldMapImageSize();
		FVector2D Panel_MinimapSize = GetPanelMinimapSize();
		
		FVector2D ClippingLocation;
		
		ClippingLocation.X = FMath::Clamp(-PlayerLocation.X + (Panel_MinimapSize.X * 0.5f), -WorldMapImageSize.X + Panel_MinimapSize.X, 0.f);
		ClippingLocation.Y = FMath::Clamp(-PlayerLocation.Y + (Panel_MinimapSize.Y * 0.5f), -WorldMapImageSize.Y + Panel_MinimapSize.Y, 0.f);
				
		WBP_Map->SetRenderTranslation(ClippingLocation);
	}
	else
	{
		WBP_Map->SetRenderTranslation(FVector2D::ZeroVector);
	}
}

FVector2D UWorldMapWidget::GetPanelMinimapSize()
{
	if (Panel_Minimap)
	{
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Panel_Minimap->Slot);
		if (CanvasSlot)
		{
			return CanvasSlot->GetSize();
		}
	}
	return FVector2D(0, 0); // 기본값
}

void UWorldMapWidget::SwitchWidget()
{
	if (bIsMiniMapState)
	{
		WBP_Map->RemoveFromParent();
		ScaleBox_WorldMap->AddChild(WBP_Map);
		
		BackgroundBlur->SetVisibility(ESlateVisibility::Visible);
		Panel_Minimap->SetVisibility(ESlateVisibility::Hidden);
		
		bIsMiniMapState = false;
	}
	else
	{
		WBP_Map->RemoveFromParent();
		Panel_Minimap->AddChildToGrid(WBP_Map);
		
		BackgroundBlur->SetVisibility(ESlateVisibility::Hidden);
		Panel_Minimap->SetVisibility(ESlateVisibility::Visible);
		
		bIsMiniMapState = true;
	}	
}
