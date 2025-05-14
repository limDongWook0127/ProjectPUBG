// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataTableManager.generated.h"

UENUM(BlueprintType)
enum class EDataTableType : uint8
{
	ItemData UMETA(DisplayName = "ItemData"),
	WeaponData UMETA(DisplayName = "WeaponData"),
	PartsData UMETA(DisplayName = "PartsData"),
	ArmorData UMETA(DisplayName = "ArmorData"),
	ConsumeEffectData UMETA(DisplayName = "ConsumeEffectData"),
};
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PUBG_API UDataTableManager : public UObject
{
	GENERATED_BODY()
	
	
public:
	UDataTableManager();

	void InitializeManager();
	UDataTable* GetDataTable(EDataTableType TableName);

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TMap<EDataTableType, UDataTable*> DataTables;
};
