// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor/ItemSpawn/ItemSpawnerComponent.h"

#include "BaseLibrary/DataEnum/ItemEnum.h"
#include "Weapon/DataTable/DT_Weapon.h"
#include "Item/ItemBase.h"
#include "Algo/RandomShuffle.h"
#include "Item/ArmorItem.h"

#include "Item/WeaponItem.h"

// Sets default values for this component's properties
UItemSpawnerComponent::UItemSpawnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UItemSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();


	
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	//SpawnItems();
	// ...
	
}


// Called every frame
void UItemSpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//아이템 스폰 함수
void UItemSpawnerComponent::SpawnItem(FName ItemID,FVector SpawnLocation,bool bAbleAttach, AActor* ParentActor)
{
	UWorld* World = GetWorld();
	
	//아이템 랜덤 생성	
	AItemBase* SpawnedItem;
	FItemStruct* ItemStruct = SpawnItemTable->FindRow<FItemStruct>(ItemID, TEXT("Weapon Info Lookup"));

	EItemCategory ItemCategory = ItemStruct->Category;
	
	switch (ItemCategory) 
	{
	case EItemCategory::MainWeapon:
	case EItemCategory::SubWeapon:
		{
			SpawnedItem = World->SpawnActorDeferred<AWeaponItem>(WeaponItemClass,FTransform(FRotator(0), FVector(0)));
			break;
		}
	case EItemCategory::Helmet:
	case EItemCategory::Vest:
	case EItemCategory::Bag:
		{
			SpawnedItem = World->SpawnActorDeferred<AArmorItem>(ArmorItemClass,FTransform(FRotator(0), FVector(0)));
			break;
		}
	default:
		{
			SpawnedItem = World->SpawnActorDeferred<AItemBase>(ItemBaseClass,FTransform(FRotator(0), FVector(0)));
			break;
		}
	}

	if (SpawnedItem)
	{
		if (bAbleAttach)
		{
			SpawnedItem->AttachToComponent(ParentActor->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		}
		
		//SpawnedItem->SetItemID(ItemID,bAbleAttach);
		SpawnedItem->SetItemTableRowName(ItemID);
		SpawnedItem->SetIsSupplyDrop(bAbleAttach);
		SpawnedItem->FinishSpawning(FTransform(FRotator::ZeroRotator, SpawnLocation));
	}

}

void UItemSpawnerComponent::SpawnItems(bool bAbleAttach , AActor* ParentActor)
{
	AActor* Owner = GetOwner();

	TArray<FVector> ShuffledLocations = SpawnLocations;
	Algo::RandomShuffle(ShuffledLocations);

	//랜덤 위치 랜덤 아이템 스폰
	for (int k = 0; k < LocationCount; k++)
	{
		FVector SpawnLocation = ShuffledLocations[k];
		
		for (int i = 0; i < SpawnItemCount; i++)
		{
			FVector WorldSpawnLocation = Owner->GetTransform().TransformPosition(SpawnLocation);
			
			UE_LOG(LogTemp, Warning, TEXT("GetRandomLocation()"));
			FVector FinalLocation = WorldSpawnLocation + GetRandomOffset();
			
			//스폰시킬 아이템 정하기
			FName SpawnedItemName = GetRandomItemRowName();
			UE_LOG(LogTemp, Warning, TEXT("dddd: %s") , *SpawnedItemName.ToString());
					
			SpawnItem(SpawnedItemName,FinalLocation,bAbleAttach,ParentActor);
			
			//랜덤값이 무기인지 확인
			if (IsWeapon(SpawnedItemName))
			{
				//무기 정보 테이블 접근
				FWeaponData* WeaponData = WeaponTable->FindRow<FWeaponData>(SpawnedItemName, TEXT("Weapon Info Lookup"));
	
				if (WeaponData)
				{
					FName BulletTypeName = GetBulletTypeName(WeaponData->BulletType);
					UE_LOG(LogTemp, Warning, TEXT("BulletTypeName: %s") , *BulletTypeName.ToString());
					
					//무기에 맞는 총알 찾기					
					for (int BulletCount = 0; BulletCount < 2; BulletCount++)
					{
						UE_LOG(LogTemp, Warning, TEXT("BulletCount: %d") , BulletCount);
					
						FVector BulletRandomOffset = GetRandomOffset();
					
						SpawnItem(BulletTypeName,FinalLocation + BulletRandomOffset,bAbleAttach,ParentActor);
					}//for
				}//if
			}//if
		}//for
	}//for	
}

//랜덤 아이템 지정
FName UItemSpawnerComponent::GetRandomItemRowName()
{
	TArray<FName> RowNames = SpawnItemTable->GetRowNames();
	FName RandomRowName = RowNames[FMath::RandRange(0,RowNames.Num()-1)];

	//UE_LOG(LogTemp, Warning, TEXT("RandomRowName: %s") , *RandomRowName.ToString());
	
	return RandomRowName;
}


//무기 타입인지 확인
bool UItemSpawnerComponent::IsWeapon(FName ItemID)
{
	static const FString ContextString(TEXT("Item Lookup"));
	FItemStruct* FoundItem = SpawnItemTable->FindRow<FItemStruct>(ItemID, ContextString);

	return (FoundItem && FoundItem->ItemType == EItemType::GunWeapon);
}

//총알 타입 찾기
FName UItemSpawnerComponent::GetBulletTypeName(EBulletType BulletType)
{
	switch (BulletType)
	{
		case EBulletType::B_7_62: return FName("7.62mm");
		case EBulletType::B_5_56: return FName("5.56mm");
		case EBulletType::B_9: return FName("9mm");
		case EBulletType::B_12G: return FName("12G");
		default: return FName("Unknown");
	}
}

//랜덤 반경
FVector UItemSpawnerComponent::GetRandomOffset()
{
	return FVector(
		FMath::RandRange(-ItemSpawnRadius, ItemSpawnRadius),
		FMath::RandRange(-ItemSpawnRadius, ItemSpawnRadius),
		0.0f
	);
}

FVector UItemSpawnerComponent::GetRandomLocation()
{
	int32 RandomIndex = FMath::RandRange(0, SpawnLocations.Num() - 1);
	UE_LOG(LogTemp, Warning, TEXT("RandomIndex : %d"),RandomIndex);
	return SpawnLocations[RandomIndex];
}
