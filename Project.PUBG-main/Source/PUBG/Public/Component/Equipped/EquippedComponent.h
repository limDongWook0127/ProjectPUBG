// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseLibrary/DataStruct/ItemSlotStruct.h"
#include "Components/ActorComponent.h"
#include "Weapon/Grenade/Grenade_Base.h"
#include "Weapon/Guns/Gun_Base.h"
#include "EquippedComponent.generated.h"


class AArmor_Base;
enum class EEquippedItemCategory : uint8;
class AWeaponItem;
enum class EItemCategory : uint8;
struct FItemSlotStruct;
class AItemBase;

DECLARE_DELEGATE_OneParam( FCurrentWeaponAmmoDelegate, int32);
DECLARE_DELEGATE(FCurrentWeaponDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUBG_API UEquippedComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquippedComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FCurrentWeaponAmmoDelegate CurrentWeaponAmmoDelegate;
	FCurrentWeaponDelegate CurrentWeaponDelegate;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UDataTable* ItemDataTable;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UDataTable* ArmorDataTable;
	
	// 무기 5칸, 방어구(헬멧, 가방, 조끼, 길리슈트) 4칸, 의상 (머리, 안경, 마스크, 셔츠, 겉옷, 한벌 옷, 바지, 신발) 8칸 총 17칸
	// 01 = 주무기, 2 = 보조무기 , 3 = 근접무기, 4 = 투척무기, 5 = 헬멧, 6 = 가방, 7 = 조끼, 8 = 길리, 9 ~ 16 의상
	UPROPERTY(ReplicatedUsing=OnRep_EquippedItems, BlueprintReadWrite, Category="Equipped", meta = (AllowPrivateAccess = true))
	TArray<AEquipableItem*> EquippedItems;
	
	
	// //TArray<AItemBase*> EquippedMainWeapon; // 현재 장착된 무기, 생성자에서 크기 지정 (2)
	// UPROPERTY(BlueprintReadWrite, Replicated, Category="Equipped", meta = (AllowPrivateAccess = true))
	// TArray<AWeapon_Base*> EquippedMainWeapon; // 현재 장착된 무기, 생성자에서 크기 지정 (2)
	// AItemBase* EquippedSubWeapon; // 보조 무기
	// AItemBase* MeleeWeapon; // 근접 무기


public:
	// OnRep
	UFUNCTION()
	void OnRep_EquippedItems();
	UFUNCTION()
	void OnRep_CurrentWeapon();

	// Getter
	TArray<AEquipableItem*> &GetEquippedItems () { return EquippedItems; }
	UDataTable* GetItemDataTable() const { return ItemDataTable; }
	
	// 1. F를 누르면 아이템이 들어옴
	// 2. 아이템을 통해 카테고리를 나눔
	EItemCategory GetEquippedItemCategory(AItemBase* Item);
	// 3. 카테고리에 따라 인덱스에 할당
	// 4. 이미 인덱스에 값이 있으면 스왑

	
	
	// 메인 무기 빈 슬롯 찾기
	int32 FindSlotMainWeapon();
	// 메인 무기 장착
	UFUNCTION(Server, Reliable)
	void ServerEquipMainItem(AItemBase* Item);
	// 메인 무기 바닥에 버리기
	int32 DropMainWeapon(AGun_Base* OutCurrentWeapon = nullptr, int32 OutIndex = -1);
	UFUNCTION(Server, Reliable)
	void ServerDropMainWeapon(AGun_Base* OutCurrentWeapon = nullptr, int32 OutIndex = -1);
	// 스태틱 메인 무기 소환
	UFUNCTION(Server, Reliable)
	void ServerSpawnStaticMeshFromMainWeapon(AGun_Base* OutCurrentWeapon);
	
	// 서브 무기 장착
	UFUNCTION(Server, Reliable)
	void ServerEquipSubWeapon(AItemBase* Item);
	// 서브 무기 바닥에 버리기
	void DropSUbWeapon(AGun_Base* OutCurrentWeapon = nullptr);
	UFUNCTION(Server, Reliable)
	void ServerDropSUbWeapon(AGun_Base* OutCurrentWeapon = nullptr);
	// 스태틱 서브 무기 소환
	UFUNCTION(Server, Reliable)
	void ServerSpawnStaticMeshFromSubWeapon(AGun_Base* OutCurrentWeapon);

	// 헬멧 장착
	UFUNCTION(Server, Reliable)
	void ServerEquiptHelmet(AItemBase* Item);
	// 헬멧 바닥에 버리기
	void DropArmor(int32 OutIndex);
	UFUNCTION(Server, Reliable)
	void ServerDropArmor(int32 OutIndex);
	// 스태틱 헬멧 소환
	UFUNCTION(Server, Reliable)
	void ServerSpawnStaticMeshFromArmor(AArmor_Base* OutCurrentArmor);

	// 수류탄 장착
	//UFUNCTION(Server, Reliable)
	void EquipThrow(AItemBase* Item = nullptr, FItemSlotStruct* ItemSlot = nullptr);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerEquipThrow(AItemBase* Item = nullptr, FItemSlotStruct ItemSlot = FItemSlotStruct());
	
	// 파츠 장착
	//UFUNCTION(Server, Reliable)
	bool EquipParts(AItemBase* PartsItem = nullptr, int32 Index = -1, FItemSlotStruct* ItemSlot = nullptr);

	UFUNCTION()
	TArray<EGunType> GetCompatibleWeaponType(FName Name) const; 
	UFUNCTION()
	bool IsCompatibleWeaponParts(FName Name, AGun_Base* Gun); // 그 무기에 파츠를 장착할 수 있는지 
	UFUNCTION()
	TArray<int32> CheckEquippedWeaponCompatibleParts(FName Name, TArray<int32> EquippedGunIndex); // 그 무기에 파츠를 장착할 수 있는지 
	UFUNCTION()
	TArray<int32> GetEquippedGunIndex();
	
	// 데이터 테이블 행 인덱스 가져오기
	int32 GetRowIndex(UDataTable* DataTable, FName TargetRowName);
	// Drop 위치 가져오기
	FVector DropLocation();
	// 장착중인 배열 가져오기 
	UFUNCTION(BlueprintCallable)
	void PrintEquippedItems();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerPrintEquippedItems();
	UFUNCTION(BlueprintCallable)
	void PrintWeaponParts();
	UFUNCTION(BlueprintCallable)
	void PrintCurrentWeapon();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerPrintCurrentWeapon();
	// 무기 장착 시 씬캡쳐 컴포넌트 연결
	void EquippingWeaponUpdate(AEquipableItem* OutEquippedItem, UTextureRenderTarget2D* OutLoadedRT);
	
	// 재윤 ======================================
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon, visibleAnywhere, Category="Equipped")
	AWeapon_Base* CurrentWeapon;	// 현재 장착 무기

	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* LastCurrentWeapon;// 무기 넣었을 시 넣기 이전 장착했었던 무기

	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* PrimarySlot;		// 첫번쨰 무기 슬롯
	
	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* SecondarySlot;	// 두번째 무기 슬롯

	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* SideArmSlot;		// 권총 슬롯

	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* MeleeSlot;		// 근점 무기 슬롯

	UPROPERTY(visibleAnywhere, Replicated, Category="Equipped")
	AWeapon_Base* ThrowableSlot;	// 투척물 슬롯


	// Getter
	UFUNCTION(BlueprintCallable)
	AWeapon_Base* GetCurrentWeapon() const {return CurrentWeapon; }
	
	UFUNCTION(BlueprintCallable)
	AWeapon_Base* GetLastCurrentWeapon() const { return LastCurrentWeapon; }
	
	UFUNCTION(BlueprintCallable)
	AWeapon_Base* GetPrimarySlotWeapon() const { return Cast<AWeapon_Base>(EquippedItems[0]);}
	
	UFUNCTION(BlueprintCallable)
	AWeapon_Base* GetSecondarySlot() const { return Cast<AWeapon_Base>(EquippedItems[1]); }

	UFUNCTION(BlueprintCallable)
	AWeapon_Base* GetSideArmSlot() const { return Cast<AWeapon_Base>(EquippedItems[2]); }

	UFUNCTION(BlueprintCallable)
	AWeapon_Base* GetMeleeSlot() const { return Cast<AWeapon_Base>(EquippedItems[3]); }

	UFUNCTION(BlueprintCallable)
	AWeapon_Base* GetThrowableSlot() const { return Cast<AWeapon_Base>(EquippedItems[4]); }
	
	// UFUNCTION(BlueprintCallable)
	// AWeapon_Base* GetThrowableSlot() const { return ThrowableSlot; }

	// setter
	UFUNCTION(BlueprintCallable)
	void SetCurrentWeapon(AWeapon_Base* _CurrentWeapon);
	
	UFUNCTION(BlueprintCallable)
	void SetLastCurrentWeapon(AWeapon_Base* _LastCurrentWeapon);

	UFUNCTION(BlueprintCallable)
	void SetPrimarySlotWeapon(AWeapon_Base* _PrimarySlot);
	
	UFUNCTION(BlueprintCallable)
	void SetSecondarySlotWeapon(AWeapon_Base* _Secondary);

	UFUNCTION(BlueprintCallable)
	void SetSideArmSlotWeapon(AWeapon_Base* _SideArm);

	UFUNCTION(BlueprintCallable)
	void SetMeleeSlotWeapon(AWeapon_Base* _Melee);

	UFUNCTION(BlueprintCallable)
	void SetThrowableSlotWeapon(AWeapon_Base* _Throwable);

	// ------------- gun base casting 반환
	UFUNCTION(BlueprintCallable)
	AGun_Base* GetCurrentWeapon_GunBase() const { return Cast<AGun_Base>(CurrentWeapon);}

	UFUNCTION(BlueprintCallable)
	AGun_Base* GetLastCurrentWeapon_GunBase() const { return Cast<AGun_Base>(LastCurrentWeapon);}
	
	UFUNCTION(BlueprintCallable)
	AGun_Base* GetPrimary_GunBase() const { return Cast<AGun_Base>(PrimarySlot);}
	
	UFUNCTION(BlueprintCallable)
	AGun_Base* GetSecondary_GunBase() const { return Cast<AGun_Base>(SecondarySlot);}

	UFUNCTION(BlueprintCallable)
	AGun_Base* GetPistol_GunBase() const { return Cast<AGun_Base>(SideArmSlot);}

	UFUNCTION(BlueprintCallable)
	AGrenade_Base* GetThrow_GrenadeBase() const { return Cast<AGrenade_Base>(ThrowableSlot);}
};









