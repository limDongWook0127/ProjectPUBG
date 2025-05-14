// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/Startup/DataAsset_StartupBase.h"
#include "DataAsset_StartupPlayer.generated.h"

struct FPlayerAbilitySet;
/**
 * 
 */
UCLASS()
class PUBG_API UDataAsset_StartupPlayer : public UDataAsset_StartupBase
{
	GENERATED_BODY()	

protected:
	virtual void GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* ASC, int32 Level = 1) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "StartupData", meta = (TitleProPerty = "InputTag"))
	TArray<FPlayerAbilitySet> PlayerStartupAbilitySets;
};
