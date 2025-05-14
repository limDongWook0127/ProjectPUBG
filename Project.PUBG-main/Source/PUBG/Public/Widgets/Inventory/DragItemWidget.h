// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WidgetBase.h"
#include "DragItemWidget.generated.h"

class UInventoryWidget;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PUBG_API UDragItemWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

	
private:
	// Widget Variable
	UPROPERTY(meta = (BindWidget))
	UImage* Image_ItemImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock_ItemQuantity;

	// Variable
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName ItemName;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 Quantity;
	UPROPERTY()
	UInventoryWidget* InventoryWidget;
public:
	// Setter
	UFUNCTION(BlueprintCallable)
	void SetQuantity(int32 OutQuantity) { Quantity = OutQuantity; }
	UFUNCTION(BlueprintCallable)
	void SetItemName(FName OutItemName) { ItemName = OutItemName; }
	UFUNCTION()
	void SetInventoryWidget(UInventoryWidget* OutInventoryWidget) { InventoryWidget = OutInventoryWidget; }
	
	UFUNCTION(BlueprintCallable)
	void UpdateDragItemWidget();
};
