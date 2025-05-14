// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle/Vehicle_UAZ.h"
#include "Character/PlayerCharacter.h"
#include "AnimInstance/PlayerAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Component/PUBGSpringArmComponent.h"
#include "Components/ArrowComponent.h"

void AVehicle_UAZ::InteractWith_Implementation(APlayerCharacter* Character)
{
	
	//IInteractInterface::InteractWith_Implementation(Character);
	TSubclassOf<UPlayerAnimInstance> UAZAnimLayer = LoadClass<UPlayerAnimInstance>(nullptr, TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprint/Character/Animation/AnimLayer/Character_VehicleAnimLayer/ABP_UAZ.ABP_UAZ'"));
	Character->Server_SetAnimLayer(UAZAnimLayer);
	Super::InteractWith_Implementation(Character);

	
}

FText AVehicle_UAZ::LookAt()
{
	return Super::LookAt();
}
