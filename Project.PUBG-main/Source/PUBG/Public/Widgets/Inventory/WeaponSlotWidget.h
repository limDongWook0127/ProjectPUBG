// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "WeaponSlotWidget.generated.h"

class AGun_Base;
enum class EBulletType : uint8;
class USizeBox;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class PUBG_API UWeaponSlotWidget : public UWidgetBase
{
	GENERATED_BODY()

public:
	virtual FReply NativeOnPreviewMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;


private:
	// Widget Variable
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_RenderTargetWeapon; // 무기 이미지
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UTextBlock* Text_SlotNumber; // 슬롯 숫자
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UTextBlock* Text_WeaponName; // 무기 이름
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UTextBlock* Text_CurrentAmmo; // 장전된 총알
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UTextBlock* Text_TotalAmmo; // 남은 총알
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_AmmoImage; // 총알 이미지
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UTextBlock* Text_AmmoName; // 총알 이름
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USizeBox* SizeBox_Grip;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USizeBox* SizeBox_Barrel;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USizeBox* SizeBox_Magazine;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USizeBox* SizeBox_Sight;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USizeBox* SizeBox_CantedSight;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USizeBox* SizeBox_Stock;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Grip;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Barrel;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Magazine;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Sight;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_CantedSight;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Stock;

	

	

public:
	// Getter
	UFUNCTION(BlueprintCallable, Category = "Widget")
    UImage* GetImageRenderTargetWeapon() const { return Image_RenderTargetWeapon; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    UTextBlock* GetTextSlotNumber() const { return Text_SlotNumber; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    UTextBlock* GetTextWeaponName() const { return Text_WeaponName; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    UTextBlock* GetTextCurrentAmmo() const { return Text_CurrentAmmo; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    UTextBlock* GetTextTotalAmmo() const { return Text_TotalAmmo; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    UImage* GetImageAmmoImage() const { return Image_AmmoImage; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    UTextBlock* GetTextAmmoName() const { return Text_AmmoName; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    USizeBox* GetSizeBoxGrip() const { return SizeBox_Grip; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    USizeBox* GetSizeBoxBarrel() const { return SizeBox_Barrel; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    USizeBox* GetSizeBoxMagazine() const { return SizeBox_Magazine; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    USizeBox* GetSizeBoxSight() const { return SizeBox_Sight; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    USizeBox* GetSizeBoxCantedSight() const { return SizeBox_CantedSight; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    USizeBox* GetSizeBoxStock() const { return SizeBox_Stock; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    UImage* GetImageGrip() const { return Image_Grip; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    UImage* GetImageBarrel() const { return Image_Barrel; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    UImage* GetImageMagazine() const { return Image_Magazine; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    UImage* GetImageSight() const { return Image_Sight; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    UImage* GetImageCantedSight() const { return Image_CantedSight; }
    UFUNCTION(BlueprintCallable, Category = "Widget")
    UImage* GetImageStock() const { return Image_Stock; }
	
	//Setter
	void SetImage_RenderTargetWeapon(UImage* OutImage) { Image_RenderTargetWeapon = OutImage; }

	UFUNCTION(BlueprintCallable)
	FString SetBulletTypeTextBlock(EBulletType OutEBulletType);
	UFUNCTION(BlueprintCallable)
	void FindEquiablePartsSlot(AGun_Base* OutGun);

	
};
