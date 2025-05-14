// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/DataTable/DT_Weapon.h"
#include "Widgets/WidgetBase.h"
#include "PlayerStatusWidget.generated.h"

class UTextBlock;
class APlayerCharacter;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PUBG_API UPlayerStatusWidget : public UWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	// Widget Variable
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UProgressBar* ProgressBar_Health;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UProgressBar* ProgressBar_Stamina;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UProgressBar* ProgressBar_FillingGaguge;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UProgressBar* ProgressBar_FillGaguge;
	UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UWidgetAnimation* Anim_ProgressBar_Health;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_CurrentAmmo;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UTextBlock* Text_Ammo;
	
	// Variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	APlayerCharacter* PlayerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	float Stamina;
public:
	// Variable Setter
	UFUNCTION(BlueprintCallable)
	void SetPlayerCharacter(APlayerCharacter* OutPlayerCharacter);
	UFUNCTION(BlueprintCallable)
	void SetHealth(float OutHealth);
	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float OutMaxHealth);
	UFUNCTION(BlueprintCallable)
	void SetStamina(float OutStamina);
	UFUNCTION(BlueprintCallable)
	void SetMaxStamina(float OutMaxStamina);
	
	//Widget Getter
	UFUNCTION()
	UProgressBar* GetProgressBar_Health() const { return ProgressBar_Health; }
	UFUNCTION()
	UProgressBar* GetProgressBar_Stamina() const { return ProgressBar_Stamina; }
	UFUNCTION(BlueprintCallable)
	UTextBlock* GetText_CurrentAmmo() const { return Text_CurrentAmmo; }
	UFUNCTION(BlueprintCallable)
	UTextBlock* GetText_Ammo() const { return Text_Ammo; }

	//Widget Setter
	UFUNCTION(BlueprintCallable)
	void SetProgressBar_Health(float OutHealth);
	UFUNCTION(BlueprintCallable)
	void SetProgressBar_Stamina(float OutStamina);
	UFUNCTION(BlueprintCallable)
	void SetText_CurrentAmmo(int32 CurrentAmmo);
	UFUNCTION(BlueprintCallable)
	void SetText_Ammo(int32 Ammo);
	UFUNCTION(BlueprintCallable)
	void CurrentWeaponUIInit();

	UFUNCTION(BlueprintCallable)
	FString SetBulletTypeTextBlock(EBulletType OutEBulletType);
};




