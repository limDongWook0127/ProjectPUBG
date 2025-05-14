// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "WeaponPartsSlot.generated.h"

class UInventoryComponent;
class AGun_Base;
class UButton;
class UImage;
/**
 * 
 */
UCLASS()
class PUBG_API UWeaponPartsSlot : public UWidgetBase
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	//virtual FReply NativeOnPreviewMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent );


	
private:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UButton* Button_Parts;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Hovered;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Pressed;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Disabled;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Normal;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Parts;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Equipable;

	// Variable
	UPROPERTY()
	int32 PartsIndex;
	UPROPERTY()
	AGun_Base* Gun;
	UPROPERTY()
	UInventoryComponent* InventoryComponent;

public:
	// Getter Functions
	UFUNCTION(BlueprintCallable, Category = "UI")
	UButton* GetButtonParts() const { return Button_Parts; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImageHovered() const { return Image_Hovered; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImagePressed() const { return Image_Pressed; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImageDisabled() const { return Image_Disabled; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImageNormal() const { return Image_Normal; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImageParts() const { return Image_Parts; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetPartsIndex() const { return PartsIndex; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	AGun_Base* GetGun() const { return Gun; }
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	
	// Setter Functions
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetButtonParts(UButton* NewButton) { Button_Parts = NewButton; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImageHovered(UImage* NewImage) { Image_Hovered = NewImage; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImagePressed(UImage* NewImage) { Image_Pressed = NewImage; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImageDisabled(UImage* NewImage) { Image_Disabled = NewImage; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImageNormal(UImage* NewImage) { Image_Normal = NewImage; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImageParts(UImage* NewImage) { Image_Parts = NewImage; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetPartsIndex(int32 NewIndex) { PartsIndex = NewIndex; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetGun(AGun_Base* NewGun) { Gun = NewGun;}
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetInventoryComponent(UInventoryComponent* NewInventory) { InventoryComponent = NewInventory; }
};
