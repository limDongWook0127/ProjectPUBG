#pragma once
#include "CoreMinimal.h"
#include "ItemEnum.generated.h"

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	MainWeapon = 0 UMETA(Display = "MainWeapon"),
	SubWeapon  = 2 UMETA(Display = "SubWeapon"),
	MeleeWeapon = 3 UMETA(Display = "MeleeWeapon"),
	Throw = 4 UMETA(Display = "Throw"),
	
	Helmet = 5 UMETA(Display = "Helmet"),
	Bag = 6  UMETA(Display = "Bag"),
	Vest = 7 UMETA(Display = "Vest"),
	Belt = 8 UMETA(Display = "Belt"),
	
	Ammo = 9  UMETA(Display = "Ammo"),
	
	Heal = 10 UMETA(Display = "Heal"),
	Booster = 11 UMETA(Display = "Booster"),
	
	WeaponPart = 12 UMETA(Display = "WeaponPart"),

	Head  UMETA(Display = "Head"),
	Mask UMETA(Display = "Mask"),
	Glasses UMETA(Display = "Glasses"),

	Shirt UMETA(Display = "Shirts"),
	Outerwear UMETA(Display = "Outerwear"),
	Glove UMETA(Display = "Glove"),
	FullBody UMETA(Display = "FullBody"),

	Pants UMETA(Display = "Pants"),
	Shoes UMETA(Display = "Shoes"),
};

UENUM(BlueprintType)
enum class EEquippedItemCategory : uint8
{
	None = 99 UMETA(Display = "None"),
	PrimarySlot = 0 UMETA(Display = "PrimarySlot"),
	SecondarySlot UMETA(Display = "SecondarySlot"),
	SubWeapon UMETA(Display = "SubWeapon"),
	MeleeWeapon UMETA(Display = "MellWeapon"),
	Throw UMETA(Display = "Throw"),
	
	Helmet = 5 UMETA(Display = "Helmet"),
	Bag UMETA(Display = "Bag"),
	Vest UMETA(Display = "Vest"),
	
};

//아이템 대분류
UENUM(BlueprintType)
enum class EItemType : uint8
{
	None UMETA(DisplayName = "None"),           // 정의되지않음
	GunWeapon UMETA(DisplayName = "GunWeapon"),       //주무기,보조무기
	
	ThrowWeapon UMETA(DisplayName = "ThrowWeapon"),		   //투척무기
	MeleeWeapon UMETA(DisplayName = "MeleeWeapon"),		   //근접무기
	
   Ammo UMETA(DisplayName = "Ammo"),           //탄약
	
   Consumable UMETA(DisplayName = "Consumable"), //소모품
	
   Equipment UMETA(DisplayName = "Equipment"), //장비
	
   WeaponPart UMETA(DisplayName = "WeaponPart"), //파츠
};
