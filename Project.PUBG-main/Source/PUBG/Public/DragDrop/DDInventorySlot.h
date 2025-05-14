// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DDInventorySlot.generated.h"

class UNearComponent;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class PUBG_API UDDInventorySlot : public UDragDropOperation
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UInventoryComponent* InventoryComponent;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UNearComponent* NearComponent;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 ContentIndex;


public:
	// Setter
	UFUNCTION(BlueprintCallable)
	void SetInventoryComponent(UInventoryComponent* OutInventoryComponent) { InventoryComponent = OutInventoryComponent; }
	UFUNCTION(BlueprintCallable)
	void SetNearComponent(UNearComponent* OutNearComponent) { NearComponent = OutNearComponent; }
	UFUNCTION(BlueprintCallable)
	void SetContentIndex(int32 InContentIndex) { ContentIndex = InContentIndex; }

	//Getter
	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetInventoryComponent() { return InventoryComponent; }
	UFUNCTION(BlueprintCallable)
	UNearComponent* GetNearComponent() { return NearComponent; }
	UFUNCTION(BlueprintCallable)
	int32 GetContentIndex() { return ContentIndex; }
	
	UFUNCTION(BlueprintCallable)
	void UpdateDDInventorySlot();
	UFUNCTION(BlueprintCallable)
	int32 GetHaveComponent();
};
