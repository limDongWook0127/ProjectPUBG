// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/WeaponItem.h"

#include "Component/Equipped/EquippedComponent.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Component/ItemData/ItemDataComponent.h"


AWeaponItem::AWeaponItem()
{
	bReplicates = true;
	//SetReplicates(true);
}

void AWeaponItem::BeginPlay()
{
	Super::BeginPlay();
}

FText AWeaponItem::LookAt()
{
	FName Name = GetItemDataComponent()->GetItemRowName();
	FString NewMessage = Name.ToString() + TEXT(" 장착");
	FText Message = FText::FromString(NewMessage);
	
	return Message;
}

void AWeaponItem::InteractWith_Implementation(APlayerCharacter* Character)
{
	//Super::InteractWith_Implementation(Character);

	if (!HasAuthority()) // 클라
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponItem : InteractWith_Implementation!!!!!!!!!!!!!"));

		UInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
		InventoryComponent->SetItem(this); // 리플리케이트가 느림
		InventoryComponent->ServerSetItem(this); // 

		UEquippedComponent* EquippedComponent = Character->GetEquippedComponent();

		if (ItemDataTable != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("WeaponItem : ItemDataTable"));

			FName ID = this->GetItemDataComponent()->GetItemRowName();
			FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ID, TEXT("Find Row"));

			int32 ItemCategory = static_cast<int32>(Row->Category);

			if (ItemCategory == 0)
			{
				EquippedComponent->ServerEquipMainItem(InventoryComponent->GetItem());
			}
			else if (ItemCategory == 2)
			{
				EquippedComponent->ServerEquipSubWeapon(InventoryComponent->GetItem());
			}
			// else if (ItemCategory == 4) // 수류탄
			// {
			// 	EquippedComponent->ServerEquiptThrow(InventoryComponent->GetItem());
			// }
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WeaponItem : ItemDataTable None"));
		}
		
	}

	
}
