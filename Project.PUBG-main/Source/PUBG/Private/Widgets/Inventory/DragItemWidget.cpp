// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DragItemWidget.h"

#include "BaseLibrary/DataStruct/ItemStruct.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UDragItemWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	FString DataTablePath = TEXT("/Game/Datatables/ItemTable.ItemTable");
	UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));

	if (DataTable)
	{
		if (ItemName != NAME_None)
		{
			FItemStruct* Row = DataTable->FindRow<FItemStruct>(ItemName, TEXT("Get Item Row"));

			Image_ItemImage->SetBrushFromTexture(Row->Image);
			TextBlock_ItemQuantity->SetText(FText::AsNumber(Quantity));
			//Text_ItemName->SetText(FText::FromName(ItemName));
		}
		
	}
}

void UDragItemWidget::UpdateDragItemWidget()
{
	FString DataTablePath = TEXT("/Game/Datatables/ItemTable.ItemTable");
	UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));

	if (DataTable)
	{
		if (ItemName != NAME_None)
		{
			FItemStruct* Row = DataTable->FindRow<FItemStruct>(ItemName, TEXT("Get Item Row"));

			Image_ItemImage->SetBrushFromTexture(Row->Image);
			TextBlock_ItemQuantity->SetText(FText::AsNumber(Quantity));
			//Text_ItemName->SetText(FText::FromName(ItemName));
		}
	}
}
