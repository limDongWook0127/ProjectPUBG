// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/DataTable/DT_Weapon.h"
#include "ItemSpawnerComponent.generated.h"

class AArmorItem;
class AItemBase;
class AWeaponItem;
struct FItemStruct;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUBG_API UItemSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemSpawnerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//스폰존 생성 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<FVector> SpawnLocations;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	// TArray<FVector> SelectedLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AItemBase> ItemBaseClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AWeaponItem> WeaponItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AArmorItem> ArmorItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 LocationCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 SpawnItemCount;

	//아이템 스폰 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 ItemSpawnRadius;

	//아이템 데이터테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	UDataTable* SpawnItemTable;

	//무기테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	UDataTable* WeaponTable;
	
	//TestItem 스폰시킬 함수
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnItem(FName ItemID,FVector SpawnLocation,bool bAbleAttach, AActor* ParentActor);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnItems(bool bAbleAttach , AActor* ParentActor);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FName GetRandomItemRowName();
	
	//무기 확인 함수
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	bool IsWeapon(FName ItemID);

	//무기 타입에 맞는 총알 찾기
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FName GetBulletTypeName(EBulletType BulletType);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FVector GetRandomOffset();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FVector GetRandomLocation();
	
	
};


