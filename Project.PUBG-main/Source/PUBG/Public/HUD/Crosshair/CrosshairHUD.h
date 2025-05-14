// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CrosshairHUD.generated.h"


USTRUCT()
struct FHUDTexture
{
	GENERATED_BODY()
public:
	class UTexture2D* Player_Crosshair_Center;
	
	class UTexture2D* Crosshair_Center;
	class UTexture2D* Crosshair_Left;
	class UTexture2D* Crosshair_Right;
	class UTexture2D* Crosshair_Top;
	class UTexture2D* Crosshair_Bottom;
};
/**
 * 
 */
UCLASS()
class PUBG_API ACrosshairHUD : public AHUD
{
	GENERATED_BODY()
	

	
protected:
	virtual void DrawHUD() override;


private:
	FHUDTexture HUDTexture;

public:
	FORCEINLINE void SetHudTexture(const FHUDTexture& HUD_Tex) { HUDTexture = HUD_Tex; }
};
