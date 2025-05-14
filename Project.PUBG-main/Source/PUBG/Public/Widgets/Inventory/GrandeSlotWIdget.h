// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Equipped/EquippedComponent.h"
#include "Widgets/WidgetBase.h"
#include "GrandeSlotWIdget.generated.h"

class UInventoryComponent;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PUBG_API UGrandeSlotWIdget : public UWidgetBase
{
	GENERATED_BODY()

public:
	virtual FReply NativeOnPreviewMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

private:
	// WidgetVariable
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_SlotNumber;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UTextBlock* Text_GrenadeName;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UImage* Image_Grenade;

	UPROPERTY() FName Name;
	UPROPERTY() int32 Index;
	UPROPERTY() UInventoryComponent* InventoryComponent;
	UPROPERTY() UEquippedComponent* EquippedComponent;

public:
	// Getter
	UFUNCTION(BlueprintCallable, Category = "UI")
	UTextBlock* GetTextSlotNumber() const { return Text_SlotNumber; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	UTextBlock* GetTextGrenadeName() const { return Text_GrenadeName; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	UImage* GetImageGrenade() const { return Image_Grenade; }

	// Setter
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetTextSlotNumber(UTextBlock* NewText) { Text_SlotNumber = NewText; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetTextGrenadeName(UTextBlock* NewText) { Text_GrenadeName = NewText; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetImageGrenade(UImage* NewImage){ Image_Grenade = NewImage; }

	UFUNCTION(BlueprintCallable)
	void SetName(FName InName) { Name = InName; } 
	UFUNCTION(BlueprintCallable)
	void SetIndex(int32 InIndex) { Index = InIndex; } 
	UFUNCTION(BlueprintCallable)
	void SetInventoryComponent(UInventoryComponent* InInventoryComponent) { InventoryComponent = InInventoryComponent; } 
	UFUNCTION(BlueprintCallable)
	void SetEquippedComponent(UEquippedComponent* InEquippedComponent) { EquippedComponent = InEquippedComponent; }

};
