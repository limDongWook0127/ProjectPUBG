// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Guns/Gun_Base.h"

#include "EnhancedActionKeyMapping.h"
#include "NaniteSceneProxy.h"
#include "BaseLibrary/DataEnum/ItemEnum.h"
#include "BaseLibrary/DataStruct/ArmorStruct.h"
#include "Weapon/DataTable/DT_PartsData.h"

AGun_Base::AGun_Base()
{
	ScopeMesh = CreateDefaultSubobject<UStaticMeshComponent>("ScopeMesh");
	ScopeMesh->SetupAttachment(WeaponSkeletalMeshComponent,FName("ScopeSocket"));

	MagMesh = CreateDefaultSubobject<UStaticMeshComponent>("MagMesh");
	MagMesh->SetupAttachment(WeaponSkeletalMeshComponent, FName("MagSocket"));

	MuzzleMesh = CreateDefaultSubobject<UStaticMeshComponent>("MuzzleMesh");
	MuzzleMesh->SetupAttachment(WeaponSkeletalMeshComponent,FName("MuzzleSocket"));

	GripMesh = CreateDefaultSubobject<UStaticMeshComponent>("GripMesh");
	GripMesh->SetupAttachment(WeaponSkeletalMeshComponent, FName("GripSocket"));

	LoopsMesh = CreateDefaultSubobject<UStaticMeshComponent>("LoopsMesh");
	LoopsMesh->SetupAttachment(WeaponSkeletalMeshComponent, FName("StockSocket"));

	//SetReplicates(true);
	bReplicates = true;

	// 파츠 정보 초기화
	ActivateMag = false;
	ActivateMuzzle = false;
	ActivateScope = false;

	BulletArmo = 0;

	Changevalue = 0.0f;
	
	// 이준수
	FString DataTablePath = TEXT("/Game/Blueprint/Weapon/Datatable/DT_WeaponData.DT_WeaponData");
	WeaponDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));

	int32 WeaponType = static_cast<int32>(GetWeaponDataAsset().Type);
	
	// UE_LOG(LogTemp, Warning, TEXT("WeaponType : %d"), WeaponType);
	// if (WeaponType == 5)
	// {
	// 	EquipSlot = EEquippedItemCategory::SubWeapon;
	// }
}

void AGun_Base::BeginPlay()
{
	Super::BeginPlay();
	
	int32 WeaponType = static_cast<int32>(GetWeaponDataAsset().Type);

	if (WeaponType == 5)
	{
		EquipSlot = EEquippedItemCategory::SubWeapon;
	}
	// else
	// {
	// 	EquipSlot = EEquippedItemCategory::PrimarySlot;
	// }
}

void AGun_Base::SetFiremode(EFiremodes Firemodes)
{
	this->FireMode = Firemodes;
}

void AGun_Base::ChangeFiremode_AR(EFiremodes _Firemode)
{
	if (IgnoreMode != EFiremodes::None)
	{
		if (IgnoreMode == _Firemode)
		{
			this->FireMode = EFiremodes::Semi_Auto;
		}
		else
		{
			this->FireMode = _Firemode;
		}
	}
	this->FireMode = _Firemode;

}

void AGun_Base::SetMagToHandSocekt()
{
	//if (MagMesh->GetAttachParent())
}

void AGun_Base::SetBulletArom(float Armo)
{
	this->BulletArmo = Armo;
}

FVector AGun_Base::GetWeaponMagSocketLocation() const
{
	return WeaponSkeletalMeshComponent->GetSocketLocation(FName("ProjectileSocket"));
}

void AGun_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGun_Base, BulletArmo);
	DOREPLIFETIME(AGun_Base, WeaponParts);
	DOREPLIFETIME(AGun_Base, MagMesh);
	DOREPLIFETIME(AGun_Base, ScopeMesh);
	DOREPLIFETIME(AGun_Base, MuzzleMesh);

	DOREPLIFETIME(AGun_Base, ActivateMag);
	DOREPLIFETIME(AGun_Base, ActivateMuzzle);
	DOREPLIFETIME(AGun_Base, ActivateScope);
	DOREPLIFETIME(AGun_Base, Changevalue);


}

// 스테틱 메쉬 셋팅
void AGun_Base::SettingStaticmesh(int32 PartsDataArray)
{
	
	if (PartsDataArray == 1)
	{
		FString ScopeName = WeaponParts[1].PartsName.ToString();
		FString GunName = GetWeaponDataAsset().GunName;
	
		ScopeName.ReplaceInline(*FString(TEXT("_") + GunName), TEXT(""));
		FName NewPartsName = FName(*ScopeName);
		

		if (NewPartsName == "ACOG_01")
		{
			FRotator NewRotator = FRotator(0, -90, 0);
			ScopeMesh->SetRelativeRotation(NewRotator);
		}
		else
		{
			FRotator NewRotator = FRotator(0, 0, 0);
			ScopeMesh->SetRelativeRotation(NewRotator);
		}
		
		ScopeMesh->SetStaticMesh(WeaponParts[PartsDataArray].StaticMesh);
		ActivateScope = true;
	}

	if (PartsDataArray == 2) 
	{
		MagMesh->SetStaticMesh(WeaponParts[PartsDataArray].StaticMesh);
		ActivateMag = true;
	}
	
	if (PartsDataArray == 3)
	{
		MuzzleMesh->SetStaticMesh(WeaponParts[PartsDataArray].StaticMesh);
		ActivateMuzzle = true;
	}
}


void AGun_Base::Server_SettingParts_Implementation(int32 partsIndex)
{
	if (HasAuthority())
	{
		SettingStaticmesh(partsIndex);
		
		// if (WeaponParts[partsIndex].PartsName.IsValid())
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("EquipedPartsName : %s"), *WeaponParts[partsIndex].PartsName.ToString());
		// }
		// else
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("EquipedPartsName : NoValid"));
		// }
	}
	
}

bool AGun_Base::Server_SettingParts_Validate(int32 partsIndex)
{
	return true;
}

TArray<EPartsCategory> AGun_Base::GetInstalledParts() const
{
	TArray<EPartsCategory> InstalledParts;
	if (WeaponDataAsset.Installable_Parts.Scope) InstalledParts.Add(EPartsCategory::Scope);
	if (WeaponDataAsset.Installable_Parts.Mag) InstalledParts.Add(EPartsCategory::Mag);
	if (WeaponDataAsset.Installable_Parts.Muzzle) InstalledParts.Add(EPartsCategory::Muzzle);
	
	return InstalledParts;
}

bool AGun_Base::IsEquipParts(EPartsCategory PartsCategory)
{
	TArray<EPartsCategory> EquippableParts = GetInstalledParts();

	for (int32 i = 0; i < EquippableParts.Num(); i++)
	{
		if (EquippableParts[i] == PartsCategory)
		{
			return true;
		}
	}

	return false;
}

bool AGun_Base::EquipParts(FPartsData& PartsData, float Weight, EItemCategory ItemCategory)
{
	FString PartsString = PartsData.TypeName.ToString() + "_" + WeaponDataAsset.GunName;
	FName PartsName = FName(*PartsString);
	
	if (IsEquipParts(PartsData.PartsCategory))
	{
		int32 PartsCategory_int = static_cast<int32>(PartsData.PartsCategory);
		//UE_LOG(LogTemp, Warning, TEXT("Gun_Base::EquipParts  PartsCategory_int = %d"), PartsCategory_int);

		if (WeaponParts[PartsCategory_int].PartsName == NAME_None)
		{
			WeaponParts[PartsCategory_int].PartsName = PartsName;
			WeaponParts[PartsCategory_int].ItemCategory = ItemCategory;
			WeaponParts[PartsCategory_int].Weight = Weight;
			WeaponParts[PartsCategory_int].StaticMesh = PartsData.Parts_StaticMesh;
			WeaponParts[PartsCategory_int].ChangeValue = PartsData.ChangedValues;
			
			ServerSetParts(PartsCategory_int, PartsName,Weight,  ItemCategory,PartsData.Parts_StaticMesh, PartsData.ChangedValues);
			
			if (PartsCategory_int)
			{
				Server_SettingParts(PartsCategory_int);
			}
			else
			//	UE_LOG(LogTemp, Warning, TEXT("artsCategory_int = null"));

			
			// UE_LOG(LogTemp, Warning, TEXT("Gun_Base::EquipParts PartsCategory_int = %s"), *PartsString);
			// UE_LOG(LogTemp, Warning, TEXT("Gun_Base::EquipParts = true"));
				//UE_LOG(LogTemp, Warning, TEXT("artsCategory_int = null"));

			
			//UE_LOG(LogTemp, Warning, TEXT("Gun_Base::EquipParts PartsCategory_int = %s"), *PartsString);
			//UE_LOG(LogTemp, Warning, TEXT("Gun_Base::EquipParts = true"));
			return true;
		}
		
		// if (PartsSlot[PartsCategory_int] == NAME_None)
		// {
		// 	PartsSlot[PartsCategory_int] = PartsName;
		// 	UE_LOG(LogTemp, Warning, TEXT("Gun_Base::EquipParts PartsCategory_int = %s"), *PartsString);
		// 	return true;
		// }
	}
	return false;
}

void AGun_Base::ServerSetParts_Implementation(int32 NewIndex, FName NewName, float Weight, EItemCategory ItemCategory,
	UStaticMesh* NewStaticMesh, float _Changevalue)
{
	WeaponParts[NewIndex].PartsName = NewName;
	WeaponParts[NewIndex].ItemCategory = ItemCategory;
	WeaponParts[NewIndex].Weight = Weight;
	WeaponParts[NewIndex].StaticMesh = NewStaticMesh;
	WeaponParts[NewIndex].ChangeValue = _Changevalue;
}

void AGun_Base::PrintPartsSlot()
{
	if (HasAuthority())
	{
		// UE_LOG(LogTemp, Warning, TEXT("Execute Server : PrintPartsSlot"));
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("Execute Client : PrintPartsSlot"));
	}

	for (int32 i = 0; i < WeaponParts.Num(); i++)
	{
		if (WeaponParts[i].PartsName != NAME_None)
		{
			// UE_LOG(LogTemp, Warning, TEXT("WeaponParts[%d] = %s"), i, *WeaponParts[i].PartsName.ToString());
		}
	}
}

void AGun_Base::ServerPrintPartsSlot_Implementation()
{
	if (HasAuthority())
	{
		// UE_LOG(LogTemp, Warning, TEXT("Execute Server : ServerPrintPartsSlot_Implementation"));
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("Execute Client : ServerPrintPartsSlot_Implementation"));
	}

	for (int32 i = 0; i < PartsSlot.Num(); i++)
	{
		if (WeaponParts[i].PartsName != NAME_None)
		{
			// UE_LOG(LogTemp, Warning, TEXT("WeaponParts[%d] = %s"), i, *WeaponParts[i].PartsName.ToString());
		}
	}
}

void AGun_Base::Server_SetBulletArom_Implementation(float Armo)
{
	SetBulletArom(Armo);
}

bool AGun_Base::Server_SetBulletArom_Validate(float Armo)
{
	return true;
}




