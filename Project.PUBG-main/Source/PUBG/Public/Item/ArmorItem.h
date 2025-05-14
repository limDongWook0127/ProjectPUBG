// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseLibrary/DataStruct/ArmorStruct.h"
#include "Item/ItemBase.h"
#include "ArmorItem.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API AArmorItem : public AItemBase
{
	GENERATED_BODY()

public:
	AArmorItem();
	
	void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	FArmorStruct ArmorData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	float Durability; // 내구도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	float Defense; // 방어력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	float Weight;

public:
	void InteractWith(APlayerCharacter* Character);
	virtual void InteractWith_Implementation(APlayerCharacter* Character) override;
};


