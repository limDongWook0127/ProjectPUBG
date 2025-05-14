// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/DataTable/DT_Weapon.h"
#include "EquipableItem.generated.h"

enum class EEquippedItemCategory : uint8;

UCLASS()
class PUBG_API AEquipableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	EEquippedItemCategory EquipSlot;
	// 이준수
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneCaptureComponent2D *SceneCapture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextureRenderTarget2D *TextureRenderTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* RenderMaterialInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;
	//Setter

public:
	//Getter
	UFUNCTION(BlueprintCallable)
	virtual USkeletalMeshComponent* GetWeaponSkeletalMeshComponent() { return nullptr; };
	virtual EEquippedItemCategory GetEquipSlot() const { return EquipSlot; }
	UFUNCTION(BlueprintCallable)
	USceneCaptureComponent2D* GetSceneCaptureComponent() { return SceneCapture;}
	UFUNCTION(BlueprintCallable)
	UTextureRenderTarget2D* GetTextureRenderTarget() const { return TextureRenderTarget; }
	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* GetRenderMaterial() const { return RenderMaterialInstance; }
	UFUNCTION(BlueprintCallable)
	UDataTable* GetItemDataTable() const { return ItemDataTable; }
	
	//Setter
	void SetEquipSlot(EEquippedItemCategory OutEquippedItemCategory) { EquipSlot = OutEquippedItemCategory; } 
	void SetTextureRenderTarget(UTextureRenderTarget2D* OutTextureRenderTarget) { TextureRenderTarget = OutTextureRenderTarget; } 
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientCreateMaterial();
	
};
