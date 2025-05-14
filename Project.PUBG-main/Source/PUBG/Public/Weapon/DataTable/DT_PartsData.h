// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DT_Weapon.h"
#include "Engine/DataTable.h"
#include "DT_PartsData.generated.h"

/**
 * 
 */

USTRUCT(Atomic, BlueprintType)
struct FPartsData : public FTableRowBase
{

	GENERATED_BODY()

	FPartsData();  // 초기화

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName GunName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPartsCategory PartsCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChangedValues;
	
	UPROPERTY(editanywhere, BlueprintReadWrite)
	FName AttachParts_SocketName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Parts_StaticMesh;
	
	
};

UCLASS()
class PUBG_API UDT_PartsData : public UDataTable
{
	GENERATED_BODY()
	
};
