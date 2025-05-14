// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Armor/Armor_Base.h"

#include "Character/PlayerCharacter.h"
#include "Component/Inventory/InventoryComponent.h"
#include "GameInstance/BaseGameInstance.h"
#include "Manager/DataTableManager.h"

AArmor_Base::AArmor_Base()
{
	bReplicates = true;

	//StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = SkeletalMeshComponent;
	
	SkeletalMeshComponent->SetIsReplicated(true);
	Name = NAME_None;

	Weight = -1.0f;
	Defense = -1.0f;
	Durability = -1.0f;
	
}

void AArmor_Base::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())  // 서버에서 실행 여부 체크
	{
		UE_LOG(LogTemp, Warning, TEXT("AArmor_Base::BeginPlay = Execute Server"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AArmor_Base::BeginPlay = Execute Client"));
	}
	
	if (!Name.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("AArmor_Base::BeginPlay : %s"), *Name.ToString());
		//FString ArmorTablePath = "/Game/Datatables/Armor/DT_Armor.DT_Armor";
		//UDataTable* ArmorTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ArmorTablePath));

		UBaseGameInstance* GI = Cast<UBaseGameInstance>(GetGameInstance());
		UDataTable* ArmorTable = GI->GetDataTableManager()->GetDataTable(EDataTableType::ArmorData);
		FArmorStruct* FoundData = ArmorTable->FindRow<FArmorStruct>(Name, TEXT("Fail Find Row"));
		
		if (FoundData) // nullptr 체크 필수
		{
			UE_LOG(LogTemp, Warning, TEXT("AArmor_Base::BeginPlay : FoundData"));
			//StaticMeshComponent->SetStaticMesh(FoundData->StaticMesh);
			//SkeletalMeshComponent->SetSkeletalMesh(FoundData->SkeletalMesh);
			UE_LOG(LogTemp, Warning, TEXT("AArmor_Base::BeginPlay : %s"), *FoundData->SkeletalMesh->GetName());
			ArmorData = *FoundData; // 구조체 값을 복사해서 대입
			UE_LOG(LogTemp, Warning, TEXT("AArmor_Base::BeginPlay : %s"), *FoundData->Image->GetName());
		}
	}

	if (Durability == -1)
	{
		Durability = ArmorData.Durability;
	}
	if (Defense == -1)
	{
		Defense = ArmorData.Defense;
	}
	if (Weight == -1)
	{
		Weight = ArmorData.Weight;
		UE_LOG(LogTemp, Warning, TEXT("AArmor_Base::BeginPlay = %f"),Weight );
	}

	
}

void AArmor_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AArmor_Base, StaticMeshComponent);
	DOREPLIFETIME(AArmor_Base, Name);
	DOREPLIFETIME(AArmor_Base, SkeletalMeshComponent);

}

void AArmor_Base::OnRep_StaticMeshComponent()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_StaticMeshComponent"));
}

void AArmor_Base::EquipArmor(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->SetArmor(PlayerCharacter->GetArmor() + Defense);
	PlayerCharacter->GetInventoryComponent()->SetMaxInventoryWeight(PlayerCharacter->GetInventoryComponent()->GetMaxInventoryWeight() + Weight);

	UE_LOG(LogTemp, Warning, TEXT("EquipArmor : MaxInventoryWeight : %f"), PlayerCharacter->GetInventoryComponent()->GetMaxInventoryWeight());
}

void AArmor_Base::UnEquipArmor(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->SetArmor(PlayerCharacter->GetArmor() - Defense);
	PlayerCharacter->GetInventoryComponent()->SetMaxInventoryWeight(PlayerCharacter->GetInventoryComponent()->GetMaxInventoryWeight() - Weight);
}

void AArmor_Base::SetMesh(UStaticMesh* NewStaticMesh)
{
	//StaticMeshComponent->SetStaticMesh(NewStaticMesh);
}

void AArmor_Base::SetSetSkeletalMesh(USkeletalMesh* NewSkeletalMesh)
{
	SkeletalMeshComponent->SetSkeletalMesh(NewSkeletalMesh);
}
