// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/NearArea/NearComponent.h"

#include "BaseLibrary/DataStruct/ItemSlotStruct.h"
#include "BaseLibrary/DataStruct/ItemStruct.h"
#include "Component/ItemData/ItemDataComponent.h"
#include "Controller/BasePlayerController.h"
#include "Item/ItemBase.h"
#include "Widgets/Inventory/InventoryWidget.h"

// Sets default values for this component's properties
UNearComponent::UNearComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	// ...
}


// Called when the game starts
void UNearComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UNearComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UNearComponent, GroundItems);
	DOREPLIFETIME_CONDITION(UNearComponent, GroundItems, COND_OwnerOnly);
}

void UNearComponent::OnRep_GroundItems()
{
	UE_LOG(LogTemp, Warning, TEXT("GroundItems Replication"));

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
			}
		}
	}
}

void UNearComponent::AddToGroundItem()
{
	
}

void UNearComponent::UpdateNear()
{
	GroundItems.Sort([](const AItemBase& LHS, const AItemBase& RHS)
	{
			FString DataTablePath = TEXT("/Game/Datatables/ItemTable.ItemTable");
			UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));
	
			if (DataTable)
			{
				UE_LOG(LogTemp, Warning, TEXT("DataTable!!"));
			}
		
			FItemStruct* LHSRow = DataTable->FindRow<FItemStruct>(LHS.GetItemDataComponent()->GetItemRowName(), TEXT("Get Item Row"));
			FItemStruct* RHSRow = DataTable->FindRow<FItemStruct>(RHS.GetItemDataComponent()->GetItemRowName(), TEXT("Get Item Row"));
			
			//UE_LOG(LogTemp, Warning, TEXT("LHS.ItemName: %s, RHS.ItemName: %s"), *LHSRow->Name.ToString(), *RHSRow->Name.ToString());
			
			//LHSRow 또는 RHSRow가 nullptr인 경우를 처리
		   if (!LHSRow || !RHSRow)
		   {
			   UE_LOG(LogTemp, Warning, TEXT("Row not found for one or both items!"));
			   return false;
		   }
			
			if (LHSRow->Category != RHSRow->Category)
			{
				return LHSRow->Category < RHSRow->Category;
			}
				
			if (LHSRow->Name != RHSRow->Name)
			{
				return LHSRow->Name.ToString() < RHSRow->Name.ToString();  // Name이 다르면 Name 순으로 오름차순 정렬
			}
				
			return LHSRow->Quantity > RHSRow->Quantity;  // Quantity가 다르면 Quantity 순으로 오름차순 정렬
	});
}

bool UNearComponent::ShouldUpdate(AItemBase* ItemBase)
{
	for (int i = 0; i> GroundItems.Num(); i++)
	{
		if (GroundItems[i] != ItemBase)
		{
			return false;
			
		}
	}
	return true;
}

void UNearComponent::ServerAddGroundItem_Implementation(AItemBase* OutItemBase)
{
	GroundItems.Add(OutItemBase);
}

void UNearComponent::ServerGetGroundItem_Implementation()
{
	GetGroundItems();
}

void UNearComponent::ServerUpdateNear_Implementation()
{
	//UpdateNear();
}

void UNearComponent::ServerEmptyGroundItem_Implementation()
{
	GroundItems.Empty();
}

void UNearComponent::PrintGroundItems()
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server PrintGroundItems"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client PrintGroundItems"));
	}
	
	for (int32 Index = 0; Index < GroundItems.Num(); ++Index)
	{
		FString Msg = GroundItems[Index]->GetItemDataComponent()->GetItemRowName().ToString();
		int32 Quantity = GroundItems[Index]->GetItemDataComponent()->GetQuantity();;
		
		UE_LOG(LogTemp, Warning, TEXT("%d, %s, %d"), Index, *Msg, Quantity);
	}
}

void UNearComponent::ServerPrintGroundItems_Implementation()
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server ServerPrintGroundItems_Implementation"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client ServerPrintGroundItems_Implementation"));
	}
	
	for (int32 Index = 0; Index < GroundItems.Num(); ++Index)
	{
		FString Msg = GroundItems[Index]->GetItemDataComponent()->GetItemRowName().ToString();
		int32 Quantity = GroundItems[Index]->GetItemDataComponent()->GetQuantity();;
		
		UE_LOG(LogTemp, Warning, TEXT("%d, %s, %d"), Index, *Msg, Quantity);
	}
}

void UNearComponent::ServerRemoveGroundItem_Implementation(int32 OutIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Remove Ground Item %d"), OutIndex);
	
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : ServerRemoveGroundItem_Implementation "));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : ServerRemoveGroundItem_Implementation "));
	}

	if (GroundItems.IsValidIndex(OutIndex))
	{
		GroundItems.RemoveAt(OutIndex);
			
		TArray<AItemBase*> TempArray = GroundItems;
		GroundItems = TempArray;
			
		UE_LOG(LogTemp, Warning, TEXT("GroundItems updated on server! New count: %d"), GroundItems.Num());
			
	}
	
	// if (GroundItems.IsValidIndex(OutIndex))
	// {
	// 	
	// 	GroundItems.RemoveAt(OutIndex);
	// 	
	// 	TArray<AItemBase*> TempArray = GroundItems;
	// 	GroundItems = TempArray;
	// 	
	// 	UE_LOG(LogTemp, Warning, TEXT("GroundItems updated on server! New count: %d"), GroundItems.Num());
	// }
}

void UNearComponent::ServerRequestGroundItems_Implementation()
{
	if (GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : ServerRequestGroundItems_Implementation"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : ServerRequestGroundItems_Implementation"));
	}
	
	GetGroundItems();
}





