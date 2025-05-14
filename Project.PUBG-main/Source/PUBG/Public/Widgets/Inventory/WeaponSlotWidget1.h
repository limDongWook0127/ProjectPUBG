// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "WeaponSlotWidget1.generated.h"

class UEquippedComponent;
class UWeaponPartsSlot;
enum class EBulletType : uint8;
class AGun_Base;
class UImage;
class UTextBlock;
class USizeBox;
/**
 * 
 */
UCLASS()
class PUBG_API UWeaponSlotWidget1 : public UWidgetBase
{
	GENERATED_BODY()

public:
	virtual FReply NativeOnPreviewMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	
	virtual void NativeOnInitialized() override;

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
	UWeaponPartsSlot* WBP_WeaponPartsSlot_Grip;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UWeaponPartsSlot* WBP_WeaponPartsSlot_Barrel;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UWeaponPartsSlot* WBP_WeaponPartsSlot_Magazine;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UWeaponPartsSlot* WBP_WeaponPartsSlot_Sight;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UWeaponPartsSlot* WBP_WeaponPartsSlot_Stock;

	// Variable
	UPROPERTY()
	FName WeaponName;
	UPROPERTY()
	int32 WeaponIndex;
	UPROPERTY()
	UEquippedComponent* EquippedComponent;
	

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

	UFUNCTION(BlueprintCallable, Category = "WeaponParts")
	UWeaponPartsSlot* GetWeaponPartsSlotGrip() const { return WBP_WeaponPartsSlot_Grip; }
	UFUNCTION(BlueprintCallable, Category = "WeaponParts")
	UWeaponPartsSlot* GetWeaponPartsSlotBarrel() const { return WBP_WeaponPartsSlot_Barrel; }
	UFUNCTION(BlueprintCallable, Category = "WeaponParts")
	UWeaponPartsSlot* GetWeaponPartsSlotMagazine() const { return WBP_WeaponPartsSlot_Magazine; }
	UFUNCTION(BlueprintCallable, Category = "WeaponParts")
	UWeaponPartsSlot* GetWeaponPartsSlotSight() const { return WBP_WeaponPartsSlot_Sight; }
	UFUNCTION(BlueprintCallable, Category = "WeaponParts")
	UWeaponPartsSlot* GetWeaponPartsSlotStock() const { return WBP_WeaponPartsSlot_Stock; }

	UFUNCTION(BlueprintCallable, Category = "Widget")
	FName GetWeaponName() const { return WeaponName; }
	UFUNCTION(BlueprintCallable, Category = "Widget")
	int32 GetWeaponIndex() const { return WeaponIndex;}
	
	
	//Setter
	void SetImage_RenderTargetWeapon(UImage* OutImage) { Image_RenderTargetWeapon = OutImage; }
	void SetTextureParameter(UTextureRenderTarget2D* OutImage) const; 

	UFUNCTION(BlueprintCallable)
	FString SetBulletTypeTextBlock(EBulletType OutEBulletType);
	UFUNCTION(BlueprintCallable)
	void FindEquiablePartsSlot(AGun_Base* OutGun);

	UFUNCTION(BlueprintCallable)
	void SetWeaponName(FName NewName) { WeaponName = NewName; }
	UFUNCTION(BlueprintCallable)
	void SetWeaponIndex(int32 NewIndex) { WeaponIndex = NewIndex; }
	UFUNCTION(BlueprintCallable)
	void SetEquipmentComponent(UEquippedComponent* NewEquippedComponent) { EquippedComponent = NewEquippedComponent; }

};
