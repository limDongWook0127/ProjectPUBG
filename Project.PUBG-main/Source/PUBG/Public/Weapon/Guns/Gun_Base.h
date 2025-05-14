// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseLibrary/DataStruct/WeaponPartsStruct.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/Weapon_Base.h"
#include "Weapon/DataTable/DT_Weapon.h"
#include "Gun_Base.generated.h"

struct FPartsData;
/**
 * 
 */
UENUM(BlueprintType)
enum class EFiremodes : uint8
{
	None,
	Auto,
	Single,
	Semi_Auto,
};

DECLARE_DELEGATE_OneParam(FCurrentAmmoDelegate, int32);

UCLASS()
class PUBG_API AGun_Base : public AWeapon_Base
{
	GENERATED_BODY()

public:
	AGun_Base();

	// 이준수
	virtual void BeginPlay() override;

	FCurrentAmmoDelegate CurrentAmmoDelegate;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Parts", replicated)
	UStaticMeshComponent* ScopeMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Parts", replicated)
	UStaticMeshComponent* MuzzleMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Parts", replicated)
	UStaticMeshComponent* MagMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Parts")
	UStaticMeshComponent* GripMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UStaticMeshComponent* LoopsMesh;

	UPROPERTY(EditDefaultsOnly, Category = "FireMode")
	EFiremodes IgnoreMode;

	UPROPERTY(EditDefaultsOnly, Category = "FireMode")
	EFiremodes FireMode = EFiremodes::Single;

	UPROPERTY(EditDefaultsOnly, Replicated)
	float BulletArmo;

	UPROPERTY(Replicated)
	bool ActivateScope;

	UPROPERTY(Replicated)
	bool ActivateMuzzle;
	
	UPROPERTY(Replicated)
	bool ActivateMag;

	UPROPERTY(Replicated)
	float Changevalue;

public:
	UFUNCTION()
	UStaticMeshComponent* getScopeMesh() {return ScopeMesh;}
	
	UFUNCTION(BlueprintCallable, Category = "FireMode")
	FORCEINLINE EFiremodes GetFireMode() const { return FireMode; }

	UFUNCTION(BlueprintCallable, category = "FireMode")
	void SetFiremode(EFiremodes Firemodes);

	UFUNCTION(BlueprintCallable, category = "FireMode")
	void ChangeFiremode_AR(EFiremodes _Firemode);

	UFUNCTION(BlueprintCallable, Category = "Mag")
	UStaticMeshComponent* GetMagToHandSocekt() const {return MagMesh;};
	
	UFUNCTION(BlueprintCallable, Category = "Mag")
	void SetMagToHandSocekt();

	UFUNCTION(BlueprintCallable, Category = "DataAsset")
	float GetBulletArmo() const
	{
		CurrentAmmoDelegate.ExecuteIfBound(BulletArmo);
		return BulletArmo;
	}

	UFUNCTION(BlueprintCallable, Category = "DataAsset")
	void SetBulletArom(float Armo);

	UFUNCTION(BlueprintCallable, Category = "Mag")
	FVector GetWeaponMagSocketLocation() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Mag")
	void Server_SetBulletArom(float Armo);

	// 스테틱메쉬 셋팅
	UFUNCTION()
	void SettingStaticmesh(int32 PartsDataArray);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SettingParts(int32 partsIndex);

	// 장착 여부 셋팅
	UFUNCTION(BlueprintPure)
	bool GetEquippedScope() {return ActivateScope;}
	
	UFUNCTION(BlueprintPure)
	bool GetEquippedMuzzle() {return ActivateMuzzle;}

	UFUNCTION(BlueprintPure)
	bool GetEquippedMag() {return ActivateMag;}

	UFUNCTION(BlueprintPure)
	float GetchangeValue() {return Changevalue;}

// 이준수 ================================
	EEquippedItemCategory GetEquipSlot() const override { return EquipSlot; }
	
	UPROPERTY(EditDefaultsOnly, Category = "Mag")
	UDataTable* WeaponDataTable;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FName> PartsSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, meta = (AllowPrivateAccess = "true"))
	TArray<FWeaponPartsStruct> WeaponParts;

public:
	// Getter
	UFUNCTION()
	TArray<FName> GetPartsSlot() { return PartsSlot; } 
	UFUNCTION()
	TArray<FWeaponPartsStruct>& GetWeaponParts() { return WeaponParts; }
	
	UFUNCTION()
	TArray<EPartsCategory> GetInstalledParts() const; // 현재 무기에 무착할 수 있는 파츠 종류들 
	UFUNCTION()
	bool IsEquipParts(EPartsCategory PartsCategory); // 파츠가 들어왔을 때 장착할 수 있는지
	UFUNCTION()
	bool EquipParts(FPartsData& PartsData, float Weight, EItemCategory ItemCategory);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSetParts(int32 NewIndex, FName NewName, float Weight, EItemCategory ItemCategory, UStaticMesh* NewStaticMesh, float _Changevalue);

	UFUNCTION(BlueprintCallable)
	void PrintPartsSlot();
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerPrintPartsSlot();
	// =====================================
};




