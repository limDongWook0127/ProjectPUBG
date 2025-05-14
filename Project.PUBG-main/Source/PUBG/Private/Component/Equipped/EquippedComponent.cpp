// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Equipped/EquippedComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "BaseLibrary/DataEnum/ItemEnum.h"
#include "Component/ItemData/ItemDataComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/TextBlock.h"
#include "Controller/BasePlayerController.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Item/ArmorItem.h"
#include "Item/ItemBase.h"
#include "Item/WeaponItem.h"
#include "Item/Armor/Armor_Base.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon/DataTable/DT_PartsData.h"
#include "Widgets/HUD/HudWidget.h"
#include "Widgets/HUD/PlayerStatus/PlayerStatusWidget.h"
#include "Widgets/Inventory/InventoryWidget.h"

// Sets default values for this component's properties
UEquippedComponent::UEquippedComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	SetIsReplicatedByDefault(true);
	
	
}

void UEquippedComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION(UEquippedComponent, CurrentWeapon, COND_OwnerOnly);
	DOREPLIFETIME(UEquippedComponent, CurrentWeapon);

	DOREPLIFETIME(UEquippedComponent, LastCurrentWeapon);
	DOREPLIFETIME(UEquippedComponent, PrimarySlot);
	DOREPLIFETIME(UEquippedComponent, SecondarySlot);
	DOREPLIFETIME(UEquippedComponent, EquippedItems);
	
}


// Called when the game starts
void UEquippedComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	FString DataTablePath = TEXT("/Game/Datatables/ItemTable.ItemTable");	
	ItemDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));
	FString ArmorDataTablePath = TEXT("/Game/Datatables/Armor/DT_Armor.DT_Armor");
	ArmorDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ArmorDataTablePath));


	//EquippedMainWeapon.SetNum(2);
	EquippedItems.SetNum(16);
}


// Called every frame
void UEquippedComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquippedComponent::OnRep_EquippedItems()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_EquippedItems!!"));


	for (int i = 0; i < EquippedItems.Num() || EquippedItems.Num() == 0; i++)
	{
		if (EquippedItems[i])
		{			
			UTextureRenderTarget2D* LoadedRTSlot = nullptr;
			
			switch (i)
			{				
			case 0:
				LoadedRTSlot = Cast<UTextureRenderTarget2D>(StaticLoadObject(
				UTextureRenderTarget2D::StaticClass(), 
				nullptr, 
				TEXT("/Game/Blueprint/Widgets/Materials/RenderTarget/RT_1Slot.RT_1Slot")
				));

				break;
			case 1:
				LoadedRTSlot = Cast<UTextureRenderTarget2D>(StaticLoadObject(
				UTextureRenderTarget2D::StaticClass(), 
				nullptr, 
				TEXT("/Game/Blueprint/Widgets/Materials/RenderTarget/2Slot/RT_2Slot.RT_2Slot")
				));

				break;
			case 2:
				LoadedRTSlot = Cast<UTextureRenderTarget2D>(StaticLoadObject(
				UTextureRenderTarget2D::StaticClass(), 
				nullptr, 
				TEXT("/Game/Blueprint/Widgets/Materials/RenderTarget/3Slot/RT_3Slot.RT_3Slot")
				));		
				break;
			default:
				;
			}
			
			EquippingWeaponUpdate(EquippedItems[i], LoadedRTSlot);
		}		
	}
	
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
				UE_LOG(LogTemp, Warning, TEXT("OnRep_EquippedItems : Widget Update!"));
			}
		}
	}
}

void UEquippedComponent::OnRep_CurrentWeapon()
{
	// UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::OnRep_CurrentWeapon"));
	//
	// if (APlayerCharacter* Character = Cast<APlayerCharacter>(GetOwner()))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::OnRep_CurrentWeapon = Character"));
	// 	UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::OnRep_CurrentWeapon = %s"), *Character->GetName());
	// 	// UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::OnRep_CurrentWeapon = %s"), *Character->GetController()->GetName());
	// 	if (Character->IsLocallyControlled() == false) return;
	// 	
	// 	if (ABasePlayerController* Controller = Cast<ABasePlayerController>(Character->GetController()))
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::OnRep_CurrentWeapon = Controller"));
	//
	// 		if (UHudWidget* HudWidget = Controller->GetHudWidget())
	// 		{
	// 			UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::OnRep_CurrentWeapon = HudWidget"));
	//
	// 			if (CurrentWeapon)
	// 			{
	// 				UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::OnRep_CurrentWeapon = CurrentWeapon"));
	//
	// 				AGun_Base* CurrentGun = GetCurrentWeapon_GunBase();
	// 				HudWidget->GetPlayerStatusWidget()->SetText_CurrentAmmo(CurrentGun->GetBulletArmo());
	// 			}
	// 			else
	// 			{
	// 				HudWidget->GetPlayerStatusWidget()->GetText_CurrentAmmo()->SetVisibility(ESlateVisibility::Collapsed);
	//
	// 			}
	// 		}
	// 	}
	// 	else
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("controller not exist"));
	// 	}
	// }
	
}

EItemCategory UEquippedComponent::GetEquippedItemCategory(AItemBase* Item)
{
	FName ItemID = Item->GetItemDataComponent()->GetItemRowName();

	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));

	return Row->Category;
}




int32 UEquippedComponent::FindSlotMainWeapon()
{
	for (int i = 0; i < 2; i++)
	{
		// true면 빈 슬롯이 있는 것  
		if (EquippedItems[i] == nullptr)
		{
			return i;
		}
	}

	// 빈 슬롯이 없는 것 
	return -1;
}

void UEquippedComponent::ServerEquipMainItem_Implementation(AItemBase* Item)
{
	AWeaponItem* WeaponItem = Cast<AWeaponItem>(Item);

	FName ItemID = WeaponItem->GetItemDataComponent()->GetItemRowName();
	UE_LOG(LogTemp, Warning, TEXT("ServerEquipMainItem_Implementation : ItemId %s"), *ItemID.ToString());

	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));

	int32 MainWeaponSlot = static_cast<int32>(WeaponItem->GetEquippedItemCategory());
	
	UClass* GunClass = Cast<UClass>(Row->BP_Item.LoadSynchronous());
	
	
	if (AGun_Base* MainWeapon = GetWorld()->SpawnActorDeferred<AGun_Base>(GunClass, FTransform(FRotator(0), FVector(0))))
	{
		//TempWeapon->SetTableIndex(RowIndex);
		FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
		
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
		

		int32 RemainSlot = FindSlotMainWeapon();
		UE_LOG(LogTemp, Warning, TEXT("RemainSlot : %d"), RemainSlot);

		EEquippedItemCategory EquippedItemCategory = static_cast<EEquippedItemCategory>(MainWeaponSlot);
		MainWeapon->SetEquipSlot(EquippedItemCategory);
		MainWeaponSlot = static_cast<int32>(MainWeapon->GetEquipSlot()); // 장착될 슬롯값 가져오기
	
		if (RemainSlot == -1) // Main 무기 슬롯이 가득찬 것 
		{
			int32 DropMainWeaponSlot = DropMainWeapon();
			RemainSlot = DropMainWeaponSlot;
			UE_LOG(LogTemp, Warning, TEXT("DropMainWeapon RemainSlot : %d"), RemainSlot);
		}
	
		if (RemainSlot == 0) // 1번 슬롯이 비어있는 것 
		{
			// 필요한 값 넣기 MainWeapon -> Weapon
			
			EquippedItems[MainWeaponSlot] = MainWeapon;
			EquippedItems[MainWeaponSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_primarySocket")));
			UE_LOG(LogTemp, Warning, TEXT(" Test Debug!!!!!!!!!!!!!!!!!!!!!!!!!!!!"))

		}
		else if (RemainSlot == 1) // 2번 슬롯이 비어있는 것 
		{
			MainWeapon->SetEquipSlot(EEquippedItemCategory::SecondarySlot);
			MainWeaponSlot = static_cast<int32>(MainWeapon->GetEquipSlot());
			
			EquippedItems[MainWeaponSlot] = MainWeapon;
			EquippedItems[MainWeaponSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_secondarySocket")));
		}
		
		MainWeapon->SetOwner(PlayerCharacter);
		MainWeapon->FinishSpawning(FTransform(FRotator(0), FVector(0)));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerEquipMainItem_Implementation : MainWeapon Casting Fail"));
	}
	// GetOwner()->SetNetDormancy(DORM_Awake);
	GetOwner()->ForceNetUpdate();
	Item->Destroy(true);
	
	// AGun_Base* MainWeapon = Cast<AGun_Base>(GetWorld()->SpawnActor<AEquipableItem>(Row->BP_Item));
	// FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
	//
	// APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	//
	// int32 RemainSlot = FindSlotMainWeapon();
	//
	// int32 MainWeaponSlot = static_cast<int32>(MainWeapon->GetEquipSlot()); // 장착될 슬롯값 가져오기
	//
	// if (RemainSlot == -1) // Main 무기 슬롯이 가득찬 것 
	// {
	// 	int32 DropMainWeaponSlot = DropMainWeapon();
	// 	RemainSlot = DropMainWeaponSlot;
	// }
	//
	// if (RemainSlot == 0) // 1번 슬롯이 비어있는 것 
	// {
	// 	EquippedItems[MainWeaponSlot] = MainWeapon;
	// 	EquippedItems[MainWeaponSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_primarySocket")));
	//
	// 	// 필요한 값 넣기 MainWeapon -> Weapon
	// }
	// else if (RemainSlot == 1) // 2번 슬롯이 비어있는 것 
	// {
	// 	MainWeapon->SetEquipSlot(EEquippedItemCategory::SecondarySlot);
	// 	MainWeaponSlot = static_cast<int32>(MainWeapon->GetEquipSlot());
	// 	
	// 	
	// 	EquippedItems[MainWeaponSlot] = MainWeapon;
	// 	EquippedItems[MainWeaponSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("slot_secondarySocket")));
	// }

}

int32 UEquippedComponent::DropMainWeapon(AGun_Base* OutCurrentWeapon, int32 OutIndex)
{
	// 버리는 것은 무조건 UI에서만, 지정해서
	// 교체할 때 쓰기 위해서는 버릴 아이템을 매개변수로 
	
	if (OutCurrentWeapon == nullptr) // 매개변수가 없다는 것은 교체할 때 쓰는 것 CurrentWeapon과 2번 슬롯의 무기 
	{
		
		if (CurrentWeapon != nullptr && (CurrentWeapon->GetEquipSlot() == EEquippedItemCategory::PrimarySlot || CurrentWeapon->GetEquipSlot() == EEquippedItemCategory::SecondarySlot)) // 현재 손에 든 무기가 있으면 현재 무기와 교체
		{
			ServerSpawnStaticMeshFromMainWeapon(Cast<AGun_Base>(CurrentWeapon));
			
			int32 DropIndex = static_cast<int32>(CurrentWeapon->GetEquipSlot());
			UE_LOG(LogTemp, Warning, TEXT("Drop Index : %d"), DropIndex);

			CurrentWeapon->Destroy();
			// 능력과 레이어 뺴는 곳 
			CurrentWeapon = nullptr;
			return DropIndex;
		}
		else // 현재 손에 든 무기가 없으면 2번 슬롯 무기와 교체
		{
			UE_LOG(LogTemp, Warning, TEXT("Slot : 2"));
			AGun_Base* MainWeapon = Cast<AGun_Base>(EquippedItems[1]);
			ServerSpawnStaticMeshFromMainWeapon(MainWeapon);
			
			FName ItemID = FName(Cast<AGun_Base>(EquippedItems[1])->GetWeaponDataAsset().GunName); // RowName 가져오기
			int32 RowIndex = GetRowIndex(ItemDataTable, ItemID); // RowName에 해당하는 인덱스 가져오기
			UE_LOG(LogTemp, Warning, TEXT("Row Index : %d"), RowIndex);
			
			int32 DropIndex = static_cast<int32>(EquippedItems[1]->GetEquipSlot());
			UE_LOG(LogTemp, Warning, TEXT("Drop Index : %d"), DropIndex);
			EquippedItems[1]->Destroy();
			EquippedItems[1] = nullptr;
			return DropIndex;

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OutCurrentWeaponSlot not Null"));
		UE_LOG(LogTemp, Warning, TEXT("OutCurrentWeaponSlot %d"), OutIndex);

		ServerSpawnStaticMeshFromMainWeapon(Cast<AGun_Base>(EquippedItems[OutIndex]));

		int32 DropIndex = static_cast<int32>(OutCurrentWeapon->GetEquipSlot());

		EquippedItems[OutIndex]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquippedItems[OutIndex]->Destroy(true);
		EquippedItems[OutIndex] = nullptr;
		OutCurrentWeapon = nullptr;
		OnRep_EquippedItems();
		return DropIndex;
	}
}

void UEquippedComponent::ServerDropMainWeapon_Implementation(AGun_Base* OutCurrentWeapon, int32 OutIndex)
{
	DropMainWeapon(OutCurrentWeapon, OutIndex);
}

void UEquippedComponent::ServerSpawnStaticMeshFromMainWeapon_Implementation(AGun_Base* OutCurrentWeapon)
{
	FName ItemID = FName(OutCurrentWeapon->GetWeaponDataAsset().GunName); // RowName 가져오기
	
	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row")); // 테이블 가져오기
	int32 RowIndex = GetRowIndex(ItemDataTable, ItemID); // RowName에 해당하는 인덱스 가져오기 
	UE_LOG(LogTemp, Warning, TEXT("Row Index : %d"), RowIndex);

			
	FRotator SpawnRotation = FRotator(0, 0, 0);
	FVector SpawnLocation = DropLocation();
	UE_LOG(LogTemp, Warning, TEXT("SpawnLocation : %f %f %f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);

	UClass* MainWeaponItemBPClass = LoadClass<AWeaponItem>(nullptr, TEXT("/Game/Blueprint/Item/Farming/BP_WeaponItem.BP_WeaponItem_C"));
	if (MainWeaponItemBPClass) // nullptr 체크 추가
	{
		if (AWeaponItem* TempWeapon = GetWorld()->SpawnActorDeferred<AWeaponItem>(MainWeaponItemBPClass, FTransform(SpawnRotation, SpawnLocation)))
		{
			//TempWeapon->SetTableIndex(RowIndex);
			TempWeapon->SetItemTableRowName(ItemID);
			UE_LOG(LogTemp, Warning, TEXT("TempWeapon ItemID : %s"), *ItemID.ToString());
			//TempWeapon->SetTableIndex(-1); // 무기 버릴 때 자기 데이터를 넣어줘야 함

			if (TempWeapon->GetItemDataComponent())
			{
				UE_LOG(LogTemp, Warning, TEXT("ServerSpawnStaticMeshFromMainWeapon_Implementation = GetItemDataComponent"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ServerSpawnStaticMeshFromMainWeapon_Implementation = GetItemDataComponent None"));

			}
			TempWeapon->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
			//UE_LOG(LogTemp, Warning, TEXT("TempWeapon GetItemRowName : %s"), *TempWeapon->GetItemDataComponent()->GetItemRowName().ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load blueprint class! Check the path: /Game/Blueprint/Item/Farming/TestWeaponItem.TestWeaponItem_C"));
	}
}


void UEquippedComponent::ServerEquipSubWeapon_Implementation(AItemBase* Item)
{
	AWeaponItem* SubWeaponItem = Cast<AWeaponItem>(Item);

	FName ItemID = SubWeaponItem->GetItemDataComponent()->GetItemRowName();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemID.ToString());
	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));
	
	int32 SubWeaponSlot = static_cast<int32>(SubWeaponItem->GetEquippedItemCategory()); // Item에 저장되어 있는 슬롯 값 가져오기 

	if (EquippedItems[2] != nullptr)
	{
		DropSUbWeapon();
	}

	UClass* GunClass = Cast<UClass>(Row->BP_Item.LoadSynchronous());
	if (AGun_Base* SubWeapon = GetWorld()->SpawnActorDeferred<AGun_Base>(GunClass, FTransform(FRotator(0), FVector(0))))
	{
		//TempWeapon->SetTableIndex(RowIndex);
		FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
		
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

		EEquippedItemCategory EquippedItemCategory = static_cast<EEquippedItemCategory>(SubWeaponSlot);
		SubWeapon->SetEquipSlot(EquippedItemCategory);
		
		SubWeaponSlot = static_cast<int32>(SubWeapon->GetEquipSlot()); // 장착될 슬롯값 가져오기
		UE_LOG(LogTemp, Warning, TEXT("SubWeapon Slot : %d"), SubWeaponSlot);

		EquippedItems[SubWeaponSlot] = SubWeapon;
		EquippedItems[SubWeaponSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("SideArm")));

		SubWeapon->SetOwner(PlayerCharacter);
		SubWeapon->FinishSpawning(FTransform(FRotator(0), FVector(0)));
	}

	GetOwner()->ForceNetUpdate();
	Item->Destroy(true);
}

void UEquippedComponent::DropSUbWeapon(AGun_Base* OutCurrentWeapon)
{
	if (OutCurrentWeapon != nullptr) // UI에서 버리는 것
	{
		// 권총은 하나이므로 권총 슬롯과 바로 비교 
		ServerSpawnStaticMeshFromSubWeapon(Cast<AGun_Base>(OutCurrentWeapon));
		OutCurrentWeapon->Destroy(true);
		
		EquippedItems[2]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquippedItems[2]->Destroy(true);
		EquippedItems[2] = nullptr;
		OutCurrentWeapon = nullptr;
		OnRep_EquippedItems();

	}
	else // 교체되는 것 
	{
		if (CurrentWeapon != nullptr && CurrentWeapon->GetEquipSlot() == EEquippedItemCategory::SubWeapon) // 현재 손에 권총이 있으면 
		{
			ServerSpawnStaticMeshFromSubWeapon(Cast<AGun_Base>(CurrentWeapon));
			CurrentWeapon->Destroy(true);
			// 능력과 레이어 뺴는 곳 
			CurrentWeapon = nullptr;
			EquippedItems[2]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			EquippedItems[2]->Destroy(true);
			EquippedItems[2] = nullptr;
			OnRep_EquippedItems();
		}
		else if (EquippedItems[2] != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("EquippedItems[2]"));
			ServerSpawnStaticMeshFromSubWeapon(Cast<AGun_Base>(EquippedItems[2]));

			EquippedItems[2]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			EquippedItems[2]->Destroy(true);
			EquippedItems[2] = nullptr;
			OutCurrentWeapon = nullptr;
			OnRep_EquippedItems();
		}
	}
}

void UEquippedComponent::ServerDropSUbWeapon_Implementation(AGun_Base* OutCurrentWeapon)
{
	DropSUbWeapon(OutCurrentWeapon);
}

void UEquippedComponent::ServerSpawnStaticMeshFromSubWeapon_Implementation(AGun_Base* OutCurrentWeapon)
{
	FName ItemID = FName(OutCurrentWeapon->GetWeaponDataAsset().GunName); // RowName 가져오기
	UE_LOG(LogTemp, Warning, TEXT("ITemID : %s"), *ItemID.ToString());
	
	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row")); // 테이블 가져오기
	int32 RowIndex = GetRowIndex(ItemDataTable, ItemID); // RowName에 해당하는 인덱스 가져오기 
	UE_LOG(LogTemp, Warning, TEXT("Row Index : %d"), RowIndex);

			
	FRotator SpawnRotation = FRotator(0, 0, 0);
	FVector SpawnLocation = DropLocation();
	UE_LOG(LogTemp, Warning, TEXT("SpawnLocation : %f %f %f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);

	UClass* MainWeaponItemBPClass = LoadClass<AWeaponItem>(nullptr, TEXT("/Game/Blueprint/Item/Farming/BP_WeaponItem.BP_WeaponItem_C"));
	if (MainWeaponItemBPClass) // nullptr 체크 추가
	{
		if (AWeaponItem* TempWeapon = GetWorld()->SpawnActorDeferred<AWeaponItem>(MainWeaponItemBPClass, FTransform(SpawnRotation, SpawnLocation)))
		{
			//TempWeapon->SetTableIndex(RowIndex);
			//TempWeapon->SetTableIndex(-1); // 무기 버릴 때 자기 데이터를 넣어줘야 함
			TempWeapon->SetItemTableRowName(ItemID);
			TempWeapon->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
			UE_LOG(LogTemp, Warning, TEXT("TempWeapon GetItemRowName : %s"), *TempWeapon->GetItemDataComponent()->GetItemRowName().ToString());

		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load blueprint class! Check the path: /Game/Blueprint/Item/Farming/TestWeaponItem.TestWeaponItem_C"));
	}
}


void UEquippedComponent::ServerEquiptHelmet_Implementation(AItemBase* Item)
{
	AArmorItem* HelmetItem = Cast<AArmorItem>(Item);

	FName ItemID = HelmetItem->GetItemDataComponent()->GetItemRowName();
	UE_LOG(LogTemp, Warning, TEXT("ServerEquiptHelmet_Implementation : %s"), *ItemID.ToString());
	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));
	FArmorStruct* ArmorRow = ArmorDataTable->FindRow<FArmorStruct>(ItemID, TEXT("Find Armor"));
	
	int32 HelmetSlot = static_cast<int32>(HelmetItem->GetEquippedItemCategory()); // Item에 저장되어 있는 슬롯 값 가져오기 

	if (EquippedItems[HelmetSlot] != nullptr)
	{
		DropArmor(HelmetSlot);
	}

	UClass* ArmorClass = Cast<UClass>(Row->BP_Item.LoadSynchronous());
	if (AArmor_Base* Armor = GetWorld()->SpawnActorDeferred<AArmor_Base>(ArmorClass, FTransform(FRotator(0), FVector(0))))
	{
		//TempWeapon->SetTableIndex(RowIndex);
		FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
		
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

		EEquippedItemCategory EquippedItemCategory = static_cast<EEquippedItemCategory>(HelmetSlot);
		Armor->SetEquipSlot(EquippedItemCategory);
		Armor->SetName(ItemID);
		//Armor->SetMesh(Row->StaticMesh);
		//Armor->GetSkeletalMeshComponent()->SetSkeletalMesh(ArmorRow->SkeletalMesh);
		UE_LOG(LogTemp, Warning, TEXT("Armor SpawnActorDeferred : %s"), *Row->StaticMesh->GetName());
		
		HelmetSlot = static_cast<int32>(Armor->GetEquipSlot()); // 장착될 슬롯값 가져오기
		UE_LOG(LogTemp, Warning, TEXT("Helmet Slot : %d"), HelmetSlot);

		EquippedItems[HelmetSlot] = Armor;
		if (HelmetSlot == 5)
		{
			EquippedItems[HelmetSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("HeadSocket")));
			//PlayerCharacter->SetMeshComponent(EPlayerMeshType::Head, ArmorRow->SkeletalMesh);
			PlayerCharacter->Multicast_SetMeshComponent(EPlayerMeshType::Head, ArmorRow->SkeletalMesh);
		}
		else if (HelmetSlot == 6)
		{
			UE_LOG(LogTemp, Warning, TEXT("BagSlot == 6"));
			EquippedItems[HelmetSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("BagSocket")));

			UE_LOG(LogTemp, Warning, TEXT("BagSlot = %s "), *ArmorRow->SkeletalMesh->GetName());
			PlayerCharacter->Multicast_SetMeshComponent(EPlayerMeshType::Bottom, ArmorRow->SkeletalMesh);

		}
		else if (HelmetSlot == 7)
		{
			EquippedItems[HelmetSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("ArmorSocket")));
			//PlayerCharacter->SetMeshComponent(EPlayerMeshType::Top, ArmorRow->SkeletalMesh);
			PlayerCharacter->Multicast_SetMeshComponent(EPlayerMeshType::Top, ArmorRow->SkeletalMesh);

		}

		Armor->SetOwner(PlayerCharacter);
		Armor->FinishSpawning(FTransform(FRotator(0), FVector(0)));
		Armor->EquipArmor(PlayerCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Armor SpawnActorDeferred Failed"));
	}

	Item->Destroy(true);
	GetOwner()->ForceNetUpdate();
}

void UEquippedComponent::DropArmor(int32 OutIndex)
{
	if (EquippedItems[OutIndex] != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::DropArmor = EquippedItems[%d]"), OutIndex);
		ServerSpawnStaticMeshFromArmor(Cast<AArmor_Base>(EquippedItems[OutIndex]));
		
		if (AArmor_Base* Armor = Cast<AArmor_Base>(EquippedItems[OutIndex]))
		{
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

			if (OutIndex == 5)
			{
				PlayerCharacter->Multicast_SetMeshComponent(EPlayerMeshType::Head, nullptr);
			}
			else if (OutIndex == 6)
			{
				UE_LOG(LogTemp, Warning, TEXT("HelmetSlot == 6"));
				PlayerCharacter->Multicast_SetMeshComponent(EPlayerMeshType::Bottom, nullptr);
			}
			else if (OutIndex == 7)
			{
				PlayerCharacter->Multicast_SetMeshComponent(EPlayerMeshType::Top, nullptr);
			}
			
			Armor->UnEquipArmor(PlayerCharacter);
		}

		EquippedItems[OutIndex]->Destroy(true);
		EquippedItems[OutIndex] = nullptr;

		if (EquippedItems[OutIndex] == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("EquippedItems[%d] = NULL"), OutIndex);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("EquippedItems[%d] = %s"), OutIndex, *EquippedItems[OutIndex]->GetActorNameOrLabel());

		}
		OnRep_EquippedItems();
	}
}

void UEquippedComponent::ServerDropArmor_Implementation(int32 OutIndex)
{
	DropArmor(OutIndex);
}

void UEquippedComponent::ServerSpawnStaticMeshFromArmor_Implementation(AArmor_Base* OutCurrentArmor)
{
	FName ItemID = FName(OutCurrentArmor->GetArmorData().Name); // RowName 가져오기
	UE_LOG(LogTemp, Warning, TEXT("ItemID : %s"), *ItemID.ToString());
	
	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row")); // 테이블 가져오기
	int32 RowIndex = GetRowIndex(ItemDataTable, ItemID); // RowName에 해당하는 인덱스 가져오기 
	UE_LOG(LogTemp, Warning, TEXT("Row Index : %d"), RowIndex);

			
	FRotator SpawnRotation = FRotator(0, 0, 0);
	FVector SpawnLocation = DropLocation();
	UE_LOG(LogTemp, Warning, TEXT("SpawnLocation : %f %f %f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);

	UClass* ArmorItemBPClass = LoadClass<AArmorItem>(nullptr, TEXT("/Game/Blueprint/Item/Farming/BP_ArmorItem.BP_ArmorItem_C"));
	if (ArmorItemBPClass) // nullptr 체크 추가
	{
		if (AArmorItem* TempArmor = GetWorld()->SpawnActorDeferred<AArmorItem>(ArmorItemBPClass, FTransform(SpawnRotation, SpawnLocation)))
		{
			//TempWeapon->SetTableIndex(RowIndex);
			//TempWeapon->SetTableIndex(-1); // 무기 버릴 때 자기 데이터를 넣어줘야 함
			
			TempArmor->SetItemTableRowName(ItemID);
			TempArmor->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
			UE_LOG(LogTemp, Warning, TEXT("TempWeapon GetItemRowName : %s"), *TempArmor->GetItemDataComponent()->GetItemRowName().ToString());

		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load blueprint class! Check the path: /Game/Blueprint/Item/Farming/TestWeaponItem.TestWeaponItem_C"));
	}
}

// void UEquippedComponent::ServerEquipThrow_Implementation(AItemBase* Item, FItemSlotStruct* ItemSlot)
// {
// 	if (Item != nullptr)
// 	{
// 		AWeaponItem* GrenadeItem = Cast<AWeaponItem>(Item);
//
// 		FName ItemID = GrenadeItem->GetItemDataComponent()->GetItemRowName();
// 		UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemID.ToString());
// 		FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));
// 	
// 		int32 GrenadeSlot = static_cast<int32>(GrenadeItem->GetEquippedItemCategory()); // Item에 저장되어 있는 슬롯 값 가져오기
// 		
// 		if (EquippedItems[4] != nullptr) 
// 		{
// 			// 이미 있으면 인벤토리에 추가
// 			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
// 			UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
//
// 			FName Name = Item->GetItemDataComponent()->GetItemRowName();
// 			float Weight = Item->GetItemDataComponent()->GetWeight();
// 			EItemCategory ItemCategory = Item->GetItemDataComponent()->GetItemCategory();
// 		
// 			InventoryComponent->AddToInventory(Name, 1, Weight, ItemCategory);
// 		}
//
// 		TSubclassOf<AGrenade_Base> GrenadeClass = Cast<UClass>(Row->BP_Item.Get());
// 		if (AGrenade_Base* Grenade = GetWorld()->SpawnActorDeferred<AGrenade_Base>(GrenadeClass, FTransform(FRotator(0), FVector(0))))
// 		{
// 			//TempWeapon->SetTableIndex(RowIndex);
// 			FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
// 			
// 			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
//
// 			EEquippedItemCategory EquippedItemCategory = static_cast<EEquippedItemCategory>(GrenadeSlot);
// 			Grenade->SetEquipSlot(EquippedItemCategory);
// 			
// 			GrenadeSlot = static_cast<int32>(Grenade->GetEquipSlot()); // 장착될 슬롯값 가져오기
// 			UE_LOG(LogTemp, Warning, TEXT("Grenade Slot : %d"), GrenadeSlot);
//
// 			EquippedItems[GrenadeSlot] = Grenade;
// 			EquippedItems[GrenadeSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("throwable_Socket")));
// 			
// 			Grenade->FinishSpawning(FTransform(FRotator(0), FVector(0)));
//
// 			GetOwner()->ForceNetUpdate();
// 			//Item->Destroy(true);
// 		}
// 	}
// 	else if (ItemSlot != nullptr)
// 	{
// 		FName Name = ItemSlot->ItemName;
// 		FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(Name, TEXT("Find Row"));
//
// 		int32 GrenadeSlot = static_cast<int32>(Row->Category); // Item에 저장되어 있는 슬롯 값 가져오기
// 		
// 		if (EquippedItems[4] != nullptr) 
// 		{
// 			// 이미 있으면 인벤토리에 추가
// 			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
// 			UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
//
// 			FName Name = ItemSlot->ItemName;
// 			float Weight = Row->Weight;
// 			EItemCategory ItemCategory = ItemSlot->ItemCategory;
// 			
// 			InventoryComponent->AddToInventory(Name, 1, Weight, ItemCategory);
// 		}
//
// 		TSubclassOf<AGrenade_Base> GrenadeClass = Cast<UClass>(Row->BP_Item.Get());
// 		if (AGrenade_Base* Grenade = GetWorld()->SpawnActorDeferred<AGrenade_Base>(GrenadeClass, FTransform(FRotator(0), FVector(0))))
// 		{
// 			//TempWeapon->SetTableIndex(RowIndex);
// 			FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
// 			
// 			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
//
// 			EEquippedItemCategory EquippedItemCategory = static_cast<EEquippedItemCategory>(GrenadeSlot);
// 			Grenade->SetEquipSlot(EquippedItemCategory);
// 			
// 			GrenadeSlot = static_cast<int32>(Grenade->GetEquipSlot()); // 장착될 슬롯값 가져오기
// 			UE_LOG(LogTemp, Warning, TEXT("Grenade Slot : %d"), GrenadeSlot);
//
// 			EquippedItems[GrenadeSlot] = Grenade;
// 			EquippedItems[GrenadeSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("throwable_Socket")));
// 			
// 			Grenade->FinishSpawning(FTransform(FRotator(0), FVector(0)));
//
// 			GetOwner()->ForceNetUpdate();
// 			//Item->Destroy(true);
// 		}
// 	}
// }


void UEquippedComponent::EquipThrow(AItemBase* Item, FItemSlotStruct* ItemSlot)
{
	if (Item != nullptr)
	{
		AWeaponItem* GrenadeItem = Cast<AWeaponItem>(Item);

		FName ItemID = GrenadeItem->GetItemDataComponent()->GetItemRowName();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemID.ToString());
		FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));
	
		int32 GrenadeSlot = static_cast<int32>(GrenadeItem->GetEquippedItemCategory()); // Item에 저장되어 있는 슬롯 값 가져오기
		
		if (EquippedItems[4] != nullptr) 
		{
			// 이미 있으면 인벤토리에 추가
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
			UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();

			FName Name = Item->GetItemDataComponent()->GetItemRowName();
			float Weight = Item->GetItemDataComponent()->GetWeight();
			EItemCategory ItemCategory = Item->GetItemDataComponent()->GetItemCategory();
		
			InventoryComponent->AddToInventory(Name, 1, Weight, ItemCategory);
		}

		UClass* GrenadeClass = Cast<UClass>(Row->BP_Item.LoadSynchronous());
		
		if (AGrenade_Base* Grenade = GetWorld()->SpawnActorDeferred<AGrenade_Base>(GrenadeClass, FTransform(FRotator(0), FVector(0))))
		{
			//TempWeapon->SetTableIndex(RowIndex);
			FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
			
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
			
			EEquippedItemCategory EquippedItemCategory = static_cast<EEquippedItemCategory>(GrenadeSlot);
			Grenade->SetEquipSlot(EquippedItemCategory);
			
			GrenadeSlot = static_cast<int32>(Grenade->GetEquipSlot()); // 장착될 슬롯값 가져오기
			UE_LOG(LogTemp, Warning, TEXT("Grenade Slot : %d"), GrenadeSlot);

			EquippedItems[GrenadeSlot] = Grenade;
			EquippedItems[GrenadeSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("throwable_Socket")));

			Grenade->SetOwner(PlayerCharacter);
			Grenade->FinishSpawning(FTransform(FRotator(0), FVector(0)));

			GetOwner()->ForceNetUpdate();
			//Item->Destroy(true);
		}
	}
	else if (ItemSlot != nullptr)
	{
		FName Name = ItemSlot->ItemName;
		UE_LOG(LogTemp, Warning, TEXT("ItemSlot != nullptr = %s"), *Name.ToString())
		FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(Name, TEXT("Find Row"));

		int32 GrenadeSlot = static_cast<int32>(Row->Category); // Item에 저장되어 있는 슬롯 값 가져오기
		
		if (EquippedItems[4] != nullptr) 
		{
			// 이미 있으면 인벤토리에 추가
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
			UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
			
			float Weight = Row->Weight;
			EItemCategory ItemCategory = ItemSlot->ItemCategory;
			
			InventoryComponent->AddToInventory(Name, 1, Weight, ItemCategory);
		}

		TSubclassOf<AGrenade_Base> GrenadeClass = Cast<UClass>(Row->BP_Item.Get());
		if (AGrenade_Base* Grenade = GetWorld()->SpawnActorDeferred<AGrenade_Base>(GrenadeClass, FTransform(FRotator(0), FVector(0))))
		{
			//TempWeapon->SetTableIndex(RowIndex);
			FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
			
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
			Grenade->OwningPlayerCharacter = PlayerCharacter;

			EEquippedItemCategory EquippedItemCategory = static_cast<EEquippedItemCategory>(GrenadeSlot);
			Grenade->SetEquipSlot(EquippedItemCategory);
			
			GrenadeSlot = static_cast<int32>(Grenade->GetEquipSlot()); // 장착될 슬롯값 가져오기
			UE_LOG(LogTemp, Warning, TEXT("Grenade Slot : %d"), GrenadeSlot);

			EquippedItems[GrenadeSlot] = Grenade;
			EquippedItems[GrenadeSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("throwable_Socket")));
			
			Grenade->FinishSpawning(FTransform(FRotator(0), FVector(0)));

			GetOwner()->ForceNetUpdate();
			//Item->Destroy(true);
		}
	}
}

void UEquippedComponent::ServerEquipThrow_Implementation(AItemBase* Item, FItemSlotStruct ItemSlot)
{
	if (Item != nullptr)
	{
		AWeaponItem* GrenadeItem = Cast<AWeaponItem>(Item);

		FName ItemID = GrenadeItem->GetItemDataComponent()->GetItemRowName();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemID.ToString());
		FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ItemID, TEXT("Find Row"));
	
		int32 GrenadeSlot = static_cast<int32>(GrenadeItem->GetEquippedItemCategory()); // Item에 저장되어 있는 슬롯 값 가져오기
		
		if (EquippedItems[4] != nullptr) 
		{
			// 이미 있으면 인벤토리에 추가
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
			UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();

			FName Name = Item->GetItemDataComponent()->GetItemRowName();
			float Weight = Item->GetItemDataComponent()->GetWeight();
			EItemCategory ItemCategory = Item->GetItemDataComponent()->GetItemCategory();
		
			InventoryComponent->AddToInventory(Name, 1, Weight, ItemCategory);
		}

		TSubclassOf<AGrenade_Base> GrenadeClass = Cast<UClass>(Row->BP_Item.Get());
		if (AGrenade_Base* Grenade = GetWorld()->SpawnActorDeferred<AGrenade_Base>(GrenadeClass, FTransform(FRotator(0), FVector(0))))
		{
			//TempWeapon->SetTableIndex(RowIndex);
			FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
			
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

			EEquippedItemCategory EquippedItemCategory = static_cast<EEquippedItemCategory>(GrenadeSlot);
			Grenade->SetEquipSlot(EquippedItemCategory);
			
			GrenadeSlot = static_cast<int32>(Grenade->GetEquipSlot()); // 장착될 슬롯값 가져오기
			UE_LOG(LogTemp, Warning, TEXT("Grenade Slot : %d"), GrenadeSlot);

			EquippedItems[GrenadeSlot] = Grenade;
			EquippedItems[GrenadeSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("throwable_Socket")));
			
			Grenade->FinishSpawning(FTransform(FRotator(0), FVector(0)));

			Grenade->SetOwner(PlayerCharacter);
			GetOwner()->ForceNetUpdate();
			//Item->Destroy(true);
		}
	}
	else if (ItemSlot.ItemName != NAME_None)
	{
		FName Name = ItemSlot.ItemName;
		UE_LOG(LogTemp, Warning, TEXT("ItemSlot != nullptr = %s"), *Name.ToString())
		FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(Name, TEXT("Find Row"));

		int32 GrenadeSlot = static_cast<int32>(Row->Category); // Item에 저장되어 있는 슬롯 값 가져오기
		
		if (EquippedItems[4] != nullptr) 
		{
			// 이미 있으면 인벤토리에 추가
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
			UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
			
			float Weight = Row->Weight;
			EItemCategory ItemCategory = ItemSlot.ItemCategory;
			
			InventoryComponent->AddToInventory(Name, 1, Weight, ItemCategory);
		}

		TSubclassOf<AGrenade_Base> GrenadeClass = Cast<UClass>(Row->BP_Item.Get());
		if (AGrenade_Base* Grenade = GetWorld()->SpawnActorDeferred<AGrenade_Base>(GrenadeClass, FTransform(FRotator(0), FVector(0))))
		{
			//TempWeapon->SetTableIndex(RowIndex);
			FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, true);
			
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
			Grenade->OwningPlayerCharacter = PlayerCharacter;

			if (PlayerCharacter != nullptr)
			{
				Grenade->OwningPlayerCharacter = PlayerCharacter;
			}
			EEquippedItemCategory EquippedItemCategory = static_cast<EEquippedItemCategory>(GrenadeSlot);
			Grenade->SetEquipSlot(EquippedItemCategory);
			
			GrenadeSlot = static_cast<int32>(Grenade->GetEquipSlot()); // 장착될 슬롯값 가져오기
			UE_LOG(LogTemp, Warning, TEXT("Grenade Slot : %d"), GrenadeSlot);

			EquippedItems[GrenadeSlot] = Grenade;
			EquippedItems[GrenadeSlot]->AttachToComponent(PlayerCharacter->GetMesh(), Rule, FName(TEXT("throwable_Socket")));
			
			Grenade->FinishSpawning(FTransform(FRotator(0), FVector(0)));

			Grenade->SetOwner(PlayerCharacter);
			GetOwner()->ForceNetUpdate();
			//Item->Destroy(true);
		}
	}
}

bool UEquippedComponent::EquipParts(AItemBase* PartsItem, int32 Index, FItemSlotStruct* ItemSlot)
{
	if (PartsItem != nullptr) // F로 장착
	{
		UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::ServerEquipParts PartsItem"));
	}
	else if (Index != -1) // UI로 장착 
	{
		UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::ServerEquipParts Index"));

		// FItemSlot이 들어오면 이름으로 데이터 테이블 접근 후
	}
	else if (ItemSlot != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::ServerEquipParts ItemSlot"));

		FName Name = ItemSlot->ItemName;
		FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(Name, TEXT("UEquippedComponent::ServerEquipParts Fail Row"));
		if (Row)
		{
			UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::ServerEquipParts = Row"));

			TArray<int32> EquippedGunIndex = GetEquippedGunIndex(); // 현재 장착된 무기 인덱스 반환

			if (EquippedGunIndex.Num() > 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::ServerEquipParts EquippedGunIndex.Num = %d"), EquippedGunIndex.Num());

				TArray<int32> CompatibleWeapon = CheckEquippedWeaponCompatibleParts(Name, EquippedGunIndex); // 현재 장착 중인 무기에 파츠를 낄 수 있는지
				
				
				if (CompatibleWeapon.Num() > 0) // 파츠 장착이 가능한 무기가 있다는 것 
				{
					UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::ServerEquipParts CompatibleWeapon.Num = %d"), CompatibleWeapon.Num());

					//FName PartsName = FName(*(Name.ToString() + "_" + );  
					// 우선 순위를 결정
					// 1. 빈 칸인지 (다 빈 칸이면 손에 들고 있는 무기 > 1번 슬롯 부터)

					FString PartsDataTablePath = "/Game/Blueprint/Weapon/Datatable/DT_PartsData.DT_PartsData";
					UDataTable* PartsDT = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *PartsDataTablePath));
					
					for (int i=0; i<CompatibleWeapon.Num(); i++)
					{
						if (PartsDT)
						{
							UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::ServerEquipParts CompatibleWeapon[i] = %d"), CompatibleWeapon[i]);

							AGun_Base* Gun = Cast<AGun_Base>(EquippedItems[CompatibleWeapon[i]]);
							if (Gun)
							{
								UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::ServerEquipParts Gun Casting"));
								FString PartsString = Name.ToString() + "_" + Gun->GetWeaponDataAsset().GunName;
								FName PartsName = FName(*PartsString);
								FPartsData* PartsDataRow = PartsDT->FindRow<FPartsData>(PartsName, TEXT("Parts"));

								 // 무게, 종류, 이름 스태틱 메쉬
								if (Row)
								{
									if (Gun->EquipParts(*PartsDataRow, Row->Weight, Row->Category))
									{
										UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::ServerEquipParts Gun->EquipParts = true"));
										return true;
									}
									else
									{
										UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::ServerEquipParts Gun->EquipParts = false"));
									}
								}
							}
						}
					}
					// 2. 다 파츠가 장착되어 있으면 (손에 들고 있는 무기 > 1번 슬롯 부터)
				}
			}
		}
	}
	return false;
}


TArray<EGunType> UEquippedComponent::GetCompatibleWeaponType(FName Name) const
{
	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(Name, TEXT("Find Row"));
	
	TArray<EGunType> CompatibleWeaponType;
	if (Row->CompatibleWeaponPartsTypeStruct.Pistol) CompatibleWeaponType.Add(EGunType::Pistol);
	if (Row->CompatibleWeaponPartsTypeStruct.AR) CompatibleWeaponType.Add(EGunType::AR);
	if (Row->CompatibleWeaponPartsTypeStruct.SG) CompatibleWeaponType.Add(EGunType::SG);
	if (Row->CompatibleWeaponPartsTypeStruct.SR) CompatibleWeaponType.Add(EGunType::SR);
	if (Row->CompatibleWeaponPartsTypeStruct.DMR) CompatibleWeaponType.Add(EGunType::DMR);
	if (Row->CompatibleWeaponPartsTypeStruct.SMG) CompatibleWeaponType.Add(EGunType::SMG);
	
	return CompatibleWeaponType;
}

bool UEquippedComponent::IsCompatibleWeaponParts(FName Name, AGun_Base* Gun)
{
	TArray<EGunType> GunTypes = GetCompatibleWeaponType(Name);

	for (int i = 0; i<GunTypes.Num(); i++)
	{
		if (Gun->GetWeaponDataAsset().Type == GunTypes[i])
		{
			return true;
		}
	}
	return false;
}

TArray<int32> UEquippedComponent::CheckEquippedWeaponCompatibleParts(FName Name, TArray<int32> EquippedGunIndex)
{
	TArray<int32> EquippedWeaponParts;

	TArray<EGunType> GunTypes = GetCompatibleWeaponType(Name);

	for (int i= 0; i<EquippedGunIndex.Num(); i++)
	{
		AGun_Base* Gun = Cast<AGun_Base>(EquippedItems[EquippedGunIndex[i]]);

		if (Gun)
		{
			for (int l = 0; l < GunTypes.Num(); l++)
			{
				if (Gun->GetWeaponDataAsset().Type == GunTypes[l])
				{
					EquippedWeaponParts.Add(EquippedGunIndex[i]);
					break;
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::CheckEquippedWeaponCompatibleParts = EquippedWeaponParts.Num %d"), EquippedWeaponParts.Num());

	return EquippedWeaponParts;
}

TArray<int32> UEquippedComponent::GetEquippedGunIndex()
{
	TArray<int32> EquippedGuns;

	for (int i = 0; i <= 2 ; i++)
	{
		if (EquippedItems[i] != nullptr)
		{
			EquippedGuns.Add(i);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::GetEquippedGunIndex = EquippedGuns.Num %d"), EquippedGuns.Num());
	return EquippedGuns;
}

int32 UEquippedComponent::GetRowIndex(UDataTable* DataTable, FName TargetRowName)
{
	if (DataTable == nullptr)
	{
		return -1;
	}

	TArray<FName> RowNames = DataTable->GetRowNames(); // 모든 RowName 가져오기
	//TArray<FName> RowNames = DataTable->FindRow<>();
	


	for (int32 Index = 0; Index < RowNames.Num(); Index++)
	{
		if (RowNames[Index] == TargetRowName)
		{
			return Index; // 일치하는 인덱스 반환
		}
	}

	return -1;
}

FVector UEquippedComponent::DropLocation()
{
	if (GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : DropLocation"));
		
		FVector Start = GetOwner()->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("Start Location : %f %f %f"), Start.X, Start.Y, Start.Z);
		FVector End = GetOwner()->GetActorLocation() - FVector(0.0f, 0.0f, 500.0f);
		UE_LOG(LogTemp, Warning, TEXT("End Location : %f %f %f"), End.X, End.Y, End.Z);
		ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_WorldDynamic);
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(GetOwner());
		FHitResult Hit;

		//UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceChannel, false, IgnoreActors, EDrawDebugTrace::Persistent, Hit, true, FLinearColor::Red, FLinearColor::Green);

		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceChannel, false, IgnoreActors, EDrawDebugTrace::Persistent, Hit, true, FLinearColor::Red, FLinearColor::Green))
		{
			UE_LOG(LogTemp, Warning, TEXT("Drop Location : %f %f %f"), Hit.Location.X, Hit.Location.Y, Hit.Location.Z);
			return Hit.Location;
		}

		UE_LOG(LogTemp, Warning, TEXT("Drop Location : 0"));
		return FVector(0, 0, 0);	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : DropLocation"));

		FVector Start = GetOwner()->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("Start Location : %f %f %f"), Start.X, Start.Y, Start.Z);
		FVector End = GetOwner()->GetActorLocation() - FVector(0.0f, 0.0f, 500.0f);
		UE_LOG(LogTemp, Warning, TEXT("End Location : %f %f %f"), End.X, End.Y, End.Z);
		ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_WorldDynamic);
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(GetOwner());
		FHitResult Hit;

		UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceChannel, false, IgnoreActors, EDrawDebugTrace::Persistent, Hit, true, FLinearColor::Red, FLinearColor::Green);

		// if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceChannel, false, IgnoreActors, EDrawDebugTrace::Persistent, Hit, true, FLinearColor::Red, FLinearColor::Green))
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("Drop Location : %f %f %f"), Hit.Location.X, Hit.Location.Y, Hit.Location.Z);
		// 	return Hit.Location;
		// }

		UE_LOG(LogTemp, Warning, TEXT("Drop Location : 0"));
		return FVector(0, 0, 0);
	}

	
	//return FVector(0, 0, 0);
}

void UEquippedComponent::PrintEquippedItems()
{
	if (GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : PrintEquippedItems"));

		for (int i = 0; i < EquippedItems.Num(); i++)
		{
			if (EquippedItems[i] != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("%d : %s"), i, *EquippedItems[i]->GetActorNameOrLabel());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : PrintEquippedItems"));

		for (int i = 0; i < EquippedItems.Num(); i++)
		{
			if (EquippedItems[i] != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("%d : %s"), i, *EquippedItems[i]->GetActorNameOrLabel());
			
			}
		}
		
	}
}

void UEquippedComponent::ServerPrintEquippedItems_Implementation()
{
	if (GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : PrintEquippedItems"));

		for (int i = 0; i < EquippedItems.Num(); i++)
		{
			if (EquippedItems[i] != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("%d : %s"), i, *EquippedItems[i]->GetActorNameOrLabel());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : PrintEquippedItems"));

		for (int i = 0; i < EquippedItems.Num(); i++)
		{
			if (EquippedItems[i] != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("%d : %s"), i, *EquippedItems[i]->GetActorNameOrLabel());
			
			}
		}
		
	}
}

void UEquippedComponent::PrintWeaponParts()
{
	for (int i=0; i<=2; i++)
	{
		if (EquippedItems[i] != nullptr)
		{
			if (AGun_Base* Gun = Cast<AGun_Base>(EquippedItems[i]))
			{
				UE_LOG(LogTemp, Warning, TEXT("UEquippedComponent::PrintWeaponParts() = EquippedItems[%d]"), i);
				Gun->PrintPartsSlot();
				Gun->ServerPrintPartsSlot();
			}
		}
	}
}

void UEquippedComponent::PrintCurrentWeapon()
{
	if (GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : PrintCurrentWeapon"));

		if (IsValid(CurrentWeapon))
		{
			UE_LOG(LogTemp, Warning, TEXT("Execute Server : PrintCurrentWeapon = %s"), *CurrentWeapon->GetActorNameOrLabel());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : PrintCurrentWeapon"));

		if (IsValid(CurrentWeapon))
		{
			UE_LOG(LogTemp, Warning, TEXT("Execute Client : PrintCurrentWeapon = %s"), *CurrentWeapon->GetActorNameOrLabel());
		}
	}
}

void UEquippedComponent::ServerPrintCurrentWeapon_Implementation()
{
	if (GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : ServerPrintCurrentWeapon_Implementation"));

		if (IsValid(CurrentWeapon))
		{
			UE_LOG(LogTemp, Warning, TEXT("Execute Server : ServerPrintCurrentWeapon_Implementation = %s"), *CurrentWeapon->GetActorNameOrLabel());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : ServerPrintCurrentWeapon_Implementation"));

		if (IsValid(CurrentWeapon))
		{
			UE_LOG(LogTemp, Warning, TEXT("Execute Server : ServerPrintCurrentWeapon_Implementation = %s"), *CurrentWeapon->GetActorNameOrLabel());
		}
	}
}

void UEquippedComponent::EquippingWeaponUpdate(AEquipableItem* OutEquippedItem, UTextureRenderTarget2D* OutLoadedRT)
{

	OutEquippedItem->SetTextureRenderTarget(DuplicateObject<UTextureRenderTarget2D>(OutLoadedRT, this));
	
	OutEquippedItem->GetSceneCaptureComponent()->TextureTarget = OutEquippedItem->GetTextureRenderTarget();
	OutEquippedItem->GetSceneCaptureComponent()->bCaptureEveryFrame = false;
	OutEquippedItem->GetSceneCaptureComponent()->bCaptureOnMovement = false;
	OutEquippedItem->GetSceneCaptureComponent()->bEnableClipPlane = false;
	OutEquippedItem->GetSceneCaptureComponent()->bConsiderUnrenderedOpaquePixelAsFullyTranslucent = false;
	OutEquippedItem->GetSceneCaptureComponent()->ShowOnlyActorComponents(OutEquippedItem, true);

	OutEquippedItem->GetSceneCaptureComponent()->ShowFlags.SetDynamicShadows(false);
	OutEquippedItem->GetSceneCaptureComponent()->ShowFlags.SetStaticMeshes(true);
	OutEquippedItem->GetSceneCaptureComponent()->ShowFlags.SetSkeletalMeshes(true);
	
	OutEquippedItem->GetSceneCaptureComponent()->CaptureScene();
}

void UEquippedComponent::SetCurrentWeapon(AWeapon_Base* _CurrentWeapon)
{
	this->CurrentWeapon = _CurrentWeapon;
	
	CurrentWeaponDelegate.ExecuteIfBound();
}

void UEquippedComponent::SetLastCurrentWeapon(AWeapon_Base* _LastCurrentWeapon)
{
	this->LastCurrentWeapon = _LastCurrentWeapon;
}

void UEquippedComponent::SetPrimarySlotWeapon(AWeapon_Base* _PrimarySlot)
{
	this->PrimarySlot = _PrimarySlot;
}

void UEquippedComponent::SetSecondarySlotWeapon(AWeapon_Base* _Secondary)
{
	this->SecondarySlot = _Secondary;
}

void UEquippedComponent::SetSideArmSlotWeapon(AWeapon_Base* _SideArm)
{
	this->SideArmSlot = _SideArm;
}

void UEquippedComponent::SetMeleeSlotWeapon(AWeapon_Base* _Melee)
{
	this->MeleeSlot = _Melee;
}

void UEquippedComponent::SetThrowableSlotWeapon(AWeapon_Base* _Throwable)
{
	this->ThrowableSlot = _Throwable;
}






