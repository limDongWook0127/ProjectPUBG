// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseLibrary/DataEnum/ItemEnum.h"
#include "BaseLibrary/DataStruct/ItemSlotStruct.h"
#include "Components/ActorComponent.h"
#include "Weapon/Weapon_Base.h"
#include "Weapon/Guns/Gun_Base.h"
#include "InventoryComponent.generated.h"


class AItemBase;

DECLARE_DELEGATE_OneParam(FRemainAmmoDelegate, int32);

USTRUCT(BlueprintType)
struct FUsingItem
{
	GENERATED_BODY()

	FUsingItem()
	{
		Item = FItemSlotStruct();
		Index = -1;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemSlotStruct Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Index;
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUBG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	FRemainAmmoDelegate RemainAmmoDelegate;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// 변수 영역
private:
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, category = "Inventory", meta=(AllowPrivateAccess=true))
	float MaxInventoryWeight; // 50 // 총알의 경우 0.5, 0.4임
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Inventory", meta=(AllowPrivateAccess=true))
	float CurrentInventoryWeight;
	
	UPROPERTY(ReplicatedUsing = OnRep_Content, EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TArray<FItemSlotStruct> Content;
	UPROPERTY(ReplicatedUsing = OnRep_UsingItem, EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	FUsingItem UsingItem;
	UPROPERTY(ReplicatedUsing= OnRep_Item, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess=true))
	AItemBase* Item;
	UPROPERTY(Replicated, BlueprintReadWrite, Category="Inventory", meta=(AllowPrivateAccess=true))
	AItemBase* NearItem;

	UPROPERTY()
	UDataTable* ItemDataTable;

// 함수 영역
public:
	// OnRep
	UFUNCTION()
	void OnRep_Content();
	UFUNCTION()
	void OnRep_UsingItem();
	UFUNCTION()
	void OnRep_Item();
	
	// Setter
	void SetItem(AItemBase* OutItem) { Item = OutItem; }
	UFUNCTION(Server, Reliable)
	void ServerSetItem(AItemBase* OutItem);
	void SetNearItem(AItemBase* OutNearItem) { NearItem = OutNearItem; }
	UFUNCTION(BlueprintCallable)
	void SetContent(TArray<FItemSlotStruct> OutContent) { Content = OutContent; }
	UFUNCTION(BlueprintCallable)
	void SetUsingItem(FUsingItem OutUsingItem) { UsingItem = OutUsingItem; }

	UFUNCTION(Server, Reliable)
	void ServerSetNearItem(AItemBase* OutNearItem);
	UFUNCTION(Server, Reliable)
	void ServerSetContents(const TArray<FItemSlotStruct>& OutContnets);
	UFUNCTION(BlueprintCallable)
	void SetMaxInventoryWeight(float NewInventoryWeight) { MaxInventoryWeight = NewInventoryWeight; }
	
	// Getter
	AItemBase* GetItem() const { return Item; }
	AItemBase* GetNearItem() const { return NearItem; }
	TArray<FItemSlotStruct> &GetContent() { return Content; }
	FUsingItem GetUsingItem() { return UsingItem; }
	UFUNCTION(BlueprintCallable)
	float GetMaxInventoryWeight() { return MaxInventoryWeight; }
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable) 
	void Server_Interact();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable) 
	void Server_InteractItem(AItemBase* OutItemBase); // 바닥에 있는 아이템
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerGetItem();

	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerAddToInventory(FName ItemID, int32 Quantity, int32 Weight, EItemCategory ItemCategory);
	UFUNCTION(BlueprintCallable)
	int32 AddToInventory(FName ItemID, int32 Quantity, int32 Weight, EItemCategory ItemCategory);
	UFUNCTION(BlueprintCallable)
	int32 FindItemSlot(FName ItemID);
	UFUNCTION(BlueprintCallable)
	void AddToStack(int32 Index, int32 Quantity, int32 Weight, EItemCategory ItemCategory);
	UFUNCTION(BlueprintCallable)
	void CreateNewStack(FName ItemID, int32 Quantity, int32 Weight, int32 Index, EItemCategory ItemCategory );
	UFUNCTION(BlueprintCallable)
	void AddToLastIndexNewStack(FName ItemID, int32 Quantity, int32 Weight, EItemCategory ItemCategory);
	UFUNCTION(BlueprintCallable)
	int32 GetMaxStackSize(FName ItemID);
	// 아이템 카테고리 분류하고 함수 실행
	EItemCategory GetEquippedItemCategory(AItemBase* InItem);
	void InteractionsByCategory(AItemBase* InItem);


	
	// 아이템 먹었을 때 정렬되는 거 
	UFUNCTION(BlueprintCallable) 
	void UpdateInventory();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerUpdateInventory();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ReplicateContent();
	// 아이템 버리기, 소모하기
	UFUNCTION(BlueprintCallable)
	void RemoveFromInventory(int32 InIndex, bool IsConsumed,  int32 InQuantity = 1);
	
	// 아이템 버리기
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetUsingItem(FUsingItem OutUsingItem);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerDropItem(int32 InIndex, int32 OutQuantity);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRemoveItem(int32 InIndex, int32 OutQuantity);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSpawnItem(int32 InIndex,	int32 OutQuantity);
	
	UFUNCTION(BlueprintCallable)
	void PrintContents();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerPrintContents();
	UFUNCTION(BlueprintCallable)
	void PrintMaxInventoryWeight();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerPrintMaxInventoryWeight();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRemoveAt(int32 Index);
};






