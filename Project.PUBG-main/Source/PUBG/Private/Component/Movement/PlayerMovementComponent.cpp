// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Movement/PlayerMovementComponent.h"
#include "Character/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "MaterialHLSLTree.h"
#include "Net/UnrealNetwork.h"
#include "BaseLibrary/BaseFunctionLibrary.h"
#include "Compression/lz4hc.h"

UPlayerMovementComponent::UPlayerMovementComponent()
{
	SetIsReplicatedByDefault(true);
}

void UPlayerMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UPlayerMovementComponent, RequestToStartProne);
	DOREPLIFETIME(UPlayerMovementComponent, LeaningValue);
	DOREPLIFETIME(UPlayerMovementComponent, RequestToBackMovement);
	DOREPLIFETIME(UPlayerMovementComponent, RequestParachute);
	DOREPLIFETIME(UPlayerMovementComponent, RequestFreefalling);
	
}

float UPlayerMovementComponent::GetMaxSpeed() const
{                                                                                                                             
	APlayerCharacter* Owner = Cast<APlayerCharacter>(GetOwner());
	float BaseSpeed = Owner->GetMoveSpeed();

	// if(Owner->IsSprint)
	// 	sprint = 2.0f;
	

	// 뒤로 가기 할 때 는
	// if(Owner->IsBack)
	// {
	//     BaseSpeed -= 100.f; // 이런식으로 빼면 됨
	// }
	
	if (RequestToStartProne)
	{
		BaseSpeed -= SubProne; // PRONE : 350-240 = 110;
	}
	else if (IsCrouching())
	{
		BaseSpeed -= Subcrouch; // crouch : 350-100 = 250;
	}

	if (RequestToStartSprinting)
	{
		if (!RequestToStartProne)
		{
			if (!RequestToBackMovement)
			{
				BaseSpeed += AddSHIFTSprint;				
			}	
		}
		 //STAND+SHIFT : 350+150 = 500;
	}
	else if (RequestToStartWalking)
	{
		if (IsCrouching())
		{
			BaseSpeed -= SubCTRLCrouch;
		}
		else if (RequestToStartProne)
		{
			BaseSpeed -= SubCTRLProne;
		}
		else
		{
			BaseSpeed -= SubCTRLWalk;//STAND-CTRL : 350 - 200 = 150;	
		}
		
	}
	else if (RequestFreefalling&&MovementMode == MOVE_Falling)
	{
		BaseSpeed += 700.f;
	}
	else if (RequestParachute&&MovementMode ==MOVE_Falling)
	{
		BaseSpeed += 1000.f;
	}
	//UE_LOG(LogTemp,Warning,TEXT("BaseSpeed:%f"), BaseSpeed);
	return BaseSpeed; //350
}
// 걷기 350 
// 수그리기 250
// 눕기 110
//
// 쉬프트+걷기 500 
// 쉬프트+수그리기 400 
//
// 컨트롤+걷기 150;
// 컨트롤+수그리기 80
// 컨트롤+눕기 50


void UPlayerMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	
	//Flags 매개변수에는 저장된 이동에 저장된 압축된 입력 플래그가 포함되어 있습니다.
	//UpdateFromCompressed 플래그는 단순히 저장된 이동의 플래그를 이동 구성요소에 복사합니다.
	//기본적으로 이동 구성요소를 이동이 이루어졌을 때의 상태로 재설정하므로 거기에서 시뮬레이션할 수 있습니다.
	RequestToStartSprinting = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	RequestToStartWalking = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
	RequestToStartProne = (Flags & FSavedMove_Character::FLAG_Custom_2) != 0;	
	RequestToBackMovement = (Flags & FSavedMove_Character::FLAG_Custom_3) != 0;
	RequestFreefalling = (Flags & FSavedMove_Character::FLAG_Reserved_1) != 0;
	RequestParachute = (Flags & FSavedMove_Character::FLAG_Reserved_2) != 0;
}

void UPlayerMovementComponent::FGDSavedMove::Clear()
{
	Super::Clear();

	SavedRequestToStartSprinting = false;
	SavedRequestToStartWalking = false;
	SavedRequestToStartProne = false;
	SavedRequestToBackMovement = false;
	SavedRequestFreefalling = false;
	SavedRequestParachute = false;

}

uint8 UPlayerMovementComponent::FGDSavedMove::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (SavedRequestToStartSprinting)
	{
		Result |= FLAG_Custom_0;
	}

	if (SavedRequestToStartWalking)
	{
		Result |= FLAG_Custom_1;
	}
	
	if (SavedRequestToStartProne)
	{
		Result |= FLAG_Custom_2;
	}
	
	if (SavedRequestToBackMovement)
	{
		Result |= FLAG_Custom_3;
	}
	if (SavedRequestFreefalling)
	{
		Result |= FLAG_Reserved_1;  // 4번 비트 설정
	}

	// SavedRequestParachute에 대해 FLAG_Custom_5 사용
	if (SavedRequestParachute)
	{
		Result |= FLAG_Reserved_2;   // 5번 비트 설정
	}
	
	return Result;
}

bool UPlayerMovementComponent::FGDSavedMove::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character,
	float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.
	if (SavedRequestToStartSprinting != ((FGDSavedMove*)&NewMove)->SavedRequestToStartSprinting)
	{
		return false;
	}

	if (SavedRequestToStartWalking != ((FGDSavedMove*)&NewMove)->SavedRequestToStartWalking)
	{
		return false;
	}
	
	if (SavedRequestToStartProne != ((FGDSavedMove*)&NewMove)->SavedRequestToStartProne)
	{
		return false;
	}
	
	if (SavedRequestToBackMovement != ((FGDSavedMove*)&NewMove)->SavedRequestToBackMovement)
	{
		return false;
	}
	if (SavedRequestFreefalling != ((FGDSavedMove*)&NewMove)->SavedRequestFreefalling)
	{
		return false;
	}
	if (SavedRequestParachute != ((FGDSavedMove*)&NewMove)->SavedRequestParachute)
	{
		return false;
	}
	
	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UPlayerMovementComponent::FGDSavedMove::SetMoveFor(ACharacter* Character, float InDeltaTime,
	FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UPlayerMovementComponent* CharacterMovement = Cast<UPlayerMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		SavedRequestToStartSprinting = CharacterMovement->RequestToStartSprinting;
		SavedRequestToStartWalking = CharacterMovement->RequestToStartWalking;
		SavedRequestToStartProne = CharacterMovement->RequestToStartProne;
		SavedRequestToBackMovement = CharacterMovement->RequestToBackMovement;
		SavedRequestFreefalling = CharacterMovement->RequestFreefalling;
		SavedRequestParachute = CharacterMovement->RequestParachute;
	}
}

void UPlayerMovementComponent::FGDSavedMove::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UPlayerMovementComponent* CharacterMovement = Cast<UPlayerMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
	}
}


UPlayerMovementComponent::FGDNetworkPredictionData_Client::FGDNetworkPredictionData_Client(const UCharacterMovementComponent & ClientMovement)
	: Super(ClientMovement)
{
	
}

FSavedMovePtr UPlayerMovementComponent::FGDNetworkPredictionData_Client::AllocateNewMove()
{
	return FSavedMovePtr(new FGDSavedMove());
}

class FNetworkPredictionData_Client* UPlayerMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);

	if (!ClientPredictionData)
	{
		UPlayerMovementComponent* MutableThis = const_cast<UPlayerMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FGDNetworkPredictionData_Client(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UPlayerMovementComponent::StartSprinting()
{
	if (!RequestToStartProne)
	{
		RequestToStartSprinting = true;
		RequestToStartWalking = false;
	}
}

void UPlayerMovementComponent::StopSprinting()
{
	RequestToStartSprinting = false;
}

void UPlayerMovementComponent::StartWalking()
{
	RequestToStartWalking = true;
	RequestToStartSprinting = false;
}

void UPlayerMovementComponent::StopWalking()
{
	RequestToStartWalking = false;		
}

void UPlayerMovementComponent::StartProne()
{
	RequestToStartProne = true;
	
}

void UPlayerMovementComponent::StopProne()
{
	RequestToStartProne = false;
	
}

void UPlayerMovementComponent::StartLeaning(float Value)
{
	LeaningValue = Value;
}

void UPlayerMovementComponent::StopLeaning()
{
	LeaningValue = 0;
}

void UPlayerMovementComponent::StartBackMovement()
{
	RequestToBackMovement = true;
}

void UPlayerMovementComponent::StopBackMovement()
{
	RequestToBackMovement = false;
}

void UPlayerMovementComponent::StartFreeFalling()
{
	RequestFreefalling = true;
}

void UPlayerMovementComponent::StopFreeFalling()
{
	RequestFreefalling = false;
}

void UPlayerMovementComponent::StartParachute()
{
	RequestParachute = true;
}

void UPlayerMovementComponent::StopParachute()
{
	RequestParachute = false;
}






