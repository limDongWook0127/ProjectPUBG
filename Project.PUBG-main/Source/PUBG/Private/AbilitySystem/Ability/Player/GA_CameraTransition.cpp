// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/GA_CameraTransition.h"

#include "Camera/CameraComponent.h"
#include "Character/PlayerCharacter.h"

void UGA_CameraTransition::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	SetCanBeCanceled(false);
	UCameraComponent* FollowCamera = GetPlayerCharacterFromActorInfo()->GetFollowCamera();
	UCameraComponent* FirstPersonCamera = GetPlayerCharacterFromActorInfo()->GetFirstPersonCamera();

	switch (GetPlayerCharacterFromActorInfo()->GetCameraMode())
	{
	case PlayerCameraMode::TPPCamera:
		FirstPersonCamera->SetActive(false);
		FollowCamera->SetActive(true);
		GetPlayerCharacterFromActorInfo()->SetCurrentCamera(FollowCamera);
		FirstPersonCamera->bUsePawnControlRotation = false;
		GetPlayerCharacterFromActorInfo()->SetCameraMode(PlayerCameraMode::FPPCamera);
		//UE_LOG(LogTemp, Warning, TEXT("DEActive Ability"));
		return;

	case PlayerCameraMode::FPPCamera:
		FollowCamera->SetActive(false);
		FirstPersonCamera->SetActive(true);
		GetPlayerCharacterFromActorInfo()->SetCurrentCamera(FirstPersonCamera);
		FirstPersonCamera->bUsePawnControlRotation = true;
		GetPlayerCharacterFromActorInfo()->SetCameraMode(PlayerCameraMode::TPPCamera);
		//UE_LOG(LogTemp, Warning, TEXT("Active Ability"));
		return;
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
}
