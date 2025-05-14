// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "Components/TimelineComponent.h"
#include "Weapon/DataTable/DT_Weapon.h"
#include "Weapon/Guns/Gun_Base.h"
#include "GA_Weapon_Fire.generated.h"


class AWeapon_Base;
class AGun_Base;

/**
 * 
 */
UCLASS()
class PUBG_API UGA_Weapon_Fire : public UPlayerGameplayAbility
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FWeaponData WeaponData;

	UPROPERTY(EditAnywhere)
	AWeapon_Base* Weapon_Base;
	
	UPROPERTY(EditAnywhere)
	AGun_Base* Gun_Base;

	UTimelineComponent* Tiemline;
	
	UCurveFloat* FloatCurve;

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float RecoilAmount; // 반동 강도 (상하)

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float YawRecoilAmount; // 좌우 반동 강도 (약간만 적용)

	UPROPERTY(EditAnywhere, Category = "Recoil")
	float RecoilDuration;  // 반동 지속 시간

public:
	UFUNCTION(BlueprintCallable, Category = "GA_weapon")
	void SetDefault();

	UFUNCTION(BlueprintCallable, Category = "GA_weapon")
	FVector GetVelocityVector();

	UFUNCTION(BlueprintCallable, Category = "GA_weapon")
	FWeaponData GetWeaponData() const {return WeaponData;}

	UFUNCTION(BlueprintPure, Category = "GA_weapon")
	FVector GetProjectileSocketLocation() const {return Gun_Base->GetWeaponMagSocketLocation();}

	UFUNCTION(BlueprintPure, Category = "GA_Weapon")
	UAnimMontage* PlayCurrentPose_recoilMontage(UAnimMontage* StandMontage, UAnimMontage* ProneMontage, UAnimMontage* CrouchMontage);
	
	UFUNCTION(BlueprintCallable, Category = "GA_weapon")
	void CheckZoomIn();
	
	UFUNCTION(BlueprintCallable, Category = "GA_weapon")
	void CheckZoomOut();
	
	// timeline
	UFUNCTION(BlueprintCallable)
	void ApplyRecoil(float _RecoilAmount, float _RecoilDuration, float _YawRecoilAmount);
	
};
