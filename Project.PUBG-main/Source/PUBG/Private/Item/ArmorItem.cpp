// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ArmorItem.h"

#include "Component/Equipped/EquippedComponent.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Component/ItemData/ItemDataComponent.h"
#include "GameInstance/BaseGameInstance.h"
#include "Manager/DataTableManager.h"

AArmorItem::AArmorItem()
{
	Durability = -1;
	Defense = -1;
	Weight = -1;
}

void AArmorItem::BeginPlay()
{
	Super::BeginPlay();

	if (ItemTableRowName.IsNone())
	{
		//FString ArmorTablePath = "/Game/Datatables/Armor/DT_Armor.DT_Armor";
		//UDataTable* ArmorTable = LoadObject<UDataTable>(nullptr, TEXT("ArmorTablePath"));

		UBaseGameInstance* GI = Cast<UBaseGameInstance>(GetGameInstance());
		UDataTable* ArmorTable = GI->GetDataTableManager()->GetDataTable(EDataTableType::ArmorData);
		FArmorStruct* FoundData = ArmorTable->FindRow<FArmorStruct>(ItemTableRowName, TEXT("Fail Find Row"));

		if (FoundData) // nullptr 체크 필수
		{
			ArmorData = *FoundData; // 구조체 값을 복사해서 대입
		}

		if (Durability == -1)
		{
			Durability = ArmorData.Durability;
		}
		if (Defense == -1)
		{
			Defense = ArmorData.Defense;
		}
		if (Weight == -1)
		{
			Weight = ArmorData.Weight;
		}
	}
}

void AArmorItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AArmorItem::InteractWith_Implementation(APlayerCharacter* Character)
{
	//Super::InteractWith_Implementation(Character);

	if (!HasAuthority()) // 클라
	{
		UE_LOG(LogTemp, Warning, TEXT("AArmorItem : InteractWith_Implementation!!!!!!!!!!!!!"));

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
			
			EquippedComponent->ServerEquiptHelmet(InventoryComponent->GetItem());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("WeaponItem : ItemDataTable None"));

		}
		
	}
}