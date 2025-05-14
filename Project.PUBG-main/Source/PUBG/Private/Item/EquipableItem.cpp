// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EquipableItem.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values
AEquipableItem::AEquipableItem()
{
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;
	PrimaryActorTick.bCanEverTick = false;

	// if (HasAuthority())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Execute Server : AEquipableItem"));
	// 	UE_LOG(LogTemp, Warning, TEXT("Execute Server : AEquipableItem %s"), *GetActorNameOrLabel());
	// 	
	// 	//ClientCreateMaterial_Implementation();
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Execute Client : AEquipableItem"));
	// }

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->bCaptureEveryFrame = false; 
	SceneCapture->bEditableWhenInherited = true;

	//SceneCapture->ShowOnlyComponent(this);
	//SceneCapture->ShowOnlyActorComponents(this, true);
}

//Called when the game starts or when spawned
void AEquipableItem::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server AEquipableItem::BeginPlay() : AEquipableItem"));
		UE_LOG(LogTemp, Warning, TEXT("Execute Server AEquipableItem::BeginPlay() : AEquipableItem %s"), *GetActorNameOrLabel());
		
		//ClientCreateMaterial_Implementation();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client AEquipableItem::BeginPlay() : AEquipableItem"));
	}
	
	FString DataTablePath = TEXT("/Game/Datatables/ItemTable.ItemTable");
	ItemDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));

	if (ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDataTable"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemDataTable None"));

	}
}

void AEquipableItem::ClientCreateMaterial_Implementation()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : ClientCreateMaterial_Implementation"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : ClientCreateMaterial_Implementation"));
	}
		
	
	if (UTextureRenderTarget2D* LoadedRT = Cast<UTextureRenderTarget2D>(StaticLoadObject(
		UTextureRenderTarget2D::StaticClass(), 
		nullptr, 
		TEXT("/Game/Blueprint/Widgets/Materials/RenderTarget/RT_Weapon.RT_Weapon")
	)))
	{
		SceneCapture->TextureTarget = LoadedRT;
		SceneCapture->CaptureScene();
		TextureRenderTarget = LoadedRT;

		UMaterialInterface* LoadedMaterial = Cast<UMaterialInterface>(StaticLoadObject(
		UMaterialInterface::StaticClass(),
		nullptr,
		TEXT("/Game/Blueprint/Widgets/Materials/RenderTarget/M_RenderTarget_Inst.M_RenderTarget_Inst")
	   ));

		RenderMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), LoadedMaterial);
		UE_LOG(LogTemp, Warning, TEXT("RenderMaterialInstance : %s"), *RenderMaterialInstance->GetName());
		
		if (RenderMaterialInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT(" Dynamic Material Instance successfully created!"));
			RenderMaterialInstance->SetTextureParameterValue(TEXT("WeaponTexture"), LoadedRT);

			UTexture* RetrievedTexture = nullptr;
			RenderMaterialInstance->GetTextureParameterValue(TEXT("WeaponTexture"), RetrievedTexture);

			if (RetrievedTexture == LoadedRT)
			{
				UE_LOG(LogTemp, Warning, TEXT(" WeaponTexture successfully set in the material!"));
			}
			else if (RetrievedTexture == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT(" GetTextureParameterValue() returned NULL! The texture was not set correctly."));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT(" WeaponTexture is not correctly set. Retrieved different texture."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT(" Failed to create Dynamic Material Instance!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" Failed to create LoadedRT"));

	}
}


// Called every frame
// void AEquipableItem::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

