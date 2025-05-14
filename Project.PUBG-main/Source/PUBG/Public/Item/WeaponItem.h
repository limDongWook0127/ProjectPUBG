// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API AWeaponItem : public AItemBase
{
	GENERATED_BODY()

	AWeaponItem();

	virtual void BeginPlay() override;
public:
	virtual FText LookAt() override;

	void InteractWith(APlayerCharacter* Character);
	virtual void InteractWith_Implementation
				(
				APlayerCharacter* Character
				) override;
};
