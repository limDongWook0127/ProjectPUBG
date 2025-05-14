// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "MapWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class PUBG_API UMapWidget : public UWidgetBase
{
	GENERATED_BODY()

public:
	UMapWidget();
	void InitializeImage();

	virtual void NativeConstruct() override;	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	FVector2D ConvertWorldToMap(const FVector& WorldLocation) const;
	float ConvertScale(float Scale);
	FVector2D ConvertPosition(const FVector& WorldLocation) const;

	void UpdatePlayerLocation();
	void UpdateCurrentZone();
	void UpdateNextZone();
	void UpdateAirplanebVisibilty();
	void UpdateAirplanebLocation();
	void UpdateRedZone();
	void SetLandScapeBoundingBox();

	FVector2D GetPlayerLocation() const;
	FVector2D GetWorldMapImageSize() const;

	bool GetIsInPlane() const;
	void SetIsInPlane(bool NewInIsInPlane);

private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_WorldMap;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_Player;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_CurrentZone;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_NextZone;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_Airplane;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_AirplaneLine;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_RedZone;
	UPROPERTY()
	UMaterialInstanceDynamic* CurrentZoneMaterial;
	UPROPERTY()
	UMaterialInstanceDynamic* NextZoneMaterial;
	UPROPERTY()
	UMaterialInstanceDynamic* AirplaneLineMaterial;
	UPROPERTY()
	UMaterialInstanceDynamic* RedZoneMaterial;
	
	FBox LandScapeBoundingBox;
	bool bIsInPlane;
	FVector2D PlayerLocation;
};
