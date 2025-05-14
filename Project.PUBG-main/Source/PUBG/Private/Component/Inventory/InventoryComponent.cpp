// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Inventory/InventoryComponent.h"

#include "AudioMixerBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Component/ItemData/ItemDataComponent.h"
#include "Interface/InteractInterface.h"
#include "Item/ItemBase.h"
#include "AbilitySystem/BaseGameplayTag.h"
#include "BaseLibrary/BaseFunctionLibrary.h"
#include "Component/Equipped/EquippedComponent.h"
#include "Controller/BasePlayerController.h"
#include "Item/WeaponItem.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/Inventory/InventoryWidget.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	MaxInventoryWeight = 50.0f;
	//MaxInventoryWeight = 1000.0f; //임시값(1000)
	CurrentInventoryWeight = 0.0f;
	
	FString DataTablePath = TEXT("/Game/Datatables/ItemTable.ItemTable");
	ItemDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//Content.SetNum(1);

	// for (FItemSlotStruct& Slot : Content)
	// {
	// 	Slot.ItemName = NAME_None;
	// 	Slot.Quantity = 0;
	// 	Slot.Tag = FGameplayTag(); // 빈 태그로 초기화
	// }
	

}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UInventoryComponent, Content);
	
	DOREPLIFETIME_CONDITION(UInventoryComponent, Content, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UInventoryComponent, NearItem, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UInventoryComponent, UsingItem, COND_OwnerOnly);
	
	DOREPLIFETIME(UInventoryComponent, Item);	
	DOREPLIFETIME(UInventoryComponent, MaxInventoryWeight);
	//DOREPLIFETIME(UInventoryComponent, CurrentInventoryWeight);
	
}

void UInventoryComponent::ServerSetNearItem_Implementation(AItemBase* OutNearItem)
{
    SetNearItem(OutNearItem);
}



void UInventoryComponent::Server_InteractItem_Implementation(AItemBase* OutItemBase)
{
	//UE_LOG(LogTemp, Warning, TEXT("Server_InteractItem_Implementation!"));

	
	if (!Item && !NearItem)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Item, NearItem None"));
		return;
	}
	
	if (Item != nullptr)
	{
		SetItem(OutItemBase);
		UItemDataComponent* ItemDataComponent = Item->GetItemDataComponent();
		ItemDataComponent->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());

		AActor* Owner = GetOwner();
		if (APlayerCharacter* Character = Cast<APlayerCharacter>(Owner))
		{
			ItemDataComponent->InteractWith(Character);
		}
	}
	
	if (NearItem != nullptr)
	{
		SetNearItem(NearItem);
		//UE_LOG(LogTemp, Warning, TEXT("NearItem"));
		
		UItemDataComponent* ItemDataComponent = NearItem->GetItemDataComponent();
		//ItemDataComponent->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());

		AActor* Owner = GetOwner();
		if (APlayerCharacter* Character = Cast<APlayerCharacter>(Owner))
		{
			ItemDataComponent->InteractWith(Character);
		}
	}

	return;
}

bool UInventoryComponent::Server_InteractItem_Validate(AItemBase* OutItemBase)
{
	return true;
}

void UInventoryComponent::ServerSetContents_Implementation(const TArray<FItemSlotStruct>& OutContnets)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Execute Server : SetContents "));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Execute Client : SetContents "));
	}
	
	SetContent(OutContnets);
}

void UInventoryComponent::ServerGetItem_Implementation()
{
	GetItem();
}

int32 UInventoryComponent::AddToInventory(FName ItemID, int32 Quantity, int32 Weight, EItemCategory ItemCategory)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : AddToInventory "));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : AddToInventory "));
	}
	
	int32 RemainingQuantity = Quantity;
	
	// 인벤이 가득 찼는지 검사, 현재 무게와 최대 무게 비교 false면 인벤에 넣을 수 있음 
	bool IsFull = CurrentInventoryWeight + Weight > MaxInventoryWeight;
	
	for (int32 i = RemainingQuantity; i > 0 && !IsFull; i--, RemainingQuantity--)
	{
		IsFull = CurrentInventoryWeight + Weight >= MaxInventoryWeight;

		int32 SlotIndex = FindItemSlot(ItemID);
		if (SlotIndex == -1)
		{
			// -1면 아이템이 없는 것으로 마지막에 추가
			AddToLastIndexNewStack(ItemID, 1, Weight, ItemCategory);
		}
		else
		{
			// -1이 아니면 이미 인벤토리에 같은 아이템이 존재하므로 거기에 쌓음 꽉 찼을 경우 새로운 스택을 만들어야 함 
			AddToStack(SlotIndex, 1, Weight, ItemCategory);

			//CreateNewStack(Content.LastIndex())
		}
		
	}

	//SetContent(Content);
	//ServerSetContents_Implementation(Content);
	
	UE_LOG(LogTemp, Warning, TEXT("CurrentRemainingQuantity : %d"), RemainingQuantity);
	UE_LOG(LogTemp, Warning, TEXT("CurrentInventoryWeight : %f"), CurrentInventoryWeight);
	//OnRep_Content();
	return RemainingQuantity;
}

void UInventoryComponent::ServerAddToInventory_Implementation(FName ItemID, int32 Quantity, int32 Weight,
	EItemCategory ItemCategory)
{
	AddToInventory( ItemID,  Quantity,  Weight,  ItemCategory);

	//OnRep_Content();
}


int32 UInventoryComponent::FindItemSlot(FName ItemID)
{
	// 아이템이 이미 있는지 확인 함수

	// 해당 아이템이 인벤토리의 여러 슬롯에 존재하는 경우 마지막에 추가해야 됨 
	int32 LastIndex = -1;
	
	for (int i = 0; i < Content.Num(); i++)
	{
		if (Content[i].ItemName == ItemID)
		{
			LastIndex = i;
		}
	}

	return LastIndex;
}

void UInventoryComponent::AddToStack(int32 Index, int32 Quantity, int32 Weight, EItemCategory ItemCategory)
{
	// 현재 인덱스의 아이템 StackSize가 꽉 차면 새로운 Stack 만들기
	if (Content[Index].Quantity == GetMaxStackSize(Content[Index].ItemName))
	{
		CreateNewStack(Content[Index].ItemName, 1, Weight, Index + 1, ItemCategory);

		return;
	}
	
	FItemSlotStruct ItemSlot;
	ItemSlot.ItemName = Content[Index].ItemName;
	ItemSlot.Quantity = Content[Index].Quantity + Quantity;
	ItemSlot.ItemCategory = Content[Index].ItemCategory;
	CurrentInventoryWeight += Weight;

	Content[Index] = ItemSlot;
	
}

void UInventoryComponent::CreateNewStack(FName ItemID, int32 Quantity, int32 Weight, int32 Index, EItemCategory ItemCategory)
{
	UE_LOG(LogTemp, Warning, TEXT("CreateNewStack : %d"), Index);
	
	FGameplayTag Tag = FGameplayTag::EmptyTag;

	FItemSlotStruct ItemSlot;
	ItemSlot.ItemName = ItemID;
	ItemSlot.Quantity = Quantity;
	ItemSlot.ItemCategory = ItemCategory;

	//Content.Insert(ItemSlot, Index);
	//Content[Index] = ItemSlot;
	
	Content.Add(ItemSlot);
	CurrentInventoryWeight += Weight;
	
	UpdateInventory();
}

void UInventoryComponent::AddToLastIndexNewStack(FName ItemID, int32 Quantity, int32 Weight, EItemCategory ItemCategory)
{
	FItemSlotStruct ItemSlot;
	ItemSlot.ItemName = ItemID;
	ItemSlot.Quantity = Quantity;
	ItemSlot.ItemCategory = ItemCategory;
	CurrentInventoryWeight += Weight;
	
	Content.Add(ItemSlot);

	UpdateInventory();
}


	


int32 UInventoryComponent::GetMaxStackSize(FName ItemID)
{
	FString DataTablePath = TEXT("/Game/Datatables/ItemTable.ItemTable");
	
	UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));
	
	FItemStruct* Row = DataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));

	return Row->StackSize;
}


void UInventoryComponent::UpdateInventory()
{
		Content.Sort([]
			(
				const FItemSlotStruct& LHS,
				const FItemSlotStruct& RHS
			)

			
	{
			FString DataTablePath = TEXT("/Game/Datatables/ItemTable.ItemTable");
			UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));

			if (DataTable)
			{
				UE_LOG(LogTemp, Warning, TEXT("DataTable!!"));
			}

			
			FItemStruct* LHSRow = DataTable->FindRow<FItemStruct>(LHS.ItemName, TEXT("Get Item Row"));
			FItemStruct* RHSRow = DataTable->FindRow<FItemStruct>(RHS.ItemName, TEXT("Get Item Row"));

			UE_LOG(LogTemp, Warning, TEXT("LHS.ItemName: %s, RHS.ItemName: %s"), *LHS.ItemName.ToString(), *RHS.ItemName.ToString());
			
			// LHSRow 또는 RHSRow가 nullptr인 경우를 처리
			if (!LHSRow || !RHSRow)
		    {
			   return false;
		    }
			if (LHSRow->Category != RHSRow->Category)
			{
				return LHSRow->Category < RHSRow->Category;
			}
			if (LHS.ItemName != RHS.ItemName)
			{
				return LHS.ItemName.ToString() < RHS.ItemName.ToString();
			}
			return LHS.Quantity > RHS.Quantity;
	});
}

void UInventoryComponent::ServerUpdateInventory_Implementation()
{
	UpdateInventory();
}

void UInventoryComponent::ReplicateContent_Implementation()
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : ReplicateContent_Implementation "));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerUpdateInventory_Implementation"));
	}

	Content.Add(FItemSlotStruct());
	Content.Pop();
	

	//OnRep_Content();
	UE_LOG(LogTemp, Warning, TEXT("Content Replicate!!"));
}

void UInventoryComponent::RemoveFromInventory(int32 InIndex, bool IsConsumed, int32 InQuantity)
{
	FName ItemID = Content[InIndex].ItemName;
	UE_LOG(LogTemp, Warning, TEXT("Dongwook: ItemID : %s"), *ItemID.ToString());
	int32 Quantity = Content[InIndex].Quantity;
	UE_LOG(LogTemp, Warning, TEXT("Dongwook: Quantity : %d"), Quantity);
	if (IsConsumed)
	{
		//Content[InIndex].Quantity = Quantity - 1; , 이것들은 능력 안에서 

		// if (Content[InIndex].Quantity == 0)
		// {
		// 	Content.RemoveAt(InIndex);
		// }
		UE_LOG(LogTemp, Warning, TEXT("RemoveFromInventory : IsConsumed true"));

		FUsingItem OutUsingItem;
		OutUsingItem.Item = Content[InIndex];
		OutUsingItem.Index = InIndex;
		// 능력 활성화
		ServerSetUsingItem(OutUsingItem);
	}
	else
	{
		// 현재 양보다 많이 넣으면 안되게, 음수도 안되게
		UE_LOG(LogTemp, Warning, TEXT("RemoveFromInventory : IsConsumed false"));

		ServerDropItem(InIndex, InQuantity);		
	}
}

void UInventoryComponent::ServerSetUsingItem_Implementation(FUsingItem OutUsingItem)
{
	SetUsingItem(OutUsingItem);
}

void UInventoryComponent::ServerRemoveItem_Implementation(int32 InIndex, int32 OutQuantity)
{
	
	Content[InIndex].Quantity -= OutQuantity; //컨텐츠는 인벤토리에 들어간 먹은아이템
	UE_LOG(LogTemp, Warning, TEXT("ServerDropItem_Implementation : InIndex = %d Quantity = %d"), InIndex, Content[InIndex].Quantity);

	if (Content[InIndex].Quantity == 0)
	{
		Content.RemoveAt(InIndex);
	}
}

void UInventoryComponent::ServerDropItem_Implementation(int32 InIndex, int32 OutQuantity)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerDropItem_Implementation"));

	ServerSpawnItem(InIndex, OutQuantity);

	Content[InIndex].Quantity -= OutQuantity;
	UE_LOG(LogTemp, Warning, TEXT("ServerDropItem_Implementation : InIndex = %d Quantity = %d"), InIndex, Content[InIndex].Quantity);

	if (Content[InIndex].Quantity == 0)
	{
		Content.RemoveAt(InIndex);
	}
}

void UInventoryComponent::ServerSpawnItem_Implementation(int32 InIndex, int32 OutQuantity)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerSpawnItem_Implementation"));

	FName ItemName = Content[InIndex].ItemName;

	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemName, TEXT("Get Item Row"));

	FRotator SpawnRotation = FRotator(0, 0, 0);
	FVector SpawnLocation = UBaseFunctionLibrary::DropLocation(Cast<APlayerCharacter>(GetOwner()));
	UE_LOG(LogTemp, Warning, TEXT("SpawnLocation : %f %f %f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);

	UClass* ItemBPClass = LoadClass<AItemBase>(nullptr, TEXT("/Game/Blueprint/Item/Farming/Item.Item_C"));
	if (ItemBPClass) // nullptr 체크 추가
	{
		if (AItemBase* TempItem = GetWorld()->SpawnActorDeferred<AItemBase>(ItemBPClass, FTransform(SpawnRotation, SpawnLocation)))
		{
			TempItem->SetItemTableRowName(ItemName);
			TempItem->GetItemDataComponent()->SetItemQuantity(OutQuantity);
			TempItem->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
			UE_LOG(LogTemp, Warning, TEXT("TempItem GetItemRowName : %s"), *TempItem->GetItemDataComponent()->GetItemRowName().ToString());
		}
	}
}

void UInventoryComponent::PrintContents()
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("서버에서 실행됨"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("클라이언트에서 실행됨"));
	}
	
	//for (int32 Index = 0; Index < Content.Num(); ++Index)
	//{//
	//	FString Msg = Content[Index].ItemName.ToString();
	//	int32 Quantity = Content[Index].Quantity;
		
		//UE_LOG(LogTemp, Warning, TEXT("%d, %s, %d"), Index, *Msg, Quantity);
	//}
}

void UInventoryComponent::PrintMaxInventoryWeight()
{
	UE_LOG(LogTemp, Warning, TEXT("%f"), MaxInventoryWeight);

}

void UInventoryComponent::ServerRemoveAt_Implementation(int32 Index)
{
	Content.RemoveAt(Index);
}

void UInventoryComponent::ServerPrintMaxInventoryWeight_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("%f"), MaxInventoryWeight);
}

void UInventoryComponent::ServerPrintContents_Implementation()
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("서버에서 실행됨"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("클라이언트에서 실행됨"));
	}
	
	for (int32 Index = 0; Index < Content.Num(); ++Index)
	{
		FString Msg = Content[Index].ItemName.ToString();
		int32 Quantity = Content[Index].Quantity;
		
		UE_LOG(LogTemp, Warning, TEXT("%d, %s, %d"), Index, *Msg, Quantity);
	}
}

EItemCategory UInventoryComponent::GetEquippedItemCategory(AItemBase* InItem)
{
	FName ItemID = Item->GetItemDataComponent()->GetItemRowName();

	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));

	return Row->Category;
}

void UInventoryComponent::InteractionsByCategory(AItemBase* InItem)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	UEquippedComponent* EquippedComponent = PlayerCharacter->GetEquippedComponent();
	
	int32 ItemCategory = static_cast<int>(GetEquippedItemCategory(InItem));
	
	// 2. 카테고리에 따라 장착 
	switch (ItemCategory)
	{
	case 0: // MainWeapon
		{
			// 아이템을 무기로 변환 
			AWeaponItem* MainWeapon = Cast<AWeaponItem>(Item);
			// 1. 빈 슬롯 찾기
			int32 InIndex = EquippedComponent->FindSlotMainWeapon();
			if (InIndex != -1)
			{
				//EquippedComponent->ServerEquipItem(InIndex, MainWeapon);
				
			}
			
			break;
		}
	case 1: // SubWeapon
		{
			break;
		}
	case 2: // MeleeWeapon
		{
			break;
		}
	case 3: // Throw
		{
			break;
		}
	case 4: // Helmet
		{
			UItemDataComponent* ItemDataComponent = InItem->GetItemDataComponent();
			ItemDataComponent->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());

			AActor* Owner = GetOwner();
			if (APlayerCharacter* Character = Cast<APlayerCharacter>(Owner))
			{
				ItemDataComponent->InteractWith(Character);
			}
			break;
			break;
		}
	case 5: // Bag
		{
			break;
		}
	case 6: // Vest
		{
			break;
		}
	case 7: // Bag
		{
			break;
		}
	default:
		{
			UItemDataComponent* ItemDataComponent = InItem->GetItemDataComponent();
			ItemDataComponent->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());

			AActor* Owner = GetOwner();
			if (APlayerCharacter* Character = Cast<APlayerCharacter>(Owner))
			{
				ItemDataComponent->InteractWith(Character);
			}
			break;
		}
	}
}

void UInventoryComponent::OnRep_Content()
{
	UE_LOG(LogTemp, Warning, TEXT("Content Replicate!"));
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (PlayerCharacter)
	{
		ABasePlayerController* PlayerController = Cast<ABasePlayerController>(PlayerCharacter->GetController());
		if (PlayerController)
		{
			if (PlayerController->GetInventoryWidget())
			{
				PlayerController->GetInventoryWidget()->UpdateInventoryWidget();
				PlayerController->GetInventoryWidget()->UpdateNearItemSlotWidget();
				PlayerController->GetInventoryWidget()->UpdateEquippedWidget();
				UE_LOG(LogTemp, Warning, TEXT("OnRep_Content : Widget Update!"));
			}
		}
	}
	
}

void UInventoryComponent::OnRep_UsingItem()
{
	 // if (UsingItem.Index ==-1)
	 // {
	 // 	return;
	 // }
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (PlayerCharacter)
	{
		UBaseAbilitySystemComponent* ASC = Cast<UBaseAbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent());
		if (ASC)
		{
			ASC->TryActivateAbilityByTag(BaseGameplayTag::Player_Ability_Action_UseItem);
			ASC->TryActivateAbilityByTag(BaseGameplayTag::InputTag_Toggle_Inventory);
		}
	}
}

void UInventoryComponent::OnRep_Item()
{
	UE_LOG(LogTemp, Warning, TEXT("Item Replicate!"));

}

void UInventoryComponent::ServerSetItem_Implementation(AItemBase* OutItem)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerSetItem!!"));
	Item = OutItem;
	GetOwner()->ForceNetUpdate();
}

void UInventoryComponent::Server_Interact_Implementation()
{
	// set 
	UE_LOG(LogTemp, Warning, TEXT("InventoryComponent!"));

	if (!Item && !NearItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item, NearItem None"));
		return;
	}
	
	if (Item != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item : InteractionsByCategory"));
		InteractionsByCategory(Item);

		// UItemDataComponent* ItemDataComponent = Item->GetItemDataComponent();
		// ItemDataComponent->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());
		//
		// AActor* Owner = GetOwner();
		// if (APlayerCharacter* Character = Cast<APlayerCharacter>(Owner))
		// {
		// 	ItemDataComponent->InteractWith(Character);
		// }
		// break;
	}
	
	if (NearItem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NearItem : InteractionsByCategory"));

		InteractionsByCategory(NearItem);

		
		// UE_LOG(LogTemp, Warning, TEXT("NearItem"));
		//
		// UItemDataComponent* ItemDataComponent = NearItem->GetItemDataComponent();
		// //ItemDataComponent->GetClass()->ImplementsInterface(UInteractInterface::StaticClass());
		//
		// AActor* Owner = GetOwner();
		// if (APlayerCharacter* Character = Cast<APlayerCharacter>(Owner))
		// {
		// 	ItemDataComponent->InteractWith(Character);
		// }
	}

	return;
}

bool UInventoryComponent::Server_Interact_Validate()
{
	return true;
}




