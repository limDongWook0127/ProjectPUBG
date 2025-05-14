#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "BaseLibrary/DataEnum/ItemEnum.h"


#include "ItemSlotStruct.generated.h"


//enum class ItemEnum : uint8;

USTRUCT(BlueprintType)
struct FItemSlotStruct : public FTableRowBase
{
	GENERATED_BODY()

	FItemSlotStruct() : ItemName(NAME_None), Quantity(0), Tag(FGameplayTag()) {}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Quantity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemCategory ItemCategory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag Tag;

};