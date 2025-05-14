// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GA/GA_Throw_Grenade.h"

#include "Character/PlayerCharacter.h"
#include "Component/Movement/PlayerMovementComponent.h"

UAnimMontage* UGA_Throw_Grenade::selectCurrentMontage_isProne_Throw(UAnimMontage* Throw_TrueMontage,
	UAnimMontage* Throw_falseMontage)
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(PlayerCharacter->GetMovementComponent());

	if (MovementComponent->RequestToStartProne)
	{
		return Throw_TrueMontage;
	}
	return Throw_falseMontage;
}
