// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NearComponent.generated.h"

class AItemBase;
struct FItemStruct;
struct FItemSlotStruct;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUBG_API UNearComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNearComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(ReplicatedUsing = OnRep_GroundItems, EditDefaultsOnly, BlueprintReadWrite, category = "Inventory", meta=(AllowPrivateAccess=true))
	TArray<AItemBase*> GroundItems;
	
	//TArray<FItemSlotStruct> GroundItem;

public:
	// OnRep
	UFUNCTION()
	void OnRep_GroundItems();

	// Setter

	// Getter
	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<AItemBase*>& GetGroundItems() { return GroundItems; }
	
	UFUNCTION(BlueprintCallable)
	void AddToGroundItem();
	UFUNCTION(BlueprintCallable)
	void UpdateNear();
	UFUNCTION(BlueprintCallable)
	bool ShouldUpdate(AItemBase* ItemBase);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRequestGroundItems();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerRemoveGroundItem(int32 OutIndex);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerAddGroundItem(AItemBase* OutItemBase);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerGetGroundItem();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerUpdateNear();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerEmptyGroundItem();

	UFUNCTION(BlueprintCallable)
	void PrintGroundItems();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerPrintGroundItems();
};
