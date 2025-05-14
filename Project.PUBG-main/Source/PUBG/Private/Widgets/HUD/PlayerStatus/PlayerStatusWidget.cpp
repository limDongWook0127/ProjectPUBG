// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/PlayerStatus/PlayerStatusWidget.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Character/PlayerCharacter.h"
#include "Component/Equipped/EquippedComponent.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PlayerState/BasePlayerState.h"

void UPlayerStatusWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPlayerStatusWidget::NativePreConstruct PlayerCharacter None"));

		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("UPlayerStatusWidget::NativePreConstruct PlayerCharacter"));

}

void UPlayerStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!PlayerCharacter)
	{
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningPlayer()->GetCharacter()))
		{			
			SetPlayerCharacter(Player);
		}
	}
}

void UPlayerStatusWidget::SetPlayerCharacter(APlayerCharacter* OutPlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("UPlayerStatusWidget::SetPlayerCharacter %s"), *OutPlayerCharacter->GetName());
	PlayerCharacter = OutPlayerCharacter;

	PlayerCharacter->GetEquippedComponent()->CurrentWeaponDelegate.BindUObject(this, &UPlayerStatusWidget::CurrentWeaponUIInit);
}

void UPlayerStatusWidget::CurrentWeaponUIInit()
{
	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	UEquippedComponent* EquippedComponent = PlayerCharacter->GetEquippedComponent();

	if (!IsValid(EquippedComponent))
	{
		return;
	}

	if (EquippedComponent->GetCurrentWeapon())
	{
		if (AGun_Base* CurrentGun = EquippedComponent->GetCurrentWeapon_GunBase())
		{
			UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();

			FWeaponData WeaponData = CurrentGun->GetWeaponDataAsset();
			FString BulletName_String = SetBulletTypeTextBlock(WeaponData.BulletType);
			FName BulletName_Name = FName(BulletName_String);
			int32 BulletIndex = InventoryComponent->FindItemSlot(BulletName_Name);
		
			InventoryComponent->RemainAmmoDelegate.BindUObject(this, &UPlayerStatusWidget::SetText_Ammo);

			UE_LOG(LogTemp, Warning, TEXT("Character used for binding: %s"), *PlayerCharacter->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Character used for binding: %p"), PlayerCharacter);
			UE_LOG(LogTemp, Warning, TEXT("InventoryComponent used for binding: %p"), InventoryComponent);

			CurrentGun->CurrentAmmoDelegate.BindUObject(this, &UPlayerStatusWidget::SetText_CurrentAmmo);
		
			SetText_CurrentAmmo(CurrentGun->GetBulletArmo());

			if (BulletIndex >= 0)
			{
				SetText_Ammo(InventoryComponent->GetContent()[BulletIndex].Quantity);
			}
			else
			{
				SetText_Ammo(0);
			}
		}
	}
	else
	{
		Text_CurrentAmmo->SetVisibility(ESlateVisibility::Collapsed);
		Text_Ammo->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FString UPlayerStatusWidget::SetBulletTypeTextBlock(EBulletType OutEBulletType)
{
	switch (OutEBulletType)
	{
	case EBulletType::B_7_62:
		return TEXT("7.62mm");
	case EBulletType::B_5_56:
		return TEXT("5.56mm");
	case EBulletType::B_9:
		return TEXT("9mm");
	case EBulletType::B_12G:
		return TEXT("12G");
	default:
		return TEXT("None");
	}
}

void UPlayerStatusWidget::SetHealth(float OutHealth)
{
	//UE_LOG(LogTemp, Warning, TEXT("PlayerStatusWidget::SetHealth"));
	Health = OutHealth;

	SetProgressBar_Health(OutHealth);
}

void UPlayerStatusWidget::SetMaxHealth(float OutMaxHealth)
{
	MaxHealth = OutMaxHealth;

	SetProgressBar_Health(OutMaxHealth);
}

void UPlayerStatusWidget::SetStamina(float OutStamina)
{
	Stamina = OutStamina;

	SetProgressBar_Stamina(OutStamina);
}

void UPlayerStatusWidget::SetMaxStamina(float OutMaxStamina)
{
	MaxStamina = OutMaxStamina;

	SetProgressBar_Stamina(OutMaxStamina);
}

void UPlayerStatusWidget::SetProgressBar_Health(float OutHealth)
{
	if (OutHealth < 50.f)
	{
		ProgressBar_Health->SetFillColorAndOpacity(FLinearColor(1.0f, 0.759928f, 0.348958f, 0.8f));
	}
	
	if (OutHealth < 20.f)
	{
		ProgressBar_Health->SetFillColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.008303f, 0.8f));
	}
	
	if (OutHealth < 10.f)
	{
		PlayAnimation(Anim_ProgressBar_Health,0.0f,0.0f);
	}
	else
	{
		StopAnimation(Anim_ProgressBar_Health);
	}
	
	ProgressBar_Health->SetPercent(OutHealth / 100.F);
}


void UPlayerStatusWidget::SetProgressBar_Stamina(float OutStamina)
{
	ProgressBar_Stamina->SetPercent(OutStamina / 100.F);
}

void UPlayerStatusWidget::SetText_CurrentAmmo(int32 CurrentAmmo)
{
	UE_LOG(LogTemp, Warning, TEXT("UPlayerStatusWidget::SetText_CurrentAmmo"));
	UE_LOG(LogTemp, Warning, TEXT("UPlayerStatusWidget::SetText_CurrentAmmo %d"), CurrentAmmo);

	Text_CurrentAmmo->SetVisibility(ESlateVisibility::Visible);
	Text_Ammo->SetVisibility(ESlateVisibility::Visible);
	FString CurrentAmmo_String = FString::FromInt(CurrentAmmo);
	FText CurrentAmmo_Text = FText::FromString(CurrentAmmo_String);
	Text_CurrentAmmo->SetText(CurrentAmmo_Text);
}

void UPlayerStatusWidget::SetText_Ammo(int32 Ammo)
{
	UE_LOG(LogTemp, Warning, TEXT("UPlayerStatusWidget::SetText_Ammo %d"), Ammo);
	FString Ammo_String = FString::FromInt(Ammo);
	FText Ammo_Text = FText::FromString(Ammo_String);
	Text_Ammo->SetText(Ammo_Text);
}


