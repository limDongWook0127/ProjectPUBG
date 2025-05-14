#pragma once
#include "CoreMinimal.h"
#include "CompatibleWeaponPartsTypeStruct.h"
#include "GameplayTagContainer.h"
#include "BaseLibrary/DataEnum/ItemEnum.h"
#include "Engine/DataTable.h"
#include "Item/EquipableItem.h"
#include "ItemStruct.generated.h"


class AItemBase;
enum class EItemCategory : uint8;
enum class EItemType : uint8;

USTRUCT(BlueprintType)
struct FItemStruct : public FTableRowBase
{
	GENERATED_BODY()

	FItemStruct()
	{
		StaticMesh = nullptr;
		BP_Item = nullptr;
		Image = nullptr;
		Weight = -1.0f;
		Category = EItemCategory::FullBody;
		IsStackAble = false;
		StackSize = -1;
		Quantity = -1;
		Tag = FGameplayTag::EmptyTag;
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* StaticMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AEquipableItem> BP_Item;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AItemBase> BP_StaticItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Image;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Weight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemCategory Category;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemType ItemType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsStackAble;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 StackSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Quantity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCompatibleWeaponPartsTypeStruct CompatibleWeaponPartsTypeStruct;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag Tag;
	
	
};

