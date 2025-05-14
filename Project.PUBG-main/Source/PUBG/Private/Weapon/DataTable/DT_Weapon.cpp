// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/DataTable/DT_Weapon.h"

FInstallable_parts::FInstallable_parts()
{
	Scope = false;
	Mag = false;
	Muzzle = false;
	Grip = false;
	Loops = false;
}

FRecoilMontage::FRecoilMontage()
{
	Stand_RecoilMontage = nullptr;
	Prone_RecoilMontage = nullptr;
	Crouch_RecoilMontage = nullptr;
}

FWeaponData::FWeaponData()
{
	Type = EGunType::None;
	GunName = "None";
	BulletType = EBulletType::None;
	Gun_Damage = 0;
	Armo = 0;
	Bullet_Speed = 0;
	Bullet_crossSection = 0;
	bullet_Mass = 0;
	Bullet_DragCoefficiency = 0;

	Min_XRecoil = 0;
	Max_XRecoil = 0;
	Min_YRecoil = 0;
	Max_YRecoil = 0;
	
}
