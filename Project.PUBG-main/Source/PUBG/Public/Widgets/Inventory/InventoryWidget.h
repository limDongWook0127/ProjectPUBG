// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrandeSlotWIdget.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Widgets/WidgetBase.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */

class UArmorSlotWidget;
class UWeaponSlotWidget1;
class USizeBox;
class UWeaponSlotWidget;
class UEquippedComponent;
class UVerticalBox;
class UNearComponent;
class UItemSlotWidget;
class UWrapBox;

UCLASS()
class PUBG_API UInventoryWidget : public UWidgetBase
{
	GENERATED_BODY()


private:
	// Variable
	UPROPERTY(EditAnywhere)
	UInventoryComponent* InventoryComponent;
	UPROPERTY(EditAnywhere)
	UNearComponent* NearComponent;
	UPROPERTY(EditAnywhere)
	UEquippedComponent* EquippedComponent;
	

	UPROPERTY()
	EItemZoneType ItemZoneType;

	UClass* ItemSlotWidgetBPClass;
	UClass* WeaponSlotWidgetBPClass1;
	UClass* WeaponSlotWidgetBPClass2;
	UClass* WeaponSlotWidgetBPClass3;
	UClass* WeaponSlotWidgetBPClass11;
	UClass* WeaponSlotWidgetBPClass22;
	UClass* WeaponSlotWidgetBPClass33;
	UClass* GrenadeSlotWidgetBPClass;
	

	// Widget Variable
	UPROPERTY(meta = (BindWidget))
	UWrapBox* WrapBox_Inventory;
	UPROPERTY(meta = (BindWidget))
	UWrapBox* WrapBox_Near;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox_Near;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox_Inventory;
	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox_1Slot;
	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox_2Slot;
	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox_3Slot;
	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox_5Slot;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UArmorSlotWidget* WBP_ArmorSlot_Helmet;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UArmorSlotWidget* WBP_ArmorSlot_Bag;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UArmorSlotWidget* WBP_ArmorSlot_Armor;

	UPROPERTY(EditAnywhere)
	UItemSlotWidget* ItemSlotWidget;
	UPROPERTY(EditAnywhere)
	UItemSlotWidget* NearItemSlotWidget;
	UPROPERTY(EditAnywhere)
	UWeaponSlotWidget* Weapon1SlotWidget;
	UPROPERTY(EditAnywhere)
	UWeaponSlotWidget* Weapon2SlotWidget;
	UPROPERTY(EditAnywhere)
	UWeaponSlotWidget* Weapon3SlotWidget;
	UPROPERTY(EditAnywhere)
	UWeaponSlotWidget1* Weapon11SlotWidget;
	UPROPERTY(EditAnywhere)
	UWeaponSlotWidget1* Weapon22SlotWidget;
	UPROPERTY(EditAnywhere)
	UWeaponSlotWidget1* Weapon33SlotWidget;
	UPROPERTY(EditAnywhere)
	UGrandeSlotWIdget* GrenadeSlotWidget;
	
public:
	UInventoryWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateInventoryWidget();
	UFUNCTION(BlueprintCallable)
	void UpdateNearItemSlotWidget();
	UFUNCTION(BlueprintCallable)
	void UpdateEquippedWidget();
	
	//Setter
	void SetInventoryComponent(UInventoryComponent* OutInventoryComponent) { InventoryComponent = OutInventoryComponent; }
	void SetNearComponent(UNearComponent* OutNearComponent) { NearComponent = OutNearComponent; }
	void SetEquippedComponent(UEquippedComponent* OutEquippedComponent) { EquippedComponent = OutEquippedComponent; }

	//Getter
	UItemSlotWidget* GetItemSlotWidget() const { return ItemSlotWidget; }
	UWrapBox *GetWrapBox_Inventory() const { return WrapBox_Inventory; }
	UWrapBox *GetWrapBox_Near() const { return WrapBox_Near; }
	UVerticalBox* GetVerticalBox_Inventory() const { return VerticalBox_Inventory; }
	UVerticalBox* GetVerticalBox_Near() const { return VerticalBox_Near; }
	UFUNCTION(BlueprintCallable)
	USizeBox* GetSizeBox_1Slot() const { return SizeBox_1Slot; }

	virtual bool NativeOnDragOver( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;

	EItemZoneType CheckItemZoneType(FDragDropEvent InDragDropEvent);

	virtual bool NativeOnDrop(
			const FGeometry& InGeometry, 
			const FDragDropEvent& InDragDropEvent,
			UDragDropOperation* InOperation ) override;
	

	// 드래그 시 영역 확인
	EItemZoneType CheckItemZoneType(FPointerEvent InMousePoint);

	// 위젯들 파괴 및 초기화
	void EquippedUIInit();
	// 장착된 총 UI 업데이트
	UWeaponSlotWidget* EquippedWeaponUIUpdate(UWeaponSlotWidget* OutWeaponSlotWidget, AGun_Base* OutGunBase, int32 OutIndex);
	UWeaponSlotWidget1* EquippedWeaponUIUpdate(UWeaponSlotWidget1* OutWeaponSlotWidget, AGun_Base* OutGunBase, int32 OutIndex);
	UWeaponSlotWidget1* PartsUIUpdate(UDataTable* PartsDT, UWeaponSlotWidget1* OutWeaponSlotWidget, AGun_Base* OutGunBase);
};

