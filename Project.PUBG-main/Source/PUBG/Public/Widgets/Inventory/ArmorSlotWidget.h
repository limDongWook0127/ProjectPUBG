// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseLibrary/DataStruct/ItemStruct.h"
#include "Widgets/WidgetBase.h"
#include "ArmorSlotWidget.generated.h"

class UEquippedComponent;
struct FItemStruct;
class UButton;
class UImage;
/**
 * 
 */
UCLASS()
class PUBG_API UArmorSlotWidget : public UWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnPreviewMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;


private:
	// Widget Variable
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UButton* Button_Armor;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Normal;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Hovered;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Pressed;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Disabled;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Armor;

	// Variable
	UPROPERTY()
	FName ArmorName;
	UPROPERTY()
	int32 ArmorIndex;
	UPROPERTY()
	FItemStruct ArmorData;
	UPROPERTY()
	UEquippedComponent* EquippedComponent;
public:
	// Getter
	UFUNCTION(BlueprintCallable, Category = "UI")
	UButton* GetButtonArmor() const { return Button_Armor; }
	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImageNormal() const { return Image_Normal; }
	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImageHovered() const { return Image_Hovered; }
	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImagePressed() const { return Image_Pressed; }
	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImageDisabled() const { return Image_Disabled; }
	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImageArmor() const { return Image_Armor; }

	FName GetArmorName() const { return ArmorName; }
	int32 GetArmorIndex() const { return ArmorIndex; }

	// Setter
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetButtonArmor(UButton* NewButtonArmor) { Button_Armor = NewButtonArmor; }
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImageNormal(UImage* NewImageNormal) { Image_Normal = NewImageNormal; }
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImageHovered(UImage* NewImageHovered) { Image_Hovered = NewImageHovered; }
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImagePressed(UImage* NewImagePressed) { Image_Pressed = NewImagePressed; }
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImageDisabled(UImage* NewImageDisabled) { Image_Disabled = NewImageDisabled; }
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImageArmor(UImage* NewImageArmor) { Image_Armor = NewImageArmor; }
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetArmorData(FItemStruct NewitemStruct) { ArmorData = NewitemStruct; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetArmorName(FName Newname) { ArmorName = Newname; }
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetArmorIndex(int32 NewIndex) { ArmorIndex = NewIndex; }
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetEquippedComponent(UEquippedComponent* NewEquippedComponent) { EquippedComponent = NewEquippedComponent; }

	void InitArmorSlot();

	UFUNCTION(BlueprintCallable)
	void OnButton_ArmorSlotHovered();
	UFUNCTION(BlueprintCallable)
	void OnButton_ArmorSlotUnHovered();
};
