// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Guns/Rifles/AK_47.h"

AAK_47::AAK_47()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh (TEXT("/Game/PUBGAsset/Weapon_Assets/Weapon/AK/AK.AK"));
	WeaponSkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh.Object);
}
