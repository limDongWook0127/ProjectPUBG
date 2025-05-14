// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/ItemSlotWidget.h"

#include "BaseLibrary/DataStruct/ConsumeEffectStruct.h"
#include "BaseLibrary/DataStruct/ItemStruct.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Component/Equipped/EquippedComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "DragDrop/DDInventorySlot.h"
#include "Item/ArmorItem.h"
#include "Item/WeaponItem.h"
#include "Widgets/Inventory/DragItemWidget.h"
#include "Widgets/Inventory/InventoryWidget.h"

void UItemSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

void UItemSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
}

void UItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_Use->SetVisibility(ESlateVisibility::Collapsed);

	Button_ItemSlot->OnHovered.AddDynamic(this, &UItemSlotWidget::OnButton_ItemSlotHovered);
	Button_ItemSlot->OnUnhovered.AddDynamic(this, &UItemSlotWidget::OnButton_ItemSlotUnHovered);

	VerticalBox_Near = InventoryWidget->GetVerticalBox_Near();
	VerticalBox_Inventory = InventoryWidget->GetVerticalBox_Inventory();
	
	//ItemZoneType = EItemZoneType::None;
	

}

void UItemSlotWidget::UpdateItemSlotWidget()
{
	//UE_LOG(LogTemp, Warning, TEXT("UItemSlotWidget::UpdateItemSlotWidget"));

	FString DataTablePath = TEXT("/Game/Datatables/ItemTable.ItemTable");
	DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));

	if (DataTable)
	{
		if (ItemName != NAME_None)
		{
			FItemStruct* Row = DataTable->FindRow<FItemStruct>(ItemName, TEXT("Get Item Row"));

			Image_Icon->SetBrushFromTexture(Row->Image);
			if (Row->IsStackAble == true)
			{
				Text_Quantity->SetText(FText::AsNumber(Quantity));
			}
			Text_ItemName->SetText(FText::FromName(ItemName));
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("UItemSlotWidget::UpdateItemSlotWidget : Item name not found"));
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("UItemSlotWidget::UpdateItemSlotWidget : DataTable None"));
	}
}

void UItemSlotWidget::OnButton_ItemSlotHovered()
{
	if (InventoryComponent)
	{
		if (DataTable)
		{
			if (ItemName != NAME_None)
			{
				FItemStruct* Row = DataTable->FindRow<FItemStruct>(ItemName, TEXT("Get Item Row"));
				int32 Category = static_cast<int32>(Row->Category);

				if (Category == 4 || Category == 12)
				{
					FString Text = TEXT("장착");
					TextBlock_Use->SetText(FText::FromString(Text));
				}
				else if (Category == 10 || Category == 11)
				{
					FString Text = TEXT("사용");
					TextBlock_Use->SetText(FText::FromString(Text));
				}
				else
				{
					FString Text = TEXT("");
					TextBlock_Use->SetText(FText::FromString(Text));
				}
			}
		}
	}
	else if (NearComponent)
	{
		if (DataTable)
		{
			if (ItemName != NAME_None)
			{
				FItemStruct* Row = DataTable->FindRow<FItemStruct>(ItemName, TEXT("Get Item Row"));
				int32 Category = static_cast<int32>(Row->Category);

				if (Category == 0 || Category == 2 || Category == 3 || Category == 4 || Category == 6 || Category == 7 || Category == 8)
				{
					FString Text = TEXT("장착");
					TextBlock_Use->SetText(FText::FromString(Text));
				}
				else
				{
					FString Text = TEXT("줍기");
					TextBlock_Use->SetText(FText::FromString(Text));
				}
			}
		}
	}

	TextBlock_Use->SetVisibility(ESlateVisibility::Visible);

}

void UItemSlotWidget::OnButton_ItemSlotUnHovered()
{
	TextBlock_Use->SetVisibility(ESlateVisibility::Collapsed);
}

FReply UItemSlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	
	if (ItemName == NAME_None)
	{
		return FReply::Unhandled();
	}
	else
	{
		if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) // 좌클릭일 경우
		{
			FEventReply ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
			StartMousePoint = InMouseEvent;
			return ReplyResult.NativeReply;
		}
		else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) // 우클릭일 경우
		{
			//UE_LOG(LogTemp, Warning, TEXT("NativeOnPreviewMouseButtonDown : RightMouseButton"));
			
			if (InventoryComponent) // 인벤토리 컴포넌트가 있으면 인벤토리에서 우클릭을 한 것 
			{
				UE_LOG(LogTemp, Warning, TEXT("Dongwook : InventoryComponent exists"));
				// 카테고리로 다시 분류
				EItemCategory Category = ItemCategory;
				
				if (Category == EItemCategory::Heal || Category == EItemCategory::Booster) // 힐, 부스터
				{
					UE_LOG(LogTemp, Warning, TEXT("Dongwook : category healorbooster"));
					UE_LOG(LogTemp, Warning, TEXT("NativeOnPreviewMouseButtonDown : RightMouseButton = Category == 10, 11"));

					InventoryComponent->RemoveFromInventory(Index, true);
					UE_LOG(LogTemp, Warning, TEXT("Dongwook : Index :%d"),Index);
				}
				else if (Category == EItemCategory::WeaponPart) // 파츠
				{
					UE_LOG(LogTemp, Warning, TEXT("NativeOnPreviewMouseButtonDown : RightMouseButton = Category == 12"));

					FItemSlotStruct ItemSlot;
					ItemSlot.Quantity = Quantity;
					ItemSlot.ItemName = ItemName;
					ItemSlot.ItemCategory = ItemCategory;

					APlayerCharacter* PlayerCharacter =  Cast<APlayerCharacter>(InventoryComponent->GetOwner());
					EquippedComponent = PlayerCharacter->GetEquippedComponent();

					if (EquippedComponent->EquipParts(nullptr, -1, &ItemSlot))
					{
						UE_LOG(LogTemp, Warning, TEXT("UItemSlotWidget::NativeOnPreviewMouseButtonDown = EquippedComponent->ServerEquipParts : true"));
						UE_LOG(LogTemp, Warning, TEXT("UItemSlotWidget::NativeOnPreviewMouseButtonDown = EquippedComponent->ServerEquipParts : GetContent[%d]"), Index);

						InventoryComponent->ServerRemoveAt(Index);
					}
					
					//InventoryComponent->RemoveFromInventory(Index, true);
				}
				else if (Category == EItemCategory::Throw)
				{
					UE_LOG(LogTemp, Warning, TEXT("NativeOnPreviewMouseButtonDown : RightMouseButton = Category == 12"));

					FItemSlotStruct ItemSlot;
					ItemSlot.Quantity = Quantity;
					ItemSlot.ItemName = ItemName;
					ItemSlot.ItemCategory = ItemCategory;

					APlayerCharacter* PlayerCharacter =  Cast<APlayerCharacter>(InventoryComponent->GetOwner());
					EquippedComponent = PlayerCharacter->GetEquippedComponent();

					EquippedComponent->ServerEquipThrow(nullptr, ItemSlot);
					
					UE_LOG(LogTemp, Warning, TEXT("UItemSlotWidget::NativeOnPreviewMouseButtonDown = EquippedComponent->ServerEquipParts : true"));
					UE_LOG(LogTemp, Warning, TEXT("UItemSlotWidget::NativeOnPreviewMouseButtonDown = EquippedComponent->ServerEquipParts : GetContent[%d]"), Index);

					if (InventoryComponent->GetContent()[Index].Quantity >1)
					{
						InventoryComponent->GetContent()[Index].Quantity -= 1;
					}
					else if (InventoryComponent->GetContent()[Index].Quantity  == 1)
					{
						InventoryComponent->ServerRemoveAt(Index);
					}
					
				}
			}
			else if (NearComponent)
			{
				UE_LOG(LogTemp, Warning, TEXT("EKeys::RightMouseButton = NearComponent"));

				APlayerCharacter* PlayerCharacter =  Cast<APlayerCharacter>(NearComponent->GetOwner());
				EquippedComponent = PlayerCharacter->GetEquippedComponent();
				InventoryComponent = PlayerCharacter->GetInventoryComponent();

				FItemStruct* Row = DataTable->FindRow<FItemStruct>(ItemName, TEXT("Find Row"));
				int32 Category = static_cast<int32>(Row->Category);
				
				if (AWeaponItem* Weapon = Cast<AWeaponItem>(NearComponent->GetGroundItems()[Index]))
				{
					if (Category == 0)
					{
						UE_LOG(LogTemp, Warning, TEXT("EKeys::RightMouseButton, ItemCategory = %d"), Category);
						EquippedComponent->ServerEquipMainItem(NearComponent->GetGroundItems()[Index]);
					}
					else if (Category == 2)
					{
						UE_LOG(LogTemp, Warning, TEXT("EKeys::RightMouseButton, ItemCategory = %d"), Category);
						EquippedComponent->ServerEquipSubWeapon(NearComponent->GetGroundItems()[Index]);
					}
				}
				else if (AArmorItem* Armor = Cast<AArmorItem>(NearComponent->GetGroundItems()[Index]))
				{
					if (Category == 5 || Category == 6 || Category == 7)
					{
						UE_LOG(LogTemp, Warning, TEXT("EKeys::RightMouseButton, ItemCategory = %d"), Category);
						EquippedComponent->ServerEquiptHelmet(NearComponent->GetGroundItems()[Index]);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("EKeys::RightMouseButton, ItemCategory = %d"), Category);

					InventoryComponent->ServerSetNearItem(NearComponent->GetGroundItems()[Index]);
					InventoryComponent->Server_InteractItem(PlayerCharacter->GetInventoryComponent()->GetNearItem());
			
					// if (ABasePlayerController* PlayerController = Cast<ABasePlayerController>(GetOwningPlayer()))
					// {
					// 	PlayerController->GetInventoryWidget()->UpdateInventoryWidget();
					// }

					PlayerCharacter->GetInventoryComponent()->SetNearItem(nullptr);
				}
			}
		}
	}
	return FReply::Unhandled();
}

void UItemSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UClass* DragItemWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/DDItem/WBP_DragItem.WBP_DragItem_C"));
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayer()->GetPawn());

	if (DragItemWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemName.ToString());
		DragItemWidget = CreateWidget<UDragItemWidget>(this, DragItemWidgetClass);
		if (DragItemWidget)
		{
			DragItemWidget->SetItemName(ItemName);
			DragItemWidget->SetQuantity(Quantity);
			DragItemWidget->UpdateDragItemWidget();
			DragItemWidget->SetInventoryWidget(InventoryWidget);
		
			UDDInventorySlot* DDInventorySlot = NewObject<UDDInventorySlot>(this);
			if (DDInventorySlot)
			{
				DDInventorySlot->DefaultDragVisual = DragItemWidget;
				
				if (InventoryComponent)
				{
					DDInventorySlot->SetInventoryComponent(InventoryComponent);
				}
				else if (NearComponent)
				{
					DDInventorySlot->SetNearComponent(NearComponent);
				}
				
				DDInventorySlot->SetContentIndex(Index);
				OutOperation = DDInventorySlot;


			// 드래그 시 특정 위젯 변수의 영역에 있는지 확인
				// 1. 시작한 영역인지 확인 (마우스의 시작 / 현재 지점이 어느 영역에 있는지 비교하여 진행)
				// if (CheckItemZoneType(StartMousePoint) == CheckItemZoneType(InMouseEvent))
				// {
				// 	UE_LOG(LogTemp, Warning, TEXT("Same Location"));
				// 	return;
				// }
				// // 2. 다르면 그 영역에 대한 UI 처리
				// else
				// {
				// 	ItemZoneType = CheckItemZoneType(InMouseEvent);
				// 	UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
				// 	if (ItemZoneType == EItemZoneType::NoneArea)
				// 	{
				// 		
				// 		return;
				// 	}
				// 	else if (ItemZoneType == EItemZoneType::NearArea)
				// 	{
				//
				// 		return;
				// 	}
				// 	else if (ItemZoneType == EItemZoneType::InventoryArea)
				// 	{
				//
				// 		return;
				// 	}
				// 	else if (ItemZoneType == EItemZoneType::EquippedArea)
				// 	{
				// 		// todo : UI 업데이트 
				// 		return;
				// 	}
				// }
				
				
			}
			// DDInventory 생성 실패
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed DDInventorySlot"));
			}
		// DragItemWidget 생성 실패
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed DragItemWidget"));
		}
	// DragItemWidgetClass 불러오기 실패 
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed DragItemWidgetClass"));
	}
}

// bool UItemSlotWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
// 	UDragDropOperation* InOperation)
// {
// 	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
//
// 	ItemZoneType = CheckItemZoneType(InDragDropEvent);
// 	UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
// 	
// 	if (ItemZoneType == EItemZoneType::NoneArea)
// 	{
// 						
// 		return true;
// 	}
// 	else if (ItemZoneType == EItemZoneType::NearArea)
// 	{
//
// 		return true;
// 	}
// 	else if (ItemZoneType == EItemZoneType::InventoryArea)
// 	{
//
// 		return true;
// 	}
// 	else if (ItemZoneType == EItemZoneType::EquippedArea)
// 	{
// 		// todo : UI 업데이트 
// 		return true;
// 	}
//
// 	// if (CheckItemZoneType(StartMousePoint) == CheckItemZoneType(InDragDropEvent))
// 	// {
// 	// 	UE_LOG(LogTemp, Warning, TEXT("Same Location"));
// 	// 	return true;
// 	// }
// 	// // 2. 다르면 그 영역에 대한 UI 처리
// 	// else
// 	// {
// 	// 	ItemZoneType = CheckItemZoneType(InDragDropEvent);
// 	// 	UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
// 	// 	if (ItemZoneType == EItemZoneType::NoneArea)
// 	// 	{
// 	// 					
// 	// 		return true;
// 	// 	}
// 	// 	else if (ItemZoneType == EItemZoneType::NearArea)
// 	// 	{
// 	//
// 	// 		return true;
// 	// 	}
// 	// 	else if (ItemZoneType == EItemZoneType::InventoryArea)
// 	// 	{
// 	//
// 	// 		return true;
// 	// 	}
// 	// 	else if (ItemZoneType == EItemZoneType::EquippedArea)
// 	// 	{
// 	// 		// todo : UI 업데이트 
// 	// 		return true;
// 	// 	}
// 	// }
// 	//
// 	return false;
// }
//
// bool UItemSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
//                                    UDragDropOperation* InOperation)
// {
// 	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
//
// 	UDDInventorySlot* DDInventorySlot = Cast<UDDInventorySlot>(InOperation);
//
// 	if (!DDInventorySlot || ItemZoneType == EItemZoneType::None)
// 	{
// 		return false;
// 	}
// 	
// 	UE_LOG(LogTemp, Warning, TEXT("Drop ItemZoneType : %d"), ItemZoneType);
//
// 	if (ItemZoneType == EItemZoneType::NoneArea)
// 	{
// 		return true;
// 	}
// 	else if (ItemZoneType == EItemZoneType::NearArea)
// 	{
//
// 		return true;
// 	}
// 	else if (ItemZoneType == EItemZoneType::InventoryArea)
// 	{
//
// 		return true;
// 	}
// 	else if (ItemZoneType == EItemZoneType::EquippedArea)
// 	{
// 		// todo : UI 업데이트 
// 		return true;
// 	}
//
// 	return false;
// }
//
// EItemZoneType UItemSlotWidget::CheckItemZoneType(FPointerEvent InMousePoint)
// {
// 	if (VerticalBox_Near || VerticalBox_Inventory)
// 	{
// 		return EItemZoneType::None;
// 	}
// 	
// 	FVector2D  CurrentMouseSpace = InMousePoint.GetScreenSpacePosition();
//
// 	// 주변 위젯 사이즈
// 	FGeometry VerticalBox_NearGeometry = VerticalBox_Near->GetCachedGeometry();
// 	FVector2D VerticalBox_NearPos = VerticalBox_NearGeometry.GetAbsolutePosition();
// 	FVector2D VerticalBox_NearSize = VerticalBox_NearGeometry.GetLocalSize();
//
// 	// 인벤 위젯 사이즈
// 	FGeometry VerticalBox_InventoryGeometry = VerticalBox_Inventory->GetCachedGeometry();
// 	FVector2D VerticalBox_InventoryPos = VerticalBox_InventoryGeometry.GetAbsolutePosition();
// 	FVector2D VerticalBox_InventorySize = VerticalBox_InventoryGeometry.GetLocalSize();
//
// 	// todo : 장작된 장비 사이즈
//
// 	if (CurrentMouseSpace.X >= VerticalBox_NearPos.X && CurrentMouseSpace.X <= VerticalBox_NearPos.X + VerticalBox_NearSize.X &&
// 		CurrentMouseSpace.Y >= VerticalBox_NearPos.Y && CurrentMouseSpace.Y <= VerticalBox_NearPos.Y + VerticalBox_NearSize.Y)
// 	{
// 		return EItemZoneType::NearArea;
// 	}
// 	else if (CurrentMouseSpace.X >= VerticalBox_InventoryPos.X && CurrentMouseSpace.X <= VerticalBox_InventoryPos.X + VerticalBox_InventorySize.X &&
// 		CurrentMouseSpace.Y >= VerticalBox_InventoryPos.Y && CurrentMouseSpace.Y <= VerticalBox_InventoryPos.Y + VerticalBox_InventorySize.Y)
// 	{
// 		return EItemZoneType::InventoryArea;
// 	}
// 	// todo : 장착된 장비랑도 비교
// 	else
// 	{
// 		return EItemZoneType::NoneArea;
// 	}
// }
//
// EItemZoneType UItemSlotWidget::CheckItemZoneType(FDragDropEvent InDragDropEvent)
// {
// 	if (VerticalBox_Near || VerticalBox_Inventory)
// 	{
// 		return EItemZoneType::None;
// 	}
// 	
// 	FVector2D  CurrentMouseSpace = InDragDropEvent.GetScreenSpacePosition();
//
// 	// 주변 위젯 사이즈
// 	FGeometry VerticalBox_NearGeometry = VerticalBox_Near->GetCachedGeometry();
// 	FVector2D VerticalBox_NearPos = VerticalBox_NearGeometry.GetAbsolutePosition();
// 	FVector2D VerticalBox_NearSize = VerticalBox_NearGeometry.GetLocalSize();
//
// 	// 인벤 위젯 사이즈
// 	FGeometry VerticalBox_InventoryGeometry = VerticalBox_Inventory->GetCachedGeometry();
// 	FVector2D VerticalBox_InventoryPos = VerticalBox_InventoryGeometry.GetAbsolutePosition();
// 	FVector2D VerticalBox_InventorySize = VerticalBox_InventoryGeometry.GetLocalSize();
//
// 	// todo : 장작된 장비 사이즈
//
// 	if (CurrentMouseSpace.X >= VerticalBox_NearPos.X && CurrentMouseSpace.X <= VerticalBox_NearPos.X + VerticalBox_NearSize.X &&
// 		CurrentMouseSpace.Y >= VerticalBox_NearPos.Y && CurrentMouseSpace.Y <= VerticalBox_NearPos.Y + VerticalBox_NearSize.Y)
// 	{
// 		return EItemZoneType::NearArea;
// 	}
// 	else if (CurrentMouseSpace.X >= VerticalBox_InventoryPos.X && CurrentMouseSpace.X <= VerticalBox_InventoryPos.X + VerticalBox_InventorySize.X &&
// 		CurrentMouseSpace.Y >= VerticalBox_InventoryPos.Y && CurrentMouseSpace.Y <= VerticalBox_InventoryPos.Y + VerticalBox_InventorySize.Y)
// 	{
// 		return EItemZoneType::InventoryArea;
// 	}
// 	// todo : 장착된 장비랑도 비교
// 	else
// 	{
// 		return EItemZoneType::NoneArea;
// 	}
// }
