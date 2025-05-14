// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/GA_RightShouldering.h"
#include "Component/PUBGSpringArmComponent.h"
#include "Character/PlayerCharacter.h"
#include "BaseLibrary/BaseFunctionLibrary.h"


UGA_RightShouldering::UGA_RightShouldering()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_RightShouldering::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	PlayerCameraBoom = GetPlayerCharacterFromActorInfo()->GetCameraBoom();
	if (GetPlayerCharacterFromActorInfo()->GetCameraMode() == PlayerCameraMode::FPPCamera)
	{
		if (UBaseFunctionLibrary::NativeActorHasTag(GetPlayerCharacterFromActorInfo(),
		                                            FGameplayTag::RequestGameplayTag(FName("InputTag.LeftLeaning"))))
		{
			FVector OffsetDelta = FVector(110.f, -40.f, 0.f);
			float Duration = 0.2f;

			PlayerCameraBoom->TimelineAddOffset(OffsetDelta, Duration);
			CameraLeftMoved = true;
			return;
		}

		//PlayerCameraBoom->SetWanstReversePlaying(false);
		FVector OffsetDelta = FVector(110.f, 40.f, 0.f);
		float Duration = 0.2f;
		CameraLeftMoved = false;
		PlayerCameraBoom->TimelineAddOffset(OffsetDelta, Duration);
	}

	// if ( UBaseFunctionLibrary::NativeActorHasTag(GetPlayerCharacterFromActorInfo(), FGameplayTag::(FName("InputTag.LeftLeaning"))))
	// {
	// }
	// // PlayerCameraBoom = GetPlayerCharacterFromActorInfo()->GetCameraBoom();
	// // if (PlayerCameraBoom&&bIsOnce)
	// // {
	// // 	
	// // 	InitialOffset = PlayerCameraBoom->SocketOffset;
	// // }
	// // UE_LOG(LogTemp, Warning, TEXT("Camera"));
	// // if (GetPlayerCharacterFromActorInfo()->GetCameraMode() == PlayerCameraMode::FPPCamera)
	// // {
	// // 	GetWorld()->GetTimerManager().SetTimer(CameraBoomTimerHandle,
	// // 	this, &UGA_Shouldering::UpdateCameraBoomOffset, 0.05f, true);
	// // 	
	// // }
}

// void UGA_Shouldering::UpdateCameraBoomOffset()
// {
// 	// CurrentOffset = PlayerCameraBoom->SocketOffset;
// 	// FVector NewOffset = FMath::VInterpTo(CurrentOffset, InitialOffset+AddOffset, GetWorld()->GetDeltaSeconds(), 30.f);  // 속도 조절
// 	// PlayerCameraBoom->SocketOffset = NewOffset;
// 	//
// 	// // 목표 위치에 도달하면 타이머 종료
// 	// if (FVector::Dist(NewOffset, CurrentOffset+AddOffset) < 1.0f)
// 	// {
// 	// 	GetWorld()->GetTimerManager().ClearTimer(CameraBoomTimerHandle);
// 	// }
// }

void UGA_RightShouldering::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                      bool bWasCancelled)
{
	if (GetPlayerCharacterFromActorInfo()->GetCameraMode() == PlayerCameraMode::FPPCamera
		&& CameraLeftMoved)
	{
		FVector OffsetDelta = FVector(-110.f, 40.f, 0.f);
		float Duration = 0.2f;

		PlayerCameraBoom->TimelineAddOffset(OffsetDelta, Duration);
		CameraLeftMoved = false;
	}
	else if (GetPlayerCharacterFromActorInfo()->GetCameraMode() == PlayerCameraMode::FPPCamera && 
			!CameraLeftMoved)
	{
		//PlayerCameraBoom->SetWanstReversePlaying(true);
		FVector OffsetDelta = FVector(-110.f, -40.f, 0.f);
		float Duration = 0.2f;

		PlayerCameraBoom->TimelineAddOffset(OffsetDelta, Duration);
		CameraLeftMoved = false;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
// // PlayerCameraBoom = GetPlayerCharacterFromActorInfo()->GetCameraBoom();
// if (PlayerCameraBoom)
// {
// 	InitialOffset = PlayerCameraBoom->SocketOffset;
// }
// UE_LOG(LogTemp, Warning, TEXT("end"));
// if (GetPlayerCharacterFromActorInfo()->GetCameraMode() == PlayerCameraMode::FPPCamera)
// {
// 	GetWorld()->GetTimerManager().SetTimer(CameraBoomTimerHandle,
// 	this, &UGA_Shouldering::ReturnCameraBoomOffset, 0.05f, true);
// 	
// }


// void UGA_Shouldering::ReturnCameraBoomOffset()
// {
// 	// FVector NewOffset = FMath::VInterpTo(CurrentOffset, InitialOffset+SubOffset, GetWorld()->GetDeltaSeconds(), 30.f);  // 속도 조절
// 	//
// 	// PlayerCameraBoom->SocketOffset = NewOffset;
// 	//
// 	// // 목표 위치에 도달하면 타이머 종료
// 	// if (FVector::Dist(NewOffset, CurrentOffset+SubOffset) < 1.0f)
// 	// {
// 	// 	GetWorld()->GetTimerManager().ClearTimer(CameraBoomTimerHandle);
// 	// }
// }
