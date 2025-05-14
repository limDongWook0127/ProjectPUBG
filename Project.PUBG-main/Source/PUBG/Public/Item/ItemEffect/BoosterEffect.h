// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemEffect/BaseItemEffect.h"
#include "BoosterEffect.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API ABoosterEffect : public ABaseItemEffect
{
	GENERATED_BODY()

public:
	ABoosterEffect();
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Booster, meta = (AllowPrivateAccess = "true"))
	UDataTable* BoosterEffectTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Booster, meta = (AllowPrivateAccess = "true"))
	FName BoosterEffectTableRowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Booster, meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* BoosterEffectSkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Booster, meta = (AllowPrivateAccess = "true"))
	float UseTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Booster, meta = (AllowPrivateAccess = "true"))
	float StaminaChargeValue;
	
public:
	// Getter
	UFUNCTION(BlueprintCallable)
	FName GetBoosterEffectTableRowName() { return BoosterEffectTableRowName; }
	// Setter
	UFUNCTION(BlueprintCallable)
	void SetBoosterEffectTableRowName(FName OutBoosterEffectTableRowName) { BoosterEffectTableRowName = OutBoosterEffectTableRowName; }

	UFUNCTION(BlueprintCallable)
	void UseItem();
};
