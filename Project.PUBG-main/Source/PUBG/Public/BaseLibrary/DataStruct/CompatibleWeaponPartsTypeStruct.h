#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "CompatibleWeaponPartsTypeStruct.generated.h"


enum class ItemEnum : uint8;

USTRUCT(BlueprintType)
struct FCompatibleWeaponPartsTypeStruct : public FTableRowBase
{
	GENERATED_BODY()
	
	FCompatibleWeaponPartsTypeStruct()
	{
		AR = false;
		SR = false;
		SG = false;
		DMR = false;
		SMG = false;
		Pistol = false;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool SR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool SG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DMR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool SMG;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Pistol;
	
};
