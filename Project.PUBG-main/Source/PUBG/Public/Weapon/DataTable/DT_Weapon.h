// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DT_Weapon.generated.h"

UENUM(BlueprintType)
enum class EGunType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	AR, 
	SR,
	DMR,
	SMG,
	SG,
	Pistol,
	Melee,
	Throwable,
};

UENUM(BlueprintType)
enum class EBulletType : uint8
{
	None,
	B_7_62 UMETA(DisplayName = "7.62mm"),
	B_5_56 UMETA(DisplayName = "5.56mm"),
	B_9 UMETA(DisplayName = "9mm"),
	B_12G UMETA(DisplayName = "12G"),
};

USTRUCT(Atomic, BlueprintType)
struct FInstallable_parts : public FTableRowBase
{

	GENERATED_BODY()

	FInstallable_parts();  // 초기화
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Scope;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Mag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Muzzle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Grip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Loops; // 샷건
};

UENUM(BlueprintType)
enum class EPartsCategory : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Scope UMETA(DisplayName = "Scope"),
	Mag UMETA(DisplayName = "Mag"),
	Muzzle UMETA(DisplayName = "Muzzle"),
};

USTRUCT(Atomic, BlueprintType)
struct FRecoilMontage : public FTableRowBase
{

	GENERATED_BODY()

	FRecoilMontage();  // 초기화
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Stand_RecoilMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Prone_RecoilMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Crouch_RecoilMontage;
};
/**
 * 
 */

USTRUCT(Atomic, BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	FWeaponData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGunType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GunName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBulletType BulletType;

	// 기본 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Gun_Damage;

	// 기본 장전 탄창
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Armo;

	// 총알 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Bullet_Speed;

	// 총알 단면적
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Bullet_crossSection;

	// 총알 무게
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float bullet_Mass;

	// 총알 항력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Bullet_DragCoefficiency;

	// 파츠별 장착 가능 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInstallable_parts Installable_Parts;
	
	// 반동 값 =====================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Min_XRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max_XRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Min_YRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max_YRecoil;

	// ===========================================

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRecoilMontage RecoilMontage;
	
};

UCLASS()
class PUBG_API UDT_Weapon : public UDataTable
{
	GENERATED_BODY()
	
};
