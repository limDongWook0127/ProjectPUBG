// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehicleBase.h"
#include "Vehicle_UAZ.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API AVehicle_UAZ : public AVehicleBase
{
	GENERATED_BODY()
public:
	virtual void InteractWith_Implementation(APlayerCharacter* Character) override;
	UFUNCTION()
	virtual FText LookAt() override;
	
	
};
