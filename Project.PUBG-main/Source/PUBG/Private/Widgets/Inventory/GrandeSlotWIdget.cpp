// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/GrandeSlotWIdget.h"

#include "Component/Inventory/InventoryComponent.h"

FReply UGrandeSlotWIdget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (EquippedComponent)
		{
			UDataTable* DataTable = EquippedComponent->GetItemDataTable();
			FItemStruct* Row = DataTable->FindRow<FItemStruct>(Name, TEXT("FAil"));

			int32 Weight = Row->Weight;
			EItemCategory Category = Row->Category;
			
			EquippedComponent->GetEquippedItems()[Index] = nullptr;
			
			InventoryComponent->ServerAddToInventory(Name, 1, Weight, Category);

			//EquippedComponent->OnRep_EquippedItems();
		}
	}
	
	return FReply::Handled();
}
