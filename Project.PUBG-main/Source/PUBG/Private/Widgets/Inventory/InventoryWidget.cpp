// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/InventoryWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Component/Equipped/EquippedComponent.h"
#include "Component/ItemData/ItemDataComponent.h"
#include "Component/NearArea/NearComponent.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/WrapBox.h"
#include "Controller/BasePlayerController.h"
#include "DragDrop/DDInventorySlot.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Item/ItemBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Inventory/ItemSlotWidget.h"
#include "Widgets/Inventory/WeaponSlotWidget.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Item/Armor/Armor_Base.h"
#include "Weapon/DataTable/DT_PartsData.h"
#include "Widgets/Inventory/ArmorSlotWidget.h"
#include "Widgets/Inventory/WeaponPartsSlot.h"
#include "Widgets/Inventory/WeaponSlotWidget1.h"


UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InventoryComponent = nullptr;
	ItemSlotWidget  = nullptr;
	WrapBox_Inventory  = nullptr;

	ItemSlotWidgetBPClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/ItemSlot/WBP_ItemSlot.WBP_ItemSlot_C"));
	WeaponSlotWidgetBPClass1 = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/ItemSlot/WBP_WeaponSlot1.WBP_WeaponSlot1_C"));
	WeaponSlotWidgetBPClass2 = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/ItemSlot/WBP_WeaponSlot2.WBP_WeaponSlot2_C"));
	WeaponSlotWidgetBPClass3 = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/ItemSlot/WBP_WeaponSlot3.WBP_WeaponSlot3_C"));
	WeaponSlotWidgetBPClass11 = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/ItemSlot/WBP_WeaponSlot11.WBP_WeaponSlot11_C"));
	WeaponSlotWidgetBPClass22 = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/ItemSlot/WBP_WeaponSlot22.WBP_WeaponSlot22_C"));
	WeaponSlotWidgetBPClass33 = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/ItemSlot/WBP_WeaponSlot33.WBP_WeaponSlot33_C"));
	GrenadeSlotWidgetBPClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Blueprint/Widgets/ItemSlot/WBP_Grande.WBP_Grande_C"));
	
	
	ItemZoneType = EItemZoneType::None;

}

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	
}

void UInventoryWidget::UpdateInventoryWidget()
{
	
	if (!GetWrapBox_Inventory())
	{
		//UE_LOG(LogTemp, Warning, TEXT("GetWrapBox_Inventory() is NULL"));
		return;
	}
	
	GetWrapBox_Inventory()->ClearChildren();
	
	TArray<FItemSlotStruct> ItemSlot = InventoryComponent->GetContent();
	for (int i = 0; i < ItemSlot.Num(); i++)
	{
		if (ItemSlotWidgetBPClass)
		{
			ItemSlotWidget = CreateWidget<UItemSlotWidget>(GetWorld(), ItemSlotWidgetBPClass);
			if (ItemSlotWidget != nullptr)
			{
				ItemSlotWidget->SetItemName(ItemSlot[i].ItemName);
				//ItemSlotWidget->GetItemName
				ItemSlotWidget->SetQuantity(ItemSlot[i].Quantity);
				ItemSlotWidget->SetInventoryComponent(InventoryComponent);
				ItemSlotWidget->SetIndex(i);
				ItemSlotWidget->SetItemCategory(ItemSlot[i].ItemCategory);
				
				int32 Category = static_cast<int32>(ItemSlot[i].ItemCategory);
				
				if (Category == 12)
				{
					//UE_LOG(LogTemp, Warning, TEXT("UpdateInventoryWidget = Category12"));
					ItemSlotWidget->SetEquippedComponent(EquippedComponent);
				}
				
				ItemSlotWidget->SetInventoryWidget(this);
				
				FString text = TEXT("사용");
				ItemSlotWidget->GetTextBlock_Use()->SetText(FText::FromString(text));
				
				ItemSlotWidget->UpdateItemSlotWidget();
				GetWrapBox_Inventory()->AddChildToWrapBox(ItemSlotWidget);
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("ItemSlot Widget not found"));
		}
	}
}

void UInventoryWidget::UpdateNearItemSlotWidget()
{
	GetWrapBox_Near()->ClearChildren();
	//NearComponent->ServerGetGroundItem();
	TArray<AItemBase*> ItemSlot = NearComponent->GetGroundItems();
	
	//UE_LOG(LogTemp, Warning, TEXT("ItemSlot Num : %d"), ItemSlot.Num());
	//UE_LOG(LogTemp, Warning, TEXT("GroundItem[0] : %s"), *NearComponent->GetGroundItems()[0]->GetItemStruct().Name.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("ItemSlot : %d"), ItemSlot.Num());
	
	for (int i = 0; i < ItemSlot.Num(); i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ItemSlot Widget not found"));
		if (ItemSlotWidgetBPClass)
		{
			NearItemSlotWidget = CreateWidget<UItemSlotWidget>(GetWorld(), ItemSlotWidgetBPClass);
			if (NearItemSlotWidget != nullptr)
			{
				if (ItemSlot[i] != nullptr)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Name : %s"), *ItemSlot[i]->GetItemStruct().Name.ToString());
					//UE_LOG(LogTemp, Warning, TEXT("Name : %s"), *ItemSlot[i]->GetItemDataComponent()->GetItemRowName().ToString());
					//NearItemSlotWidget->SetItemName(ItemSlot[i]->GetItemStruct().Name);
					NearItemSlotWidget->SetItemName(ItemSlot[i]->GetItemDataComponent()->GetItemRowName());
					//NearItemSlotWidget->SetQuantity(ItemSlot[i]->GetItemStruct().Quantity);
					NearItemSlotWidget->SetQuantity(ItemSlot[i]->GetItemDataComponent()->GetQuantity());
					NearItemSlotWidget->SetNearComponent(NearComponent);
					//NearItemSlotWidget->SetInventoryComponent(InventoryComponent);
					NearItemSlotWidget->SetIndex(i);
					NearItemSlotWidget->SetInventoryWidget(this);
					
					FString text = TEXT("줍기");
					NearItemSlotWidget->GetTextBlock_Use()->SetText(FText::FromString(text));
					//NearItemSlotWidget->GetTextBlock_Use()->SetText(LOCTEXT("UseText", "사용"));
					
					NearItemSlotWidget->UpdateItemSlotWidget();
					GetWrapBox_Near()->AddChildToWrapBox(NearItemSlotWidget);
				}
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("ItemSlot Widget not found"));
		}
	}
	
	ItemSlot.Empty();
}

void UInventoryWidget::UpdateEquippedWidget()
{
	if (!WeaponSlotWidgetBPClass1 &&!WeaponSlotWidgetBPClass2 && !WeaponSlotWidgetBPClass3 && !EquippedComponent && !WeaponSlotWidgetBPClass11)
	{
		//UE_LOG(LogTemp, Warning, TEXT("WeaponSlotWidgetClass None Or EquippedComponent None"));
		return;
	}
		
	TArray<AEquipableItem*> EquippedItems = EquippedComponent->GetEquippedItems();

	//UE_LOG(LogTemp, Warning, TEXT("EquippedItems Num : %d"), EquippedItems.Num());

	FString PartsDataTablePath = "/Game/Blueprint/Weapon/Datatable/DT_PartsData.DT_PartsData";
	UDataTable* PartsDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *PartsDataTablePath));
	
	EquippedUIInit();
	
	for (int i = 0; i < EquippedItems.Num(); i++)
	{
		if (EquippedItems[i] != nullptr)
		{
			switch (i)
			{
			case 0:
				Weapon11SlotWidget = CreateWidget<UWeaponSlotWidget1>(GetWorld(), WeaponSlotWidgetBPClass11);

				if (Weapon11SlotWidget)
				{
					AGun_Base* Slot1Weapon = Cast<AGun_Base>(EquippedItems[i]); // GunBase로 캐스팅
					Weapon11SlotWidget = EquippedWeaponUIUpdate(Weapon11SlotWidget, Slot1Weapon, i);

					if (Slot1Weapon)
					{
						PartsUIUpdate(PartsDataTable, Weapon11SlotWidget, Slot1Weapon);
						SizeBox_1Slot->SetContent(Weapon11SlotWidget);
						Weapon11SlotWidget->SetTextureParameter(Slot1Weapon->GetTextureRenderTarget());
						UE_LOG(LogTemp, Warning, TEXT("%d : SetContent"), i);
					}
				}
				break;
			case 1:
				Weapon22SlotWidget = CreateWidget<UWeaponSlotWidget1>(GetWorld(), WeaponSlotWidgetBPClass22);

				if (Weapon22SlotWidget)
				{
					UE_LOG(LogTemp, Warning, TEXT("Weapon1SlotWidget"));
					AGun_Base* Slot1Weapon = Cast<AGun_Base>(EquippedItems[i]); // GunBase로 캐스팅
					Weapon22SlotWidget = EquippedWeaponUIUpdate(Weapon22SlotWidget, Slot1Weapon, i);
					
					PartsUIUpdate(PartsDataTable, Weapon22SlotWidget, Slot1Weapon);
					SizeBox_2Slot->SetContent(Weapon22SlotWidget);
					Weapon22SlotWidget->SetTextureParameter(Slot1Weapon->GetTextureRenderTarget());
					UE_LOG(LogTemp, Warning, TEXT("%d : SetContent"), i);
					
				}
				break;
			case 2:
				Weapon33SlotWidget = CreateWidget<UWeaponSlotWidget1>(GetWorld(), WeaponSlotWidgetBPClass33);

				if (Weapon33SlotWidget)
				{
					UE_LOG(LogTemp, Warning, TEXT("Weapon1SlotWidget"));
					AGun_Base* Slot1Weapon = Cast<AGun_Base>(EquippedItems[i]); // GunBase로 캐스팅
					Weapon33SlotWidget = EquippedWeaponUIUpdate(Weapon33SlotWidget, Slot1Weapon, i);

					PartsUIUpdate(PartsDataTable, Weapon33SlotWidget, Slot1Weapon);
					SizeBox_3Slot->SetContent(Weapon33SlotWidget);
					Weapon33SlotWidget->SetTextureParameter(Slot1Weapon->GetTextureRenderTarget());
					UE_LOG(LogTemp, Warning, TEXT("%d : SetContent"), i);
				}
				break;
			case 4:
				UE_LOG(LogTemp, Warning, TEXT("Case 4"));
				GrenadeSlotWidget = CreateWidget<UGrandeSlotWIdget>(GetWorld(), GrenadeSlotWidgetBPClass);
				if (GrenadeSlotWidget)
				{
					UE_LOG(LogTemp, Warning, TEXT("Weapon1SlotWidget"));
					AGrenade_Base* Slot5Grennade = Cast<AGrenade_Base>(EquippedItems[i]); // GunBase로 캐스팅

					FString SlotNumber =  FString::FromInt(i + 1);
					GrenadeSlotWidget->GetTextSlotNumber()->SetText(FText::FromString(SlotNumber)); // 슬롯 번호 지정
					GrenadeSlotWidget->GetTextGrenadeName()->SetText(FText::FromString(Slot5Grennade->GetWeaponDataAsset().GunName));
					GrenadeSlotWidget->GetTextGrenadeName()->SetText(FText::FromString(Slot5Grennade->GetWeaponDataAsset().GunName));

					FName GrenadeName = FName(Slot5Grennade->GetWeaponDataAsset().GunName);

					if (Slot5Grennade->GetItemDataTable())
					{
						UE_LOG(LogTemp, Warning, TEXT("Slot5 ItemDataTable"));
						UE_LOG(LogTemp, Warning, TEXT("Slot5 %s"), *GrenadeName.ToString());
					}
					FItemStruct* Row = Slot5Grennade->GetItemDataTable()->FindRow<FItemStruct>(GrenadeName, TEXT("Find Row"));

					GrenadeSlotWidget->GetImageGrenade()->SetBrushFromTexture(Row->Image);
					GrenadeSlotWidget->SetName(GrenadeName);
					GrenadeSlotWidget->SetInventoryComponent(InventoryComponent);
					GrenadeSlotWidget->SetEquippedComponent(EquippedComponent);
					GrenadeSlotWidget->SetIndex(i);

					SizeBox_5Slot->SetContent(GrenadeSlotWidget);
					UE_LOG(LogTemp, Warning, TEXT("%d : SetContent"), i);
					
				}

			case 5:
				if (WBP_ArmorSlot_Helmet)
				{
					if (AArmor_Base* SlotHelmet = Cast<AArmor_Base>(EquippedItems[i])) // GunBase로 캐스팅
					{
						WBP_ArmorSlot_Helmet->GetImageArmor()->SetVisibility(ESlateVisibility::Visible);
						WBP_ArmorSlot_Helmet->GetImageArmor()->SetBrushFromTexture(SlotHelmet->GetArmorData().Image);
						WBP_ArmorSlot_Helmet->SetArmorName(SlotHelmet->GetArmorData().Name);
						WBP_ArmorSlot_Helmet->SetArmorIndex(i);
						WBP_ArmorSlot_Helmet->SetEquippedComponent(EquippedComponent);
					}
					else
					{
						WBP_ArmorSlot_Helmet->GetImageArmor()->SetVisibility(ESlateVisibility::Collapsed);
						WBP_ArmorSlot_Helmet->GetImageArmor()->SetBrushFromTexture(nullptr);
						WBP_ArmorSlot_Helmet->SetEquippedComponent(nullptr);
					}
				}
				break;
			case 6:
				if (WBP_ArmorSlot_Bag)
				{
					UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::UpdateEquippedWidget = WBP_ArmorSlot_Bag"));
					
					if (AArmor_Base* SlotBag = Cast<AArmor_Base>(EquippedItems[i])) // GunBase로 캐스팅
					{
						UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::UpdateEquippedWidget = SlotBag %s"), *SlotBag->GetActorNameOrLabel());
						WBP_ArmorSlot_Bag->GetImageArmor()->SetBrushFromTexture(SlotBag->GetArmorData().Image);
						WBP_ArmorSlot_Bag->SetArmorName(SlotBag->GetArmorData().Name);
						WBP_ArmorSlot_Bag->GetImageArmor()->SetVisibility(ESlateVisibility::HitTestInvisible);
						WBP_ArmorSlot_Bag->SetArmorIndex(i);

						if (EquippedComponent)
						{
							WBP_ArmorSlot_Bag->SetEquippedComponent(EquippedComponent);
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::UpdateEquippedWidget = EquippedComponent None"));
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::UpdateEquippedWidget = SlotBag None"));
						WBP_ArmorSlot_Bag->GetImageArmor()->SetVisibility(ESlateVisibility::Collapsed);
						WBP_ArmorSlot_Bag->GetImageArmor()->SetBrushFromTexture(nullptr);
						WBP_ArmorSlot_Bag->SetEquippedComponent(nullptr);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::UpdateEquippedWidget = WBP_ArmorSlot_Bag None"));
				}
				break;
			case 7:
				if (WBP_ArmorSlot_Armor)
				{
					if (AArmor_Base* SlotArmor = Cast<AArmor_Base>(EquippedItems[i])) // GunBase로 캐스팅
					{
						WBP_ArmorSlot_Armor->GetImageArmor()->SetVisibility(ESlateVisibility::HitTestInvisible);
						WBP_ArmorSlot_Armor->GetImageArmor()->SetBrushFromTexture(SlotArmor->GetArmorData().Image);
						WBP_ArmorSlot_Armor->SetArmorName(SlotArmor->GetArmorData().Name);
						WBP_ArmorSlot_Armor->SetArmorIndex(i);
						WBP_ArmorSlot_Armor->SetEquippedComponent(EquippedComponent);
					}
					else
					{
						WBP_ArmorSlot_Armor->GetImageArmor()->SetVisibility(ESlateVisibility::Collapsed);
						WBP_ArmorSlot_Armor->GetImageArmor()->SetBrushFromTexture(nullptr);
						WBP_ArmorSlot_Armor->SetEquippedComponent(nullptr);
					}
				}
				break;
			default: ;
			}
		}
	}
}



bool UInventoryWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	UDDInventorySlot* DDInventorySlot = Cast<UDDInventorySlot>(InOperation);
	ItemZoneType = CheckItemZoneType(InDragDropEvent);
	
	if (!DDInventorySlot || ItemZoneType == EItemZoneType::None)
	{
		return false;
	}
	
	if (ItemZoneType == EItemZoneType::NoneArea)
	{
					
		//UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
		return true;
	}
	else if (ItemZoneType == EItemZoneType::NearArea && !DDInventorySlot->GetNearComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
		return true;
	}
	else if (ItemZoneType == EItemZoneType::InventoryArea && !DDInventorySlot->GetInventoryComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
		return true;
	}
	else if (ItemZoneType == EItemZoneType::EquippedArea)
	{
		// todo : UI 업데이트 
		return true;
	}
	// if (CheckItemZoneType(StartMousePoint) == CheckItemZoneType(InDragDropEvent))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Same Location"));
	// 	return true;
	// }
	// // 2. 다르면 그 영역에 대한 UI 처리
	// else
	// {
	// 	ItemZoneType = CheckItemZoneType(InDragDropEvent);
	// 	UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
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
	// }
	//
	return false;

	
}

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	UDDInventorySlot* DDInventorySlot = Cast<UDDInventorySlot>(InOperation);
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayer()->GetPawn());
	
	if (!DDInventorySlot || ItemZoneType == EItemZoneType::None || !PlayerCharacter)
	{
		return false;
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("Drop ItemZoneType : %d"), ItemZoneType);
	
	if (ItemZoneType == EItemZoneType::NoneArea)
	{
					
		//UE_LOG(LogTemp, Warning, TEXT("Drag ItemZoneType : %d"), ItemZoneType);
		return true;
	}
	else if (ItemZoneType == EItemZoneType::NearArea && !DDInventorySlot->GetNearComponent()) // 놓는 위치 확인 및 시작 위치 컴포넌트 확인
	{
		UE_LOG(LogTemp, Warning, TEXT("Drop ItemZoneType : %d"), ItemZoneType);
		
		
		// DDInventorySlot에 담긴 값을 NearComponent에 추가
		switch (DDInventorySlot->GetHaveComponent())
		{
		case 2: // 시작이 인벤토리 도착이 바닥
			UE_LOG(LogTemp, Warning, TEXT("Start : Inventory, End : Near"));
			PlayerCharacter->GetInventoryComponent()->RemoveFromInventory(DDInventorySlot->GetContentIndex(),
																			false,
																			DDInventorySlot->GetInventoryComponent()->GetContent()[DDInventorySlot->GetContentIndex()].Quantity);
			return true;
		case 3: // 시작이 장작된 장비 도착이 바닥 
			return true;
			
		}
		
		return true;
	}
	else if (ItemZoneType == EItemZoneType::InventoryArea && !DDInventorySlot->GetInventoryComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("Drop ItemZoneType : %d"), ItemZoneType);
		// DDInventorySlot에 담긴 값을 InventoryComponent에 추가
		switch (DDInventorySlot->GetHaveComponent())
		{
		case 1: // 시작이 주위 도착이 인벤
			UE_LOG(LogTemp, Warning, TEXT("Start : Near, End : Inventory"));
			
			PlayerCharacter->GetInventoryComponent()->ServerSetNearItem(DDInventorySlot->GetNearComponent()->GetGroundItems()[DDInventorySlot->GetContentIndex()]);
			
			PlayerCharacter->GetInventoryComponent()->Server_InteractItem(PlayerCharacter->GetInventoryComponent()->GetNearItem());
			
			if (ABasePlayerController* PlayerController = Cast<ABasePlayerController>(GetOwningPlayer()))
			{
				PlayerController->GetInventoryWidget()->UpdateInventoryWidget();
			}

			PlayerCharacter->GetInventoryComponent()->SetNearItem(nullptr);
			
			return true;
		case 3: // 시작이 장작된 장비 도착이 인벤 
			return true;
		}
		
		return true;
	}
	else if (ItemZoneType == EItemZoneType::EquippedArea)
	{
		// todo : UI 업데이트 
		return true;
	}
	
	return false;
	
}

EItemZoneType UInventoryWidget::CheckItemZoneType(FPointerEvent InMousePoint)
{
	if (VerticalBox_Near || VerticalBox_Inventory)
	{
		return EItemZoneType::None;
	}
	
	FVector2D  CurrentMouseSpace = InMousePoint.GetScreenSpacePosition();

	// 주변 위젯 사이즈
	FGeometry VerticalBox_NearGeometry = VerticalBox_Near->GetCachedGeometry();
	FVector2D VerticalBox_NearPos = VerticalBox_NearGeometry.GetAbsolutePosition();
	FVector2D VerticalBox_NearSize = VerticalBox_NearGeometry.GetLocalSize();

	// 인벤 위젯 사이즈
	FGeometry VerticalBox_InventoryGeometry = VerticalBox_Inventory->GetCachedGeometry();
	FVector2D VerticalBox_InventoryPos = VerticalBox_InventoryGeometry.GetAbsolutePosition();
	FVector2D VerticalBox_InventorySize = VerticalBox_InventoryGeometry.GetLocalSize();

	// todo : 장작된 장비 사이즈

	if (CurrentMouseSpace.X >= VerticalBox_NearPos.X && CurrentMouseSpace.X <= VerticalBox_NearPos.X + VerticalBox_NearSize.X &&
		CurrentMouseSpace.Y >= VerticalBox_NearPos.Y && CurrentMouseSpace.Y <= VerticalBox_NearPos.Y + VerticalBox_NearSize.Y)
	{
		return EItemZoneType::NearArea;
	}
	else if (CurrentMouseSpace.X >= VerticalBox_InventoryPos.X && CurrentMouseSpace.X <= VerticalBox_InventoryPos.X + VerticalBox_InventorySize.X &&
		CurrentMouseSpace.Y >= VerticalBox_InventoryPos.Y && CurrentMouseSpace.Y <= VerticalBox_InventoryPos.Y + VerticalBox_InventorySize.Y)
	{
		return EItemZoneType::InventoryArea;
	}
	// todo : 장착된 장비랑도 비교
	else
	{
		return EItemZoneType::NoneArea;
	}
}

EItemZoneType UInventoryWidget::CheckItemZoneType(FDragDropEvent InDragDropEvent)
{
	if (!VerticalBox_Near || !VerticalBox_Inventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("VerticalBox None"));
		return EItemZoneType::None;
	}

	// 커서 위치 확인
	FVector2D  CurrentMouseSpace = InDragDropEvent.GetScreenSpacePosition();

	// 주변 위젯 사이즈
	FGeometry VerticalBox_NearGeometry = VerticalBox_Near->GetCachedGeometry();
	FVector2D VerticalBox_NearPos = VerticalBox_NearGeometry.GetAbsolutePosition();
	FVector2D VerticalBox_NearSize = VerticalBox_NearGeometry.GetAbsoluteSize();
	
	//UE_LOG(LogTemp, Warning, TEXT("NearPos : %f %f"), VerticalBox_NearPos.X, VerticalBox_NearPos.Y);
	//UE_LOG(LogTemp, Warning, TEXT("NearSize : %f %f"), VerticalBox_NearSize.X, VerticalBox_NearSize.Y);

	// 인벤 위젯 사이즈
	FGeometry VerticalBox_InventoryGeometry = VerticalBox_Inventory->GetCachedGeometry();
	FVector2D VerticalBox_InventoryPos = VerticalBox_InventoryGeometry.GetAbsolutePosition();
	//UE_LOG(LogTemp, Warning, TEXT("InventoryPos : %f %f"), VerticalBox_InventoryPos.X, VerticalBox_InventoryPos.Y);
	FVector2D VerticalBox_InventorySize = VerticalBox_InventoryGeometry.GetAbsoluteSize();

	// todo : 장작된 장비 사이즈

	
	if (CurrentMouseSpace.X >= VerticalBox_NearPos.X && CurrentMouseSpace.X <= VerticalBox_NearPos.X + VerticalBox_NearSize.X &&
		CurrentMouseSpace.Y >= VerticalBox_NearPos.Y && CurrentMouseSpace.Y <= VerticalBox_NearPos.Y + VerticalBox_NearSize.Y)
	{
		return EItemZoneType::NearArea;
	}
	else if (CurrentMouseSpace.X >= VerticalBox_InventoryPos.X && CurrentMouseSpace.X <= VerticalBox_InventoryPos.X + VerticalBox_InventorySize.X &&
		CurrentMouseSpace.Y >= VerticalBox_InventoryPos.Y && CurrentMouseSpace.Y <= VerticalBox_InventoryPos.Y + VerticalBox_InventorySize.Y)
	{
		return EItemZoneType::InventoryArea;
	}
	// todo : 장착된 장비랑도 비교
	else
	{
		return EItemZoneType::NoneArea;
	}
}

void UInventoryWidget::EquippedUIInit()
{
	UE_LOG(LogTemp,Warning,TEXT("UInventoryWidget::EquippedUIInit"));

	if (IsValid(Weapon11SlotWidget))
	{
		Weapon11SlotWidget->RemoveFromParent();
		Weapon11SlotWidget->ConditionalBeginDestroy(); // 즉시 삭제
		Weapon11SlotWidget = nullptr;
	}
	if (IsValid(Weapon22SlotWidget))
	{
		Weapon22SlotWidget->RemoveFromParent();
		Weapon22SlotWidget->ConditionalBeginDestroy(); // 즉시 삭제
		Weapon22SlotWidget = nullptr;
	}
	if (IsValid(Weapon33SlotWidget))
	{
		Weapon33SlotWidget->RemoveFromParent();
		Weapon33SlotWidget->ConditionalBeginDestroy(); // 즉시 삭제
		Weapon33SlotWidget = nullptr;
	}

	if (IsValid(WBP_ArmorSlot_Helmet))
	{
		WBP_ArmorSlot_Helmet->InitArmorSlot();
	}
	if (IsValid(WBP_ArmorSlot_Bag))
	{
		WBP_ArmorSlot_Bag->InitArmorSlot();
	}
	if (IsValid(WBP_ArmorSlot_Armor))
	{
		WBP_ArmorSlot_Armor->InitArmorSlot();
	}
	if (IsValid(GrenadeSlotWidget))
	{
		GrenadeSlotWidget->RemoveFromParent();
		GrenadeSlotWidget->ConditionalBeginDestroy(); // 즉시 삭제
		GrenadeSlotWidget = nullptr;
	}
}

UWeaponSlotWidget* UInventoryWidget::EquippedWeaponUIUpdate(UWeaponSlotWidget* OutWeaponSlotWidget, AGun_Base* OutGunBase, int32 OutIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::EquippedWeaponUIUpdate"));					
	OutWeaponSlotWidget->FindEquiablePartsSlot(OutGunBase);

	FString SlotNumber =  FString::FromInt(OutIndex + 1);
	OutWeaponSlotWidget->GetTextSlotNumber()->SetText(FText::FromString(SlotNumber)); // 슬롯 번호 지정 
					
	FString WeaponName = OutGunBase->GetWeaponDataAsset().GunName;
	OutWeaponSlotWidget->GetTextWeaponName()->SetText(FText::FromString(WeaponName)); // 총 이름 지정 


	// 장전된 총알, 남은 총알 
					
	EBulletType WeaponBulletType = OutGunBase->GetWeaponDataAsset().BulletType;
	FString WeaponBulletName = OutWeaponSlotWidget->SetBulletTypeTextBlock(WeaponBulletType);
	OutWeaponSlotWidget->GetTextAmmoName()->SetText(FText::FromString(WeaponBulletName)); // 총알 이름 지정

	FName WeaponBulletFName = FName(*WeaponBulletName);
	UE_LOG(LogTemp, Warning, TEXT("EquippedWeaponUIUpdate : %s"), *WeaponBulletName);
	FItemStruct* Row = OutGunBase->GetItemDataTable()->FindRow<FItemStruct>(WeaponBulletFName, TEXT("Find Row"));
	// 총데이터테이블
	if (Row != nullptr)
	{
		OutWeaponSlotWidget->GetImageAmmoImage()->SetRenderTranslation(FVector2D(-5.0f, 0.0f));
		OutWeaponSlotWidget->GetImageAmmoImage()->SetRenderScale(FVector2D(2.0f, 2.0f));
		OutWeaponSlotWidget->GetImageAmmoImage()->SetBrushFromTexture(Row->Image); // 총알 이미지 지정 
	}

	return OutWeaponSlotWidget;
}

UWeaponSlotWidget1* UInventoryWidget::EquippedWeaponUIUpdate(UWeaponSlotWidget1* OutWeaponSlotWidget,
	AGun_Base* OutGunBase, int32 OutIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::EquippedWeaponUIUpdate"));					
	OutWeaponSlotWidget->FindEquiablePartsSlot(OutGunBase);

	FString SlotNumber =  FString::FromInt(OutIndex + 1);
	OutWeaponSlotWidget->GetTextSlotNumber()->SetText(FText::FromString(SlotNumber)); // 슬롯 번호 지정 

	OutWeaponSlotWidget->SetWeaponName(FName(OutGunBase->GetWeaponDataAsset().GunName));
	OutWeaponSlotWidget->SetWeaponIndex(OutIndex);
	OutWeaponSlotWidget->SetEquipmentComponent(EquippedComponent);
	OutWeaponSlotWidget->SetVisibility(ESlateVisibility::Visible);
	OutWeaponSlotWidget->SetIsFocusable(true);
	
	FString WeaponName = OutGunBase->GetWeaponDataAsset().GunName;
	OutWeaponSlotWidget->GetTextWeaponName()->SetText(FText::FromString(WeaponName)); // 총 이름 지정
	
	// 장전된 총알, 남은 총알 
					
	EBulletType WeaponBulletType = OutGunBase->GetWeaponDataAsset().BulletType;
	FString WeaponBulletName = OutWeaponSlotWidget->SetBulletTypeTextBlock(WeaponBulletType);
	OutWeaponSlotWidget->GetTextAmmoName()->SetText(FText::FromString(WeaponBulletName)); // 총알 이름 지정

	FName WeaponBulletFName = FName(*WeaponBulletName);
	UE_LOG(LogTemp, Warning, TEXT("EquippedWeaponUIUpdate : %s"), *WeaponBulletName);
	FItemStruct* Row = OutGunBase->GetItemDataTable()->FindRow<FItemStruct>(WeaponBulletFName, TEXT("Find Row"));
	
	if (Row != nullptr)
	{
		OutWeaponSlotWidget->GetImageAmmoImage()->SetRenderTranslation(FVector2D(-5.0f, 0.0f));
		OutWeaponSlotWidget->GetImageAmmoImage()->SetRenderScale(FVector2D(2.0f, 2.0f));
		OutWeaponSlotWidget->GetImageAmmoImage()->SetBrushFromTexture(Row->Image); // 총알 이미지 지정 
	}

	return OutWeaponSlotWidget;
}

UWeaponSlotWidget1* UInventoryWidget::PartsUIUpdate(UDataTable* PartsDT, UWeaponSlotWidget1* OutWeaponSlotWidget, AGun_Base* OutGunBase)
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon1SlotWidget Slot1Weapon->GetWeaponParts().Num(%d)"), OutGunBase->GetWeaponParts().Num());
						
	for (int32 l = 0; l < OutGunBase->GetWeaponParts().Num(); l++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Slot1Weapon->GetWeaponParts() %d"), *OutWeaponSlotWidget->GetName(), l);
						
		if (OutGunBase->GetWeaponParts()[l].PartsName != NAME_None)
		{
			FPartsData* Row = PartsDT->FindRow<FPartsData>(OutGunBase->GetWeaponParts()[l].PartsName, TEXT("UInventoryWidget::UpdateEquippedWidget = Fail Load PartsDataTable"));
			UE_LOG(LogTemp, Warning, TEXT("Weapon1SlotWidget Slot1Weapon PartsName %s"), *OutGunBase->GetWeaponParts()[l].PartsName.ToString());

			int32 PartsIndex = static_cast<int32>(Row->PartsCategory);
			switch (l)
			{
			case 1:
				UE_LOG(LogTemp, Warning, TEXT("Weapon1SlotWidget Slot1Weapon Switch %d"), l);
				OutWeaponSlotWidget->GetWeaponPartsSlotSight()->GetImageParts()->SetVisibility(ESlateVisibility::Visible);
				OutWeaponSlotWidget->GetWeaponPartsSlotSight()->GetImageParts()->SetBrushFromTexture(Row->Image);

				OutWeaponSlotWidget->GetWeaponPartsSlotSight()->SetPartsIndex(PartsIndex);
				OutWeaponSlotWidget->GetWeaponPartsSlotSight()->SetGun(OutGunBase);
				OutWeaponSlotWidget->GetWeaponPartsSlotSight()->SetInventoryComponent(InventoryComponent);
				break;
			case 2:
				UE_LOG(LogTemp, Warning, TEXT("Weapon1SlotWidget Slot1Weapon Switch %d"), l);
				OutWeaponSlotWidget->GetWeaponPartsSlotMagazine()->GetImageParts()->SetVisibility(ESlateVisibility::Visible);
				OutWeaponSlotWidget->GetWeaponPartsSlotMagazine()->GetImageParts()->SetBrushFromTexture(Row->Image);
				UE_LOG(LogTemp, Warning, TEXT("Weapon1SlotWidget Slot1Weapon Switch %s"), *Row->Image->GetName());

				OutWeaponSlotWidget->GetWeaponPartsSlotMagazine()->SetPartsIndex(PartsIndex);
				OutWeaponSlotWidget->GetWeaponPartsSlotMagazine()->SetGun(OutGunBase);
				OutWeaponSlotWidget->GetWeaponPartsSlotMagazine()->SetInventoryComponent(InventoryComponent);
				break;
			case 3:
				UE_LOG(LogTemp, Warning, TEXT("Weapon1SlotWidget Slot1Weapon Switch %d"), l);
				OutWeaponSlotWidget->GetWeaponPartsSlotBarrel()->GetImageParts()->SetVisibility(ESlateVisibility::Visible);
				OutWeaponSlotWidget->GetWeaponPartsSlotBarrel()->GetImageParts()->SetBrushFromTexture(Row->Image);

				OutWeaponSlotWidget->GetWeaponPartsSlotBarrel()->SetPartsIndex(PartsIndex);
				OutWeaponSlotWidget->GetWeaponPartsSlotBarrel()->SetGun(OutGunBase);
				OutWeaponSlotWidget->GetWeaponPartsSlotBarrel()->SetInventoryComponent(InventoryComponent);

				break;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon1SlotWidget Slot1Weapon Parts NameNone %d"), l);

			switch (l)
			{
			case 1:
				UE_LOG(LogTemp, Warning, TEXT("Weapon1SlotWidget Slot1Weapon  Name None Switch %d"), l);
				OutWeaponSlotWidget->GetWeaponPartsSlotSight()->GetImageParts()->SetVisibility(ESlateVisibility::Collapsed);
				OutWeaponSlotWidget->GetWeaponPartsSlotSight()->GetImageParts()->SetBrushFromTexture(nullptr);
				break;
			case 2:
				UE_LOG(LogTemp, Warning, TEXT("Weapon1SlotWidget Slot1Weapon  Name None Switch %d"), l);
				OutWeaponSlotWidget->GetWeaponPartsSlotMagazine()->GetImageParts()->SetVisibility(ESlateVisibility::Collapsed);
				OutWeaponSlotWidget->GetWeaponPartsSlotMagazine()->GetImageParts()->SetBrushFromTexture(nullptr);
				break;
			case 3:
				UE_LOG(LogTemp, Warning, TEXT("Weapon1SlotWidget Slot1Weapon  Name None Switch %d"), l);
				OutWeaponSlotWidget->GetWeaponPartsSlotBarrel()->GetImageParts()->SetVisibility(ESlateVisibility::Collapsed);
				OutWeaponSlotWidget->GetWeaponPartsSlotBarrel()->GetImageParts()->SetBrushFromTexture(nullptr);
				break;
			}
		}
	}

	return OutWeaponSlotWidget;
}


