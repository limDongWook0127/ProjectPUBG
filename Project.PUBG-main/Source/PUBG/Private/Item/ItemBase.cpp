// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemBase.h"

//#include "K2Node_GetDataTableRow.h"
#include "BaseLibrary/BaseFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Character/TestCharacter.h"
#include "Components/BoxComponent.h"
#include "Character/TestCharacter.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Component/ItemData/ItemDataComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/PlayerCharacter.h"
#include "GameInstance/BaseGameInstance.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Manager/DataTableManager.h"


//int32 AItemBase::BeginOverlapCount = 0;

AItemBase::AItemBase()
{

	
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
	//SetReplicateMovement(true); // 위치 변화를 동기화하려면 추가
	//NetDormancy = DORM_Initial; // 네트워크 동기화 활성화
	
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetIsReplicated(true);
	RootComponent = StaticMesh;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetIsReplicated(true);
	BoxComponent->SetupAttachment(StaticMesh);
	InteractionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionComponent"));
	InteractionComponent->SetIsReplicated(true);
	InteractionComponent->SetupAttachment(StaticMesh);

	
	//InteractionComponent->InitBoxExtent(FVector(20.0f));

	ItemDataComponent = CreateDefaultSubobject<UItemDataComponent>(TEXT("ItemDataComponent"));
	ItemDataComponent->SetIsReplicated(true);
	
	TableIndex = -1;
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	ForceNetUpdate();
	
	UBaseGameInstance* GI = Cast<UBaseGameInstance>(GetGameInstance());
	ItemDataTable = GI->GetDataTableManager()->GetDataTable(EDataTableType::ItemData);

	if (ItemDataTable)
	{
		FItemStruct* Row;
		if (!ItemTableRowName.IsNone())
		{
			Row = ItemDataTable->FindRow<FItemStruct>(ItemTableRowName, TEXT("AItemBase : Fail Find Row"));
			
			StaticMesh->SetStaticMesh(Row->StaticMesh); // 스태틱 메쉬 지정	
			SetStaticMeshScaleFromCategory(Row->Category); // 아이템 종류에 따른 스태틱 메쉬 크기 지정

			if (ItemDataComponent)
			{
				ItemDataComponent->SetItemID(ItemDataTable, Row->Name);
		
				if (ItemDataComponent->GetQuantity() == 0)
				{
					ItemDataComponent->SetItemQuantity(Row->Quantity);
				}
			
				ItemDataComponent->SetItemWeight(Row->Weight);
				ItemDataComponent->SetItemCategory(Row->Category);		
				SetSlotFromCategory(); // 장착할 수 있는 아이템이면 슬롯 지정

				if(bIsSupplyDrop)
				{
					StaticMesh->SetHiddenInGame(true);
					SetSupplyDropItemCollisionBox();
				}
				else
				{
					SetCollisionScale();
				}
			}
		}
	}

	InteractionComponent->SetVisibility(false);
	BoxComponent->SetVisibility(false);
}

void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemBase, ItemStruct);
	DOREPLIFETIME(AItemBase, Item);
	DOREPLIFETIME(AItemBase, ItemID);
	DOREPLIFETIME(AItemBase, ItemDataComponent);
	DOREPLIFETIME(AItemBase, TableIndex);
	DOREPLIFETIME(AItemBase, ItemTableRowName);
	DOREPLIFETIME(AItemBase, bIsSupplyDrop);
	DOREPLIFETIME(AItemBase, ItemDataTable);
	
	
	//DOREPLIFETIME_CONDITION(AItemBase, ItemDataComponent, COND_OwnerOnly);
	//DOREPLIFETIME_CONDITION(AItemBase, ItemStruct, COND_OwnerOnly);
}

// void AItemBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	if (ATestCharacter* TestCharacter = Cast<ATestCharacter>(OtherActor))
// 	{
// 		BeginOverlapCount += 1;
// 		UE_LOG(LogTemp, Warning, TEXT("%d"), BeginOverlapCount);
//
// 		//UE_LOG(LogTemp, Warning, TEXT("Test Character"));
// 		FTimerDelegate TimerDelegate;
// 		TimerDelegate.BindLambda([OtherActor]()
// 		{
// 			if (ATestCharacter* TestCharacter = Cast<ATestCharacter>(OtherActor))
// 			{
// 				UCameraComponent* Camera = TestCharacter->GetFollowCamera();
//
// 				FHitResult Hit;
// 				FVector Start = Camera->K2_GetComponentLocation();
// 				FVector End = Start + UKismetMathLibrary::GetForwardVector(Camera->K2_GetComponentRotation()) * 380.0f;
// 				TArray<AActor*> ActorsToIgnore;
// 				ActorsToIgnore.Add(TestCharacter);
// 				ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
// 				//TraceType = ETraceTypeQuery
//
// 				if (UInventoryComponent* CharacterInventory = TestCharacter->GetInventoryComponent())
// 				{
// 					//if (CharacterInventory->GetItem() == nullptr)
// 					{
// 						UKismetSystemLibrary::LineTraceSingle(TestCharacter->GetWorld(), Start, End, TraceType, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, false, FLinearColor(1, 0, 0, 0), FLinearColor (0, 1, 0, 1));
//
// 						if (AItemBase* Item = Cast<AItemBase>(Hit.GetActor()))
// 						{
// 							//UE_LOG(LogTemp, Warning, TEXT("Item Base"));
//
// 							if (Item != CharacterInventory->GetItem())
// 							{
// 								CharacterInventory->SetItem(Item);
// 								if (Item->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
// 								{
// 									//UE_LOG(LogTemp, Warning, TEXT("Item %s"), *Item->GetName());
//
// 									Item->LookAt();
// 								}
// 							}
// 						}
// 						else
// 						{
// 							CharacterInventory->SetItem(nullptr);
// 						}
// 					}
// 				}
// 				
// 				
// 			}
// 		});
//
// 		GetWorldTimerManager().SetTimer(BeginOverlapTimerHandle, TimerDelegate, 0.1f, true);
// 		
// 	}
// }
//
// void AItemBase::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	BeginOverlapCount -= 1;
// 	UE_LOG(LogTemp, Warning, TEXT("%d"), BeginOverlapCount);
//
// 	if (BeginOverlapCount < 0)
// 	{
// 		BeginOverlapCount = 0;
// 	}
// 	
// 	if (BeginOverlapCount == 0)
// 	{
// 		GetWorldTimerManager().ClearTimer(BeginOverlapTimerHandle);
//
// 		if (ATestCharacter* TestCharacter = Cast<ATestCharacter>(OtherActor))
// 		{
// 			UInventoryComponent* CharacterInventory = TestCharacter->GetInventoryComponent();
// 			CharacterInventory->SetItem(nullptr);
// 		}
// 	}
// 	
// }

void AItemBase::OnRep_ItemDataComponent()
{
	//UE_LOG(LogTemp, Warning, TEXT("ItemDataComponent Replicate!"));
}


void AItemBase::OnRep_ItemID()
{
	SetRandomProperties(ItemID);	
}

void AItemBase::SetItemID(FName NewItemID,bool NewbIsSupplyDrop)
{	
	bIsSupplyDrop = NewbIsSupplyDrop;
	ItemID = NewItemID;
	ItemTableRowName = NewItemID;
	SetRandomProperties(ItemID);
}

void AItemBase::SetIsSupplyDrop(bool NewIsSupplyDrop)
{
	bIsSupplyDrop = NewIsSupplyDrop;
}

void AItemBase::OnRep_ItemTableRowName()
{
	//UE_LOG(LogTemp, Warning, TEXT("ItemTableRowName Replicate!"));
}

void AItemBase::ServerSetItem_Implementation(FItemStruct const& OutItem)
{
	Item = OutItem;
}

void AItemBase::ServerSetItemStruct_Implementation(FItemStruct OutItemStruct)
{
	ItemStruct = OutItemStruct;
}

void AItemBase::SetSupplyDropItemCollisionBox()
{	
	FVector InnerBoxScale = FVector(10.0f, 10.0f, 10.0f);

	InteractionComponent->SetBoxExtent(InnerBoxScale);
	BoxComponent->SetBoxExtent(InnerBoxScale);
}

FText AItemBase::LookAt()
{
	checkf(ItemDataComponent, TEXT("ItemDataComponent is Null"));
	FName Name = ItemDataComponent->GetItemRowName();
	FText Message = FText::FromString("Picked up the " + Name.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Message.ToString());
	
	//UK2Node_GetDataTableRow()
	ItemOfZ = this->GetActorLocation().Z;
	//UE_LOG(LogTemp, Warning, TEXT("ItemofZ : %f"), ItemOfZ);

	FString NewMessage = Name.ToString() + TEXT(" 줍기");
	Message = FText::FromString(NewMessage);
	return Message;
}

void AItemBase::InteractWith_Implementation(APlayerCharacter* Character)
{
	IInteractInterface::InteractWith_Implementation(Character);

	if (HasAuthority()) // 서버
	{
		 //UE_LOG(LogTemp, Warning, TEXT("Execute Server : ItemBase InteractWith_Implementation"));
		
		// UE_LOG(LogTemp, Warning, TEXT("ItemBase!"));
		// //UE_LOG(LogTemp, Display, TEXT("Interacting with %s"), *GetName());
		//
		// UInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
		// InventoryComponent->SetItem(this);
		// InventoryComponent->ServerSetItem(this);
		// // ItemOfZ = this->GetActorLocation().Z;
		//
		// if (InventoryComponent->GetItem() != nullptr)
		// {
		// 	InventoryComponent->Server_Interact();
		//
		// 	// if (this->ItemDataComponent->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		// 	// {
		// 	// 	ItemDataComponent->InteractWith(Character);
		// 	// }
		// }
	}
	else // 클라 
	{
		 //UE_LOG(LogTemp, Warning, TEXT("Execute Client : ItemBase InteractWith_Implementation"));
		
		//UE_LOG(LogTemp, Warning, TEXT("ItemBase!"));
		//UE_LOG(LogTemp, Display, TEXT("Interacting with %s"), *GetName());
		
		UInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
		InventoryComponent->SetItem(this); // 리플리케이트가 느림
		InventoryComponent->ServerSetItem(this); // 이것만 있으면 클라이언트에서 아이템이 없다고 함 
		// ItemOfZ = this->GetActorLocation().Z;
		
		if (InventoryComponent->GetItem() != nullptr)
		{
			InventoryComponent->Server_Interact();
		
			// if (this->ItemDataComponent->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			// {
			// 	ItemDataComponent->InteractWith(Character);
			// }
		}
	}
	
	// UE_LOG(LogTemp, Warning, TEXT("ItemBase!"));
	// //UE_LOG(LogTemp, Display, TEXT("Interacting with %s"), *GetName());
	//
	// UInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
	// InventoryComponent->SetItem(this);
	// // ItemOfZ = this->GetActorLocation().Z;
	//
	// if (InventoryComponent->GetItem() != nullptr)
	// {
	// 	InventoryComponent->Server_Interact();
	// 	
	// 	 // if (this->ItemDataComponent->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	// 	 // {
	// 	 // 	ItemDataComponent->InteractWith(Character);
	// 	 // }
	// }
	
	//Destroy();
}

void AItemBase::SetMesh(UStaticMesh* NewMesh)
{
	StaticMesh->SetStaticMesh(NewMesh);
	
	if(bIsSupplyDrop)
	{
		StaticMesh->SetHiddenInGame(true);
		SetSupplyDropItemCollisionBox();
	}
	else
	{
		SetCollisionScale();
	}
	
}

void AItemBase::SetSlotFromCategory()
{
	FName ID = GetItemDataComponent()->GetItemRowName();
	FItemStruct* Row = ItemDataTable->FindRow<FItemStruct>(ID, TEXT("Find Row"));

	if (Row == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AItemBase::SetSlotFromCategor = %s"), *ID.ToString());
		return;
	}
	
	int32 SlotNum = static_cast<int32>(Row->Category);
	UE_LOG(LogTemp, Warning, TEXT("AItemBase::SetSlotFromCategory = SlotNum %d"), SlotNum);
	
	switch (SlotNum)
	{
	case 0:
		EquippedItemCategory = EEquippedItemCategory::PrimarySlot;
		break;
	case 2:
		EquippedItemCategory = EEquippedItemCategory::SubWeapon;
		break;
	case 3:
		EquippedItemCategory = EEquippedItemCategory::MeleeWeapon;
		break;
	case 4:
		EquippedItemCategory = EEquippedItemCategory::Throw;
		break;
	case 5:
		EquippedItemCategory = EEquippedItemCategory::Helmet;
		break;
	case 6:
		EquippedItemCategory = EEquippedItemCategory::Bag;
		break;
	case 7:
		EquippedItemCategory = EEquippedItemCategory::Vest;
		break;
	default:
		EquippedItemCategory = EEquippedItemCategory::None;
	}
	
	int32 EquippedItemSlot = static_cast<int32>(EquippedItemCategory);
	UE_LOG(LogTemp, Warning, TEXT("EquippedItemSlot : %d"), EquippedItemSlot);
}

void AItemBase::SetCollisionScale()
{
	FBoxSphereBounds ComponentBoundsStaticMeshBounds = StaticMesh->Bounds;
	ComponentBoundsStaticMeshBounds.BoxExtent = StaticMesh->GetStaticMesh()->GetBounds().BoxExtent;

	UE_LOG(LogTemp, Warning, TEXT("ComponentBoundsStaticMeshBounds : %s"), *ComponentBoundsStaticMeshBounds.ToString());
	
	FVector BoxLocation = ComponentBoundsStaticMeshBounds.Origin;
	FVector InnerBoxScale = FVector(ComponentBoundsStaticMeshBounds.BoxExtent.X * 2.f, ComponentBoundsStaticMeshBounds.BoxExtent.Y * 2.f, ComponentBoundsStaticMeshBounds.BoxExtent.Z * 2.f);
	FVector OutBoxScale = FVector(InnerBoxScale.X + 100.f, InnerBoxScale.Y + 100.f, InnerBoxScale.Z + 100.f);

	InteractionComponent->SetWorldLocation(BoxLocation);
	BoxComponent->SetWorldLocation(BoxLocation);
	InteractionComponent->SetBoxExtent(InnerBoxScale);
	BoxComponent->SetBoxExtent(OutBoxScale);
}


//스폰 시 속성 변경
void AItemBase::SetRandomProperties(FName ItemIdName)
{
	static const FString ContextString(TEXT("Item Lookup"));
	FItemStruct* FoundItem = ItemDataTable->FindRow<FItemStruct>(ItemIdName, ContextString);

	if (FoundItem)
	{
		//아이디 설정
		GetItemDataComponent()->SetItemID(ItemDataTable,ItemIdName);
		UE_LOG(LogTemp, Warning, TEXT("RandomRowName: %s") , *ItemIdName.ToString());
		
	}

	if (FoundItem && FoundItem->StaticMesh)
	{
		//메쉬변경
		SetMesh(FoundItem->StaticMesh);
	}

	if (FoundItem && FoundItem->Weight)
	{
		//무게 변경
		GetItemDataComponent()->SetItemWeight(FoundItem->Weight);
	}

	if (FoundItem && FoundItem->Quantity)
	{
		//수량 변경
		GetItemDataComponent()->SetItemQuantity(FoundItem->Quantity);
	}
	
	if (FoundItem)
	{
		SetSlotFromCategory();
	}

	if (FoundItem)
	{
		GetItemDataComponent()->SetItemCategory(FoundItem->Category);
	}
}

void AItemBase::SetStaticMeshScaleFromCategory(EItemCategory InCategoryIndex)
{
	switch (InCategoryIndex) {
	case EItemCategory::MainWeapon:
		break;
	case EItemCategory::SubWeapon:
		break;
	case EItemCategory::MeleeWeapon:
		break;
	case EItemCategory::Throw:
		break;
	case EItemCategory::Helmet:
		break;
	case EItemCategory::Bag:
		break;
	case EItemCategory::Vest:
		break;
	case EItemCategory::Belt:
		break;
	case EItemCategory::Ammo:
		break;
	case EItemCategory::Heal:
		StaticMesh->SetWorldScale3D(FVector(2.0f));
		break;
	case EItemCategory::Booster:
		StaticMesh->SetWorldScale3D(FVector(2.0f));
		break;
	case EItemCategory::WeaponPart:		
		break;
	case EItemCategory::Head:
		break;
	case EItemCategory::Mask:
		break;
	case EItemCategory::Glasses:
		break;
	case EItemCategory::Shirt:
		break;
	case EItemCategory::Outerwear:
		break;
	case EItemCategory::Glove:
		break;
	case EItemCategory::FullBody:
		break;
	case EItemCategory::Pants:
		break;
	case EItemCategory::Shoes:
		break;
	}	
}