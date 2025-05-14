// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "CompassWidget.generated.h"

class APlayerCharacter;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class PUBG_API UCompassWidget : public UWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void Calculate(float Value);
	void SetDirectionValue(float Value);
	void SetDirectionText(int32 Value);
private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_Compass;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_Background;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* TextBlock_Direction;

	UPROPERTY()
	UMaterialInstanceDynamic * Directional;
	UPROPERTY()
	APlayerController* PlayerController;
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
};
