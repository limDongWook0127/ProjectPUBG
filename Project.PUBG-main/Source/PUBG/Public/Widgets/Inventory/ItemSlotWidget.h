// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "Component/NearArea/NearComponent.h"
#include "ItemSlotWidget.generated.h"

enum class EItemCategory : uint8;
class UEquippedComponent;
enum class EItemZoneType : uint8;
class UVerticalBox;
class UInventoryWidget;
class UInventoryComponent;
class UButton;
class UImage;
class UTextBlock;
class UDataTable;
class UWrapBox;
class UDragItemWidget;
/**
 * 
 */


UCLASS()
class PUBG_API UItemSlotWidget : public UWidgetBase
{
	GENERATED_BODY()

	// Widget Variable
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UButton* Button_ItemSlot;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UImage* Image_Icon;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_ItemName;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_Quantity;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* TextBlock_Use;

	// Variable
	UPROPERTY()
	FName ItemName;
	UPROPERTY()
	int32 Quantity;
	UPROPERTY()
	EItemCategory ItemCategory;
	UPROPERTY()
	UInventoryComponent* InventoryComponent;
	UPROPERTY()
	UNearComponent* NearComponent;
	UPROPERTY()
	UEquippedComponent* EquippedComponent;
	UPROPERTY()
	int32 Index;

	// 드래그할 때 필요한 위젯들 
	UPROPERTY()
	UDragItemWidget* DragItemWidget;
	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	// 드래그 시작 영역 
	UPROPERTY()
	UVerticalBox* VerticalBox_Near;
	UPROPERTY()
	UVerticalBox* VerticalBox_Inventory;
	UPROPERTY()
	FPointerEvent StartMousePoint; // 드래그 시작 지점, 비어있는지 확인해야 함 (어떻게?)

	UPROPERTY()
	UDataTable* DataTable;
	// UPROPERTY()
	// EItemZoneType ItemZoneType;
	
	// UPROPERTY()
	// UDataTable* DataTable;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	//Setter
	void SetItemName(FName OutItemName) { ItemName = OutItemName; }
	void SetQuantity(int32 OutQuantity) { Quantity = OutQuantity; }
	void SetInventoryComponent(UInventoryComponent* OutInventoryComponent) { InventoryComponent = OutInventoryComponent; }
	void SetNearComponent(UNearComponent* OutNearComponent) { NearComponent = OutNearComponent; }
	void SetIndex(int32 OutIndex) { Index = OutIndex; }
	void SetInventoryWidget(UInventoryWidget* OutInventoryWidget) { InventoryWidget = OutInventoryWidget; }
	void SetEquippedComponent(UEquippedComponent* OutEquippedComponent) { EquippedComponent = OutEquippedComponent; }
	void SetItemCategory(EItemCategory NewItemCategory) { ItemCategory = NewItemCategory; }
	
	// Getter
	UTextBlock* GetTextBlock_Use() const { return TextBlock_Use; }
	UTextBlock* GetText_ItemName() const { return Text_ItemName; }
	UFUNCTION(BlueprintCallable)
	void UpdateItemSlotWidget();
	UFUNCTION(BlueprintCallable)
	UEquippedComponent* GetEquippedComponent() const { return EquippedComponent; }
	UFUNCTION(BlueprintCallable)
	EItemCategory GetItemCategory() const { return ItemCategory; }
	

	
	UFUNCTION(BlueprintCallable)
	void OnButton_ItemSlotHovered();
	UFUNCTION(BlueprintCallable)
	void OnButton_ItemSlotUnHovered();
	
	virtual FReply NativeOnPreviewMouseButtonDown(
						const FGeometry& InGeometry,
						const FPointerEvent& InMouseEvent
						) override;

	
	virtual void NativeOnDragDetected(
					const FGeometry& InGeometry,
					const FPointerEvent& InMouseEvent,
					UDragDropOperation*& OutOperation
					) override;
	
	// virtual bool NativeOnDragOver( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;
	// virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;
	//
	// // 드래그 시 영역 확인
	// EItemZoneType CheckItemZoneType(FPointerEvent InMousePoint);
	// EItemZoneType CheckItemZoneType(FDragDropEvent InDragDropEvent);
};


