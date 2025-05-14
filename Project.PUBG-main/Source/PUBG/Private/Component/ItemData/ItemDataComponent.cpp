// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ItemData/ItemDataComponent.h"

#include "Character/PlayerCharacter.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Component/NearArea/NearComponent.h"
#include "Components/BoxComponent.h"
#include "Controller/BasePlayerController.h"
#include "Item/ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Inventory/InventoryWidget.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"

// Sets default values for this component's properties
UItemDataComponent::UItemDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	ItemID.DataTable = nullptr;
	ItemID.RowName = NAME_None;
	Quantity = 0;
	ItemCategory = EItemCategory::FullBody;
}


// Called when the game starts
void UItemDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//AActor* Owner = GetOwner(); 
	//if (Owner)
	{
		//	Owner->SetReplicates(true);
	}
	if (GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("AItemBase::AItemBase = Execute : Server"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AItemBase::AItemBase = Execute : Client"));
	}

	AActor* MyOwner = GetOwner(); // 이 컴포넌트가 속한 액터 가져오기
	if (MyOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("MyComponent's Owner: %s"), *MyOwner->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MyComponent's Owner is NULL!"));
	}
}

void UItemDataComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemDataComponent, Quantity);

	//DOREPLIFETIME_CONDITION(UItemDataComponent, Quantity, COND_OwnerOnly);
}

void UItemDataComponent::OnRep_Quantity()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Quantity Replicate!"));

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController)
		{
			if (ABasePlayerController* BasePlayerController = Cast<ABasePlayerController>(PlayerController))
			{
				if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(BasePlayerController->GetCharacter()))
				{
					TArray<AItemBase*>& FoundGroundItems = PlayerCharacter->GetNearComponent()->GetGroundItems();
					
					for (AItemBase* Item : FoundGroundItems)
					{
						if (Item == GetOwner())
						{
							if (BasePlayerController->GetInventoryWidget())
							{
								BasePlayerController->GetInventoryWidget()->UpdateInventoryWidget();
								BasePlayerController->GetInventoryWidget()->UpdateNearItemSlotWidget();
								UE_LOG(LogTemp, Warning, TEXT("OnRep_Quantity : Widget Update!"))
							}
						}
					}
				}
			}
		}
	}
	
	// 블루프린트 클래스 로드
	// FString BlueprintPath = TEXT("/Game/Blueprint/Character/BP_PlayerCharacter.BP_PlayerCharacter_C");
	// UClass* BlueprintClass = StaticLoadClass(AActor::StaticClass(), nullptr, *BlueprintPath);
	//
	// if (BlueprintClass)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Blueprint class loaded successfully!"));
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Failed to load blueprint class!"));
	// }
	//
	// if (BlueprintClass)
	// {
	// 	TArray<AActor*> FoundActors;
	// 	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BlueprintClass, FoundActors);
	//
	// 	for (AActor* Actor : FoundActors)
	// 	{
	// 		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Actor))
	// 		{
	// 			TArray<AItemBase*>& FoundGroundItems = PlayerCharacter->GetNearComponent()->GetGroundItems();
	//
	// 			for (AItemBase* Item : FoundGroundItems)
	// 			{
	// 				if (Item == GetOwner())
	// 				{
	// 					if (ABasePlayerController* PlayerController = Cast<ABasePlayerController>(PlayerCharacter->GetController()))
	// 					{
	// 						if (PlayerController->GetInventoryWidget())
	// 						{
	// 							PlayerController->GetInventoryWidget()->UpdateInventoryWidget();
	// 							PlayerController->GetInventoryWidget()->UpdateNearItemSlotWidget();
	// 							UE_LOG(LogTemp, Warning, TEXT("OnRep_Quantity : Widget Update!"));
	//
	// 						}
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	

}

FText UItemDataComponent::LookAt()
{
	return FText::FromString("");
}

void UItemDataComponent::InteractWith_Implementation(APlayerCharacter* Character)
{
	//IInteractInterface::InteractWith_Implementation(Character);

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server ItemDataComponent::InteractWith_Implementation"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client ItemDataComponent::InteractWith_Implementation"));
	}

	UE_LOG(LogTemp, Warning, TEXT("ItemDataComponent!"));

	
	if (UInventoryComponent* Inventory = Character->GetInventoryComponent())
	{
		// UE_LOG(LogTemp, Warning, TEXT("1"));
		UE_LOG(LogTemp, Warning, TEXT("%s, %d"), *ItemID.RowName.ToString(), Quantity);

		// 0이 아니면 인벤이 꽉 차서 아이템이 다 안들어간 것 
		int32 RemainItemQuantity = Inventory->AddToInventory(ItemID.RowName, Quantity, Weight, ItemCategory);

		if (Quantity != RemainItemQuantity)
		{
			UAbilitySystemComponent* AbilitySystemComponent = Character->GetAbilitySystemComponent();
			UBaseAbilitySystemComponent* BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent);
			if (BaseAbilitySystemComponent)
			{
				UE_LOG(LogTemp, Warning, TEXT("TryActivateAbilityByTag : Player.Ability.Pickup "));
				BaseAbilitySystemComponent->TryActivateAbilityByTag(FGameplayTag::RequestGameplayTag(FName("Player.Ability.Pickup")));
			}
		}

		if (RemainItemQuantity != 0)
		{
			// 몽타주

			//UE_LOG(LogTemp, Warning, TEXT("Remain Item : %s, %d"), *ItemID.RowName.ToString(), Quantity);

			AActor* Owner = GetOwner();
			if (Owner)
			{
				if (GetOwner() && GetOwner()->HasAuthority())
				{
					UE_LOG(LogTemp, Warning, TEXT("Execute Server : RemainItemQuantity "));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Execute Client : RemainItemQuantity "));
				}

				if (AItemBase* ItemBase = Cast<AItemBase>(Owner))
				{
					UE_LOG(LogTemp, Warning, TEXT("RemainItemQuantity : %d"), RemainItemQuantity);


					Quantity = RemainItemQuantity; // 리플리케이트되는 시간이 안맞아서 밑에서 강제로 리플리케이트 시킴

					Character->ForceNetUpdate();
					AActor* MyOwner = GetOwner(); // 이 컴포넌트가 속한 액터 가져오기
					if (MyOwner)
					{
						UE_LOG(LogTemp, Warning, TEXT("MyComponent's Owner: %s"), *MyOwner->GetName());
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("MyComponent's Owner is NULL!"));
					}
					

					ItemBase->GetItemStruct().Quantity = RemainItemQuantity;

					UE_LOG(LogTemp, Warning, TEXT("Before Quantity : %d"), Quantity);

					UE_LOG(LogTemp, Warning, TEXT("After Quantity : %d"), Quantity);


					
					
					//Character->GetInventoryComponent()->ReplicateContent();

					//Character->GetInventoryComponent()->ServerSetContents_Implementation(Character->GetInventoryComponent()->GetContent());
					
				}
			}
		}
		// 0이라면 다 들어가서 바닥에 있는 아이템이 없어져야 함 
		else
		{
			AActor* Owner = GetOwner();
			if (Owner)
			{
				Character->SetLookAtActor(nullptr);
				Owner->Destroy();
			}
		}
	}
}

void UItemDataComponent::SetItemID(UDataTable* ItemDataTable,FName ItemIdName)
{
	if (!ItemDataTable || ItemIdName == NAME_None)
	{
		return;
	}
	
	ItemID.DataTable = ItemDataTable;
	ItemID.RowName = ItemIdName;
}

void UItemDataComponent::SetItemWeight(int32 ItemWeight)
{
	Weight = ItemWeight;
}

void UItemDataComponent::SetItemQuantity(float ItemQuantity)
{
	Quantity = ItemQuantity;
}

