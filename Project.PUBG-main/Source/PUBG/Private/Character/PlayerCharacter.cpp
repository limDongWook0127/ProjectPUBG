// Fill out your copyright notice in the Description page of Project Settings.

// 캐릭터
#include "Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ActorComponent.h"
#include "PlayerState/BasePlayerState.h"
#include "Controller/BasePlayerController.h"

#include "Component/Movement/PlayerMovementComponent.h"

// 카메라
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/PUBGSpringArmComponent.h"

// 애니메이션
#include "AnimInstance/PlayerAnimInstance.h"

// 입력값 
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/Input/BaseInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAsset/Input/DataAsset_InputConfig.h"

// 어빌리티
#include "AbilitySystem//BaseGameplayTag.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "DataAsset/Startup/DataAsset_StartupBase.h"
#include "BaseLibrary/BaseFunctionLibrary.h"
#include "Component/NearArea/NearComponent.h"
#include "Interface/InteractInterface.h"

//자기장
#include "Components/PostProcessComponent.h"

//

#include "Component/Equipped/EquippedComponent.h"

#include "Component/ItemData/ItemDataComponent.h"
#include "Item/ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/Inventory/InventoryWidget.h"
//Weapon
#include "Weapon/Weapon_Base.h"
#include "BaseLibrary/BaseStructType.h"
#include "Widgets/DisplayMessage/DisplayMessageItemWidget.h"
#include "Widgets/HUD/HudWidget.h"

APlayerCharacter::APlayerCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//bReplicates = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CameraBoom = CreateDefaultSubobject<UPUBGSpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, UPUBGSpringArmComponent::SocketName);
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetMesh(),TEXT("camera_fppSocket"));
	FirstPersonCamera->Deactivate();
	CameraMode = PlayerCameraMode::FPPCamera;

	PunchCapsuleLeft = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PunchCapsuleLeft"));
	PunchCapsuleRight = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PunchCapsuleRight"));

	// 메쉬 부착
	//UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPlayerMeshType"), true);
	UEnum* EnumPtr = StaticEnum<EPlayerMeshType>();
	if (EnumPtr)
	{
		for (int32 i = 0; i < EnumPtr->GetMaxEnumValue(); ++i)
		{
			FString EnumName = EnumPtr->GetNameStringByIndex(i);
			FName SubobjectName(*EnumName);

			// Skeletal Mesh Component 생성
			USkeletalMeshComponent* NewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(SubobjectName);
			NewMesh->SetupAttachment(GetMesh());
			NewMesh->SetIsReplicated(true);

			// Enum 값으로 맵핑
			EPlayerMeshType EnumValue = static_cast<EPlayerMeshType>(i);
			CharacterEquipmentMap.Add(EnumValue, NewMesh);
		}
	}

	// 무브먼트 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	//BaseAttributeSet->SetMoveSpeed(300.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	//GetCharacterMovement()->MaxWalkSpeedCrouched = 200.0f;

	bIsProne = false;
	bAnimationIsPlaying = false;

	// 이준수 
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	NearComponent = CreateDefaultSubobject<UNearComponent>(TEXT("Near"));
	EquippedComponent = CreateDefaultSubobject<UEquippedComponent>(TEXT("EquippedComponent"));

	DetectionItem = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectionItem"));
	DetectionItem->SetupAttachment(GetRootComponent());
	DetectionItem->SetBoxExtent(FVector(80.0f));
	//DetectionItem->SetVisibility(true);

	//자기장
	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PlayerPostProcess"));
	PostProcessComponent->SetupAttachment(RootComponent);

	DeadTag = BaseGameplayTag::Player_State_Dead;

	SetCurrentCamera(FollowCamera);
	IsZoom = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 이준수
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnComponentBeginOverlap);
	
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnComponentEndOverlap);
	DetectionItem->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnDetectionItemBeginOverlap);
	DetectionItem->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnDetectionItemEndOverlap);

	DetectionItem->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	//- 무기 스폰 후 등에 Attach (Server)
	// AActor* OwnerActor = GetOwner();
	// if (OwnerActor && OwnerActor->HasAuthority())
	// {
	// UClass* BlueprintClass = LoadClass<AActor>(nullptr, TEXT("/Game/Blueprint/Weapon/BP_AK.BP_AK"));
	// USkeletalMeshComponent* MeshComponent = Cast<APlayerCharacter>(GetOwner())->GetMesh();
	// if (BlueprintClass && MeshComponent)
	// {
	// 	// 스폰 위치와 회전값 설정
	// 	FVector SpawnLocation = MeshComponent->GetSocketLocation(FName("slot_primarySocket"));
	// 	FRotator SpawnRotation = MeshComponent->GetSocketRotation(FName("slot_primarySocket"));
	//
	// 	// 스폰 파라미터 설정
	// 	FActorSpawnParameters SpawnParams;
	// 	SpawnParams.Owner = GetOwner(); // 소유자 설정
	// 	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//
	// 	// 액터 스폰
	// 	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(BlueprintClass, SpawnLocation, SpawnRotation, SpawnParams);
	//
	// 	if (SpawnedActor)
	// 	{
	// 		InventoryComponent->SetPrimarySlotWeapon(Cast<AWeapon_Base>(SpawnedActor));
	//
	// 		if (InventoryComponent->GetPrimarySlotWeapon())
	// 		{
	// 			// 스폰된 액터를 Attach
	// 			FName SocketName = TEXT("slot_primarySocket"); // 소켓 이름 (예: 등에 해당하는 소켓)
	// 			InventoryComponent->GetPrimarySlotWeapon()->AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	// 		}
	// 	}
	// }
	//
	if (HasAuthority())
	{
		SetOwner(this); // 서버에서 PlayerCharacter를 소유자로 설정
	}
	else
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			SetOwner(PlayerController); // 클라이언트에서는 플레이어 컨트롤러를 소유자로 설정
		}
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsLocallyControlled())
	{
		UpdateRotationValues();
	}
}

USkeletalMeshComponent* APlayerCharacter::FindMeshComponent(EPlayerMeshType PlayerMeshType)
{
	return *CharacterEquipmentMap.Find(PlayerMeshType);
}

void APlayerCharacter::SetMeshComponent(EPlayerMeshType PlayerMeshType, USkeletalMesh* SkeletalMesh)
{
	if (HasAuthority())
	{
		//UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::SetMeshComponent = Execute Server"));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::SetMeshComponent = Execute Client"));
	}
	//UE_LOG(LogTemp, Warning, TEXT("SetMeshComponent =  %s"), *SkeletalMesh->GetName());

	if (USkeletalMeshComponent* SkeletalMeshComponent = FindMeshComponent(PlayerMeshType))
	{
		//UE_LOG(LogTemp, Warning, TEXT("SetMeshComponent : SkeletalMeshComponent = %s"), *SkeletalMeshComponent->GetName());
		SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
	}
}

void APlayerCharacter::Multicast_SetMeshComponent_Implementation(EPlayerMeshType PlayerMeshType,
                                                                 USkeletalMesh* SkeletalMesh)
{
	//UE_LOG(LogTemp, Warning, TEXT("Multicast_SetMeshComponent_Implementation"));
	//UE_LOG(LogTemp, Warning, TEXT("Multicast_SetMeshComponent_Implementation : %s"), *SkeletalMesh->GetName());
	SetMeshComponent(PlayerMeshType, SkeletalMesh);
}

void APlayerCharacter::Client_SetMeshComponent_Implementation(EPlayerMeshType PlayerMeshType,
                                                              USkeletalMesh* SkeletalMesh)
{
	SetMeshComponent(PlayerMeshType, SkeletalMesh);
}

void APlayerCharacter::CallCheckZoomAbility()
{
	if (IsZoom)
	{
		GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
			FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Player.Ability.Weapon.Zoom")));
	}
}

void APlayerCharacter::CallCheckZoomOutAbility()
{
	if (!IsZoom)
	{
		GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
			FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Player.Ability.Weapon.Zoom")));
	}
}


void APlayerCharacter::UpdateRotationValues_Implementation()
{
	if (HasAuthority())
	{
		FRotator AimRotation = GetBaseAimRotation();
		FRotator ActorRotation = GetActorRotation();
		NormalDeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation);
		Yaw = NormalDeltaRotator.Yaw;
		// UE_LOG(LogTemp, Log, TEXT("Server - Yaw: %f"), Yaw);
		//Pitch = NormalDeltaRotator.Pitch;
	}
}

bool APlayerCharacter::UpdateRotationValues_Validate()
{
	return true;
}

void APlayerCharacter::OnRep_RotationValues()
{
	if (NormalDeltaRotator.Yaw >= 180.0f)
	{
		NormalDeltaRotator.Yaw -= 360.0f;
	}
	else if (NormalDeltaRotator.Yaw <= -180.0f)
	{
		NormalDeltaRotator.Yaw += 360.0f;
	}
	Yaw = NormalDeltaRotator.Yaw;
	// UE_LOG(LogTemp, Log, TEXT("OnRep Rotation - Yaw: %f"), Yaw);
	//Pitch = DeltaRotation.Pitch;
}


void APlayerCharacter::Server_SetAnimLayer_Implementation(TSubclassOf<UPlayerAnimInstance> PlayerAnimInstance)
{
	NetMulticast_SetAnimLayer(PlayerAnimInstance);
}

void APlayerCharacter::NetMulticast_SetAnimLayer_Implementation(TSubclassOf<UPlayerAnimInstance> PlayerAnimInstance)
{
	GetMesh()->LinkAnimClassLayers(PlayerAnimInstance);
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UBaseInputComponent* BaseInputComponent = CastChecked<UBaseInputComponent>(PlayerInputComponent);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Move,
	                                          ETriggerEvent::Triggered, this,
	                                          &APlayerCharacter::Input_Move_Implementation);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Move,
	                                          ETriggerEvent::Completed, this, &APlayerCharacter::Input_MoveReleased);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Look,
	                                          ETriggerEvent::Triggered, this, &APlayerCharacter::Input_Look);
	BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Jump,
	                                          ETriggerEvent::Started, this, &APlayerCharacter::Input_Jump);
	//BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Crouch,
	//                                          ETriggerEvent::Started, this, &APlayerCharacter::Input_Crouch);
	//BaseInputComponent->BindNativeInputAction(InputConfigDataAsset, BaseGameplayTag::InputTag_Prone,
	//                                          ETriggerEvent::Started, this, &APlayerCharacter::Input_Prone);

	BaseInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &APlayerCharacter::Input_AbilityInputPressed,
	                                           &APlayerCharacter::Input_AbilityInputReleased);

	BaseInputComponent->BindAbilityInputAction_Tab(InputConfigDataAsset, this,
	                                               &APlayerCharacter::Input_AbilityInputPressed);
}

void APlayerCharacter::Input_Move_Implementation(const FInputActionValue& InputActionValue)
{
	// 현재 MovementMode 확인
	EMovementMode CurrentMovementMode = GetCharacterMovement()->MovementMode;
	//UE_LOG(LogTemp, Warning, TEXT("CurrentMovementMode: %s"), *UEnum::GetValueAsString(CurrentMovementMode));

	Input_Move(InputActionValue);
	if (!OntheParachute)
	{
		UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
		bUseControllerRotationYaw = false;
		UBaseAbilitySystemComponent* AbilitySystemComponent = Cast<
			UBaseAbilitySystemComponent>(GetAbilitySystemComponent()); // 턴 중 Input_Move 들어오면 캔슬 
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->TryCancelAbilityByTag(BaseGameplayTag::Player_Ability_Turn);
		}
		const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

		if (IsSwimming)
		{
			const FRotator MovementRotation = FRotator(Controller->GetControlRotation().Pitch,
			                                           Controller->GetControlRotation().Yaw, 0.f);

			// 이동 방향 계산: Forward (전방) 방향 계산
			const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			UE_LOG(LogTemp, Warning, TEXT("ForwardDirection : %s"), *ForwardDirection.ToString());
			// 이동 방향 계산: Right (우측) 방향 계산
			const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

			// 수평과 수직 이동을 반영하여 방향 계산
			FVector MoveDirection = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;

			// 계산된 방향으로 이동
			AddMovementInput(MoveDirection, 1.f);
		}

		if (Controller)
		{
			const FRotator TargetRotation = FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f); // 목표 회전
			FRotator CurrentRotation = GetActorRotation(); // 현재 회전
			// 부드럽게 회전 (보간을 통해)
			FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, TargetRotation,
			                                                 GetWorld()->GetDeltaSeconds(),
			                                                 10.f); // 10.f는 회전 속도, 더 높은 값일수록 빨리 회전
			// 회전 적용
			Server_SetActorRotation(InterpolatedRotation);
		}
		else
		{
			return;
		}
		if (MovementVector.Y <= 0.f)
		{
			MovementComponent->StartBackMovement();
		}
		else
		{
			MovementComponent->StopBackMovement();
		}

		const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		if (MovementVector.Y != 0.f)
		{
			const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(ForwardDirection, MovementVector.Y);
		}
		if (MovementVector.X != 0.f)
		{
			const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
			AddMovementInput(RightDirection, MovementVector.X);
		}
		if (InFreefall)
		{
			FreefallingMoveInputY = MovementVector.Y;
			FreefallingMoveInputX = MovementVector.X;
			// MovementComponent->StartFreeFalling();
			const float FreefallSpeedMultiplier = 10.0f;
			if (MovementVector.Y != 0.f)
			{
				const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
				AddMovementInput(ForwardDirection, MovementVector.Y * FreefallSpeedMultiplier);
			}
			if (MovementVector.X != 0.f)
			{
				const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
				AddMovementInput(RightDirection, MovementVector.X * FreefallSpeedMultiplier);
			}
		}
	}
	else if (OntheParachute)
	{
		UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
		MovementComponent->StartParachute();
		const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
		FRotator MovementRotation = GetActorRotation();
		if (MovementVector.Y != 0.f)
		{
			const float ParachuteSpeedMultiplier = 10.0f;
			const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			AddMovementInput(ForwardDirection, MovementVector.Y * ParachuteSpeedMultiplier);
			//UE_LOG(LogTemp, Warning, TEXT("CurrentsPEED:%f"), MovementComponent->GetMaxSpeed());
		}
		if (MovementVector.X != 0.f)
		{
			if (MovementVector.X < 0.f)
			{
				const float ParachuteSpeedMultiplier = 10.0f;
				ActorRotate = GetActorRotation().Roll;
				FRotator RightDirection = FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, ActorRotate - 1.f);
				FVector RightVector = RightDirection.RotateVector(FVector::RightVector);
				AddMovementInput(RightVector, MovementVector.X * ParachuteSpeedMultiplier);
				UE_LOG(LogTemp, Warning, TEXT("CurrentsPEED:%f"), MovementComponent->GetMaxSpeed());
			}
			if (MovementVector.X > 0.f)
			{
				const float ParachuteSpeedMultiplier = 10.0f;
				ActorRotate = GetActorRotation().Roll;
				FRotator RightDirection = FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, ActorRotate + 1.f);
				FVector RightVector = RightDirection.RotateVector(FVector::RightVector);
				AddMovementInput(RightVector, MovementVector.X * ParachuteSpeedMultiplier);
				UE_LOG(LogTemp, Warning, TEXT("CurrentsPEED:%f"), MovementComponent->GetMaxSpeed());
			}
		}
	}
}

// void APlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
// {
// 	
// }

void APlayerCharacter::Server_SetActorRotation_Implementation(FRotator Rotator)
{
	MultiCast_SetActorRotation(Rotator);
}

bool APlayerCharacter::Server_SetActorRotation_Validate(FRotator Rotator)
{
	return true;
}

void APlayerCharacter::MultiCast_SetActorRotation_Implementation(FRotator Rotator)
{
	SetActorRotation(Rotator);
}

void APlayerCharacter::Input_MoveReleased(const FInputActionValue& InputActionValue)
{
	if (InFreefall)
	{
		FreefallingMoveInputY = 0.f;
		FreefallingMoveInputX = 0.f;
	}
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	//Server_SetAimOffset();
	// AimRotation = GetBaseAimRotation();
	// ActorRotation = GetActorRotation();
	// NormalDeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation);
	// Yaw = NormalDeltaRotator.Yaw;
	// Pitch = NormalDeltaRotator.Pitch;
	//UE_LOG(LogTemp,Warning,TEXT("NormalDeltaRotator:%s"), *NormalDeltaRotator.ToString());
	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);

		OnMouseMoved(LookAxisVector); // 마우스 무브를통해 턴 조건확인위한..
	}
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::Input_Jump(const FInputActionValue& InputActionValue)
{
	if (GetCharacterMovement()->IsFalling() || UBaseFunctionLibrary::NativeActorHasTag(
		this, FGameplayTag::RequestGameplayTag(FName("Player.State.Swim"))))
	{
		return;
	}

	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());

	if (GetMovementComponent()->IsCrouching()) //크라우칭 상태면
	{
		if (!bAnimationIsPlaying)
		{
			UnCrouch();
			FVector CrouchCameraOffset = FVector(0, 0, 40.f);
			CameraBoom->TimelineAddOffset(CrouchCameraOffset, 0.2f);
		}
	}
	else if (MovementComponent->RequestToStartProne) //누워있는 상태면
	{
		if (!bAnimationIsPlaying)
		{
			MovementComponent->StopProne();
			FVector CrouchCameraOffset = FVector(0, 0, 80.f);
			CameraBoom->TimelineAddOffset(CrouchCameraOffset, 0.2f);
		}
	}
	else
	{
		bUseControllerRotationYaw = true;
		Jump();
	}
}

// void APlayerCharacter::Input_Crouch(const FInputActionValue& InputActionValue)
// {
// 	if (GetCharacterMovement()->IsFalling() && bAnimationIsPlaying)
// 	{
// 		return;
// 	}
//
// 	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
//
// 	if (GetCharacterMovement()->IsCrouching()) //크라우칭 상태면
// 	{
// 		if (!bAnimationIsPlaying)
// 		{
// 			FVector UnCrouchCameraOffset = FVector(0, 0, 40.f);
// 			CameraBoom->TimelineAddOffset(UnCrouchCameraOffset, 0.2f);
// 			UnCrouch();
// 		}
// 		return;
// 	}
//
// 	if (MovementComponent->RequestToStartProne)
// 	{
// 		if (!bAnimationIsPlaying)
// 		{
// 			MovementComponent->StopProne();
// 			Crouch();
// 			FVector CrouchCameraOffset = FVector(0, 0, 40.f);
// 			CameraBoom->TimelineAddOffset(CrouchCameraOffset, 0.2f);
// 		}
// 		return;
// 	}
// 	if (!bAnimationIsPlaying)
// 	{
// 		FVector CrouchCameraOffset = FVector(0, 0, -40.f);
// 		CameraBoom->TimelineAddOffset(CrouchCameraOffset, 0.2f);
// 		Crouch();
// 	}
// }


// void APlayerCharacter::Input_Prone(const FInputActionValue& InputActionValue)
// {
// 	if (GetCharacterMovement()->IsFalling() && bAnimationIsPlaying)
// 	{
// 		return;
// 	}
//
// 	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
//
// 	if (MovementComponent->RequestToStartProne) //누워있는 상태면
// 	{
// 		if (!bAnimationIsPlaying)
// 		{
// 			FVector UnProneCameraOffset = FVector(0, 0, 80.f);
// 			CameraBoom->TimelineAddOffset(UnProneCameraOffset, 0.2f);
// 			MovementComponent->StopProne();
// 		}
// 		return;
// 	}
//
// 	if (GetMovementComponent()->IsCrouching())
// 	{
// 		if (!bAnimationIsPlaying)
// 		{
// 			UnCrouch();
// 			MovementComponent->StartProne();
// 			FVector ProneCameraOffset = FVector(0, 0, -40.f);
// 			CameraBoom->TimelineAddOffset(ProneCameraOffset, 0.2f);
// 		}
// 		return;
// 	}
// 	if (!bAnimationIsPlaying)
// 	{
// 		FVector ProneCameraOffset = FVector(0, 0, -80.f);
// 		CameraBoom->TimelineAddOffset(ProneCameraOffset, 0.2f); //카메라 오프셋 이동
// 		MovementComponent->StartProne();
// 	}
// }


void APlayerCharacter::Input_AbilityInputPressed(FGameplayTag InputTag)
{
	UE_LOG(LogTemp, Warning, TEXT("Ability Input Pressed"));
	BaseAbilitySystemComponent->OnAbilityInputPressed(InputTag);
}

void APlayerCharacter::Input_AbilityInputReleased(FGameplayTag InputTag)
{
	UE_LOG(LogTemp, Warning, TEXT("Ability Input Released"));
	BaseAbilitySystemComponent->OnAbilityInputReleased(InputTag);
}

void APlayerCharacter::Input_AbilityInputTab(FGameplayTag InputTag)
{
	BaseAbilitySystemComponent->OnAbilityInputPressed(InputTag);
}


void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APlayerController* PlayerController = Cast<APlayerController>(NewController);

	ULocalPlayer* LocalPlayer = PlayerController ? PlayerController->GetLocalPlayer() : nullptr;
	if (LocalPlayer)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		check(Subsystem);
		Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
	}

	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	if (PS && !FirstAttribute)
	{
		// 서버에 ASC를 설정합니다. 클라이언트는 OnRep_PlayerState()에서 이 작업을 수행합니다.
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		// 편의 속성 함수를 위해 AttributeSetBase를 설정합니다.
		BaseAttributeSet = PS->GetAttributeSetBase();

		// AI에는 PlayerController가 없으므로 확인을 위해 여기에서 다시 초기화할 수 있습니다. PlayerController가 있는 영웅을 두 번 초기화해도 아무런 해가 없습니다.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// 스타트업 데이터 할당
		if (!StartupData.IsNull())
		{
			if (UDataAsset_StartupBase* LoadedData = StartupData.LoadSynchronous())
			{
				LoadedData->GiveToAbilitySystemComponent(BaseAbilitySystemComponent.Get());
			}
		}

		ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}

		FirstAttribute = true;
	}
}

void APlayerCharacter::Die()
{
	EPlayerMeshType EnumValue = EPlayerMeshType::Face; // 예시로 머리 메시
	//USkeletalMeshComponent* HeadMesh = CharacterEquipmentMap.FindRef(EnumValue);
	//UPhysicsConstraintComponent* PhysicsConstraint = NewObject<UPhysicsConstraintComponent>(this);

	// 제약 컴포넌트를 루트에 추가
	// PhysicsConstraint->SetupAttachment(HeadMesh);
	//
	//
	// // 상위 메시와 하위 메시 연결
	// PhysicsConstraint->SetConstrainedComponents(GetMesh(),NAME_None , HeadMesh, NAME_None);
	// PhysicsConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
	// PhysicsConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
	// PhysicsConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Free, 0.f); // Z축 자유롭게 설정
	// PhysicsConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	// PhysicsConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	// PhysicsConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);
	// PhysicsConstraint->RegisterComponent();
	// 제약 설정
	// PhysicsConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
	// PhysicsConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
	// PhysicsConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Free, 0.f); // Z축 자유
	// PhysicsConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	// PhysicsConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	// PhysicsConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);
	UE_LOG(LogTemp, Warning, TEXT("Die"));

	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetCharacterMovement()->GravityScale = 0;
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	//GetCharacterMovement()->Velocity = FVector(0);


	//HeadMesh->SetAllBodiesBelowSimulatePhysics(FName("Root"),true, true);
	GetMesh()->SetSimulatePhysics(true); // 물리 시뮬레이션 시작
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly); // 충돌 및 물리 시뮬레이션 활성화
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetMesh()->WakeAllRigidBodies();
	UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharacterComp)
	{
		CharacterComp->StopMovementImmediately();
		CharacterComp->DisableMovement();
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeadSound, GetActorLocation());

	// 머리 메시의 물리 시뮬레이션을 활성화
	// HeadMesh->SetSimulatePhysics(true);
	// //
	// //
	//  HeadMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	// HeadMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	// HeadMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	// HeadMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);  // 초기 속도 설정 (필요 시)
	// HeadMesh->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
	// HeadMesh->WakeAllRigidBodies();

	// 안전하게 제거를 하려면 서버에서 타이머를 돌린 후 Destroy()하는게 좋을 거 같음
	//SetLifeSpan(10.0f);
}

void APlayerCharacter::SetCurrentCamera(UCameraComponent* NewCamera)
{
	this->CurrentCamera = NewCamera;
}

void APlayerCharacter::OnMouseMoved(FVector2D MouseMovement)
{
	CheckRotationForTurn();
}

void APlayerCharacter::CheckRotationForTurn()
{
	// 카메라나 타겟 방향


	FRotator CheckDeltaRotation = GetActorRotation() - GetBaseAimRotation();

	if (CheckDeltaRotation.Yaw >= 180.0f)
	{
		CheckDeltaRotation.Yaw -= 360.0f;
	}
	else if (CheckDeltaRotation.Yaw <= -180.0f)
	{
		CheckDeltaRotation.Yaw += 360.0f;
	}
	if (!OntheVehicle&&!InFreefall&&!OntheParachute)
	{
		UBaseAbilitySystemComponent* AbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(
			GetAbilitySystemComponent());

		if (AbilitySystemComponent && CheckDeltaRotation.Yaw >= 90.0f || CheckDeltaRotation.Yaw <= -90.f)
		{
			AbilitySystemComponent->TryActivateAbilityByTagToRandom(BaseGameplayTag::Player_Ability_Turn);
		}
	}
	// else if (OntheVehicle)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("OntheVehicle "));
	// 	if (DeltaRotation.Yaw >= -180.0f && DeltaRotation.Yaw <= -90.0f)
	// 	{
	// 		VehicleFacetoBackward = true;
	// 		UE_LOG(LogTemp, Warning, TEXT("Player Movement"));
	// 	}
	// 	else
	// 	{
	// 		VehicleFacetoBackward = false;
	// 	}
	// }
}


void APlayerCharacter::LeftLeanCameraMovement()
{
	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
	if (MovementComponent->RequestToStartProne == false)
	{
		//CameraBoom->SetWanstReversePlaying(false);
		FVector OffsetDelta = FVector(0.f, -40.f, 0.f);
		float Duration = 0.2f;


		CameraBoom->TimelineAddOffset(OffsetDelta, Duration);
	}
}

void APlayerCharacter::LeftDefaultCameraMovement()
{
	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
	if (MovementComponent->RequestToStartProne == false)
	{
		//CameraBoom->SetWanstReversePlaying(true);
		//float OppositeDistance = CameraBoom->GetDistanceMoved();
		FVector OffsetDelta = FVector(0.f, 40.f, 0.f);
		float Duration = 0.2f;

		CameraBoom->TimelineAddOffset(OffsetDelta, Duration);
	}
}

void APlayerCharacter::RightDefaultCameraMovement()
{
	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
	if (MovementComponent->RequestToStartProne == false)
	{
		//float OppositeDistance = CameraBoom->GetDistanceMoved();
		//CameraBoom->SetWanstReversePlaying(true);
		FVector OffsetDelta = FVector(0.f, -20.f, 0.f);
		float Duration = 0.2f;

		CameraBoom->TimelineAddOffset(OffsetDelta, Duration);
	}
}

void APlayerCharacter::RightLeanCameraMovement()
{
	UPlayerMovementComponent* MovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
	if (MovementComponent->RequestToStartProne == false)
	{
		//CameraBoom->SetWanstReversePlaying(false);
		FVector OffsetDelta = FVector(0.f, 20.f, 0.f);
		float Duration = 0.2f;

		CameraBoom->TimelineAddOffset(OffsetDelta, Duration);
	}
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCharacter, OntheVehicle);
	DOREPLIFETIME(APlayerCharacter, VehicleFacetoBackward);
	DOREPLIFETIME(APlayerCharacter, VehicleVelocityBackWard);
	//DOREPLIFETIME(APlayerCharacter, AimRotation);
	//DOREPLIFETIME(APlayerCharacter, ActorRotation);
	DOREPLIFETIME(APlayerCharacter, NormalDeltaRotator);
	DOREPLIFETIME(APlayerCharacter, Yaw);
	DOREPLIFETIME(APlayerCharacter, InFreefall);
	DOREPLIFETIME(APlayerCharacter, OntheParachute);
	DOREPLIFETIME(APlayerCharacter, IsSwimming);

	//DOREPLIFETIME(APlayerCharacter, Pitch);
}

void APlayerCharacter::WhenGetOntheVehicleUnequippedWeapon()
{
	if (OntheVehicle)
	{
		AWeapon_Base* CachedCurrentWeapon = GetEquippedComponent()->GetCurrentWeapon();
		if (CachedCurrentWeapon)
		{
			WeaponDisarmament();
			FPlayerWeaponData WeaponData = CachedCurrentWeapon->GetPlayerWeaponData();
			//웨폰데이터 구조체 가져오기(맵핑컨텍스트, 테그, 어빌리티)
			UInputMappingContext* CachedInputMappingContext = WeaponData.WeaponInputMappingContext;
			Client_InputMappingContextRemove(CachedInputMappingContext);
		}
	}
}

void APlayerCharacter::WeaponDisarmament() //무기장착해제 SWIM이랑 VEHICLE 탑승 사용
{
	AWeapon_Base* CachedCurrentWeapon = GetEquippedComponent()->GetCurrentWeapon();
	if (CachedCurrentWeapon)
	{
		if (CachedCurrentWeapon == GetEquippedComponent()->GetPrimarySlotWeapon())
		{
			CachedCurrentWeapon->AttachToComponent(
				GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				FName("slot_primarySocket"));
			UE_LOG(LogTemp, Warning, TEXT("PrimarySocket"));
		}
		else if (CachedCurrentWeapon == GetEquippedComponent()->GetSecondarySlot())
		{
			CachedCurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			                                       FName("slot_secondarySocket"));
			UE_LOG(LogTemp, Warning, TEXT("SecondarySocket"));
		}
		else if (CachedCurrentWeapon == GetEquippedComponent()->GetSideArmSlot())
		//지우지말것!!!!!!!!! 차탈때 총 슬롯(1슬롯,2슬롯,권총슬롯,밀리슬롯,수류탄슬롯)에 다시 넣어주는거임
		{
			CachedCurrentWeapon->AttachToComponent(
				GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				FName("SideArm"));
		}

		else if (CachedCurrentWeapon == GetEquippedComponent()->GetThrowableSlot())
		{
			CachedCurrentWeapon->AttachToComponent(
				GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				FName("throwable_Socket"));
		}
		FPlayerWeaponData WeaponData = CachedCurrentWeapon->GetPlayerWeaponData();
		Server_SetAnimLayer(nullptr);
		UBaseFunctionLibrary::RemoveGameplayTagFromActor(this, WeaponData.WeaponTag, true); //테그 삭제
		TArray<FGameplayAbilitySpecHandle> CachedAbilitySpecHandle = CachedCurrentWeapon->
			GetGrantedAbilitySpecHandles(); //어빌리티 삭제
		GetEquippedComponent()->SetCurrentWeapon(nullptr);
	}
}

void APlayerCharacter::Server_SetInFreefall_Implementation(bool NewInFreefall)
{
	InFreefall = NewInFreefall;
}


void APlayerCharacter::Server_ModifyGravity_Implementation(float GravityMultiplier)
{
	if (HasAuthority())
	{
		GetCharacterMovement()->GravityScale = GravityMultiplier;
		Multi_ModifyGravity(GravityMultiplier);
		UE_LOG(LogTemp, Warning, TEXT("Server O"));
	}
}

bool APlayerCharacter::Server_ModifyGravity_Validate(float GravityMultiplier)
{
	return true;
}

void APlayerCharacter::Multi_ModifyGravity_Implementation(float GravityMultiplier)
{
	if (!HasAuthority())
	{
		GetCharacterMovement()->GravityScale = GravityMultiplier;
		UE_LOG(LogTemp, Warning, TEXT("multi"));
	}
}

void APlayerCharacter::ModifyGravity(float GravityMultiplier)
{
	if (HasAuthority())
	{
		// 서버에서는 바로 처리
		GetCharacterMovement()->GravityScale = GravityMultiplier;
		Multi_ModifyGravity(GravityMultiplier);
		UE_LOG(LogTemp, Warning, TEXT("Server OOO"));
	}
	else
	{
		// 서버로 요청
		GetCharacterMovement()->GravityScale = GravityMultiplier;
		Server_ModifyGravity(GravityMultiplier);
		UE_LOG(LogTemp, Warning, TEXT("Server ELSE"));
	}
}

bool APlayerCharacter::GetOntheParachute() const
{
	return OntheParachute;
}

void APlayerCharacter::SetOntheParachute(bool NewParachuteState)
{
	OntheParachute = NewParachuteState;
}

void APlayerCharacter::Client_InputMappingContextRemove_Implementation(UInputMappingContext* MappingContext)
{
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	if (LocalPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("LocalPlayer exist"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LocalPlayer not exist"));
	}
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if (Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Subsystem exist"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Subsystem not exist"));
	}
	check(Subsystem);
	Subsystem->RemoveMappingContext(MappingContext); //인풋매핑컨텍스트 삭제
}

// void APlayerCharacter::ServerSetCollisionEnabled_Implementation(bool NewCollisionSet)
// {
// 	SetActorEnableCollision(NewCollisionSet);
// }
//
// bool APlayerCharacter::ServerSetCollisionEnabled_Validate(bool NewCollisionSet)
// {
// 	return true;
// }


void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	if (PS)
	{
		// 클라이언트에 대한 ASC를 설정합니다. 서버는 PossessedBy에서 이 작업을 수행합니다.
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// 클라이언트에 대한 ASC 행위자 정보를 초기화합니다. 서버는 새 액터를 보유할 때 ASC를 초기화합니다.
		BaseAbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// 편의 속성 함수를 위해 AttributeSetBase를 설정합니다.
		BaseAttributeSet = PS->GetAttributeSetBase();

		ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}

		// 체력/마나/스태미나를 최대치로 설정합니다. 이는 *Respawn*에만 필요합니다.
		//SetHealth(GetMaxHealth());
		//SetStamina(GetMaxStamina());
	}
}


void APlayerCharacter::OnComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)

{
	if (OtherActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		BeginOverlapCount += 1;
		UE_LOG(LogTemp, Warning, TEXT("%d"), BeginOverlapCount);

		ABasePlayerController* PlayerController = Cast<ABasePlayerController>(GetController());
		UE_LOG(LogTemp, Warning, TEXT("Current Controller: %s"), *GetNameSafe(GetController()));
		//UE_LOG(LogTemp, Warning, TEXT("Controller class: %s"), *GetController()->GetClass()->GetName());


		if (!PlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::OnComponentBeginOverlap PlayerController None"));
			return;
		}

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this, OtherActor, PlayerController ]()
		{
			FHitResult Hit;
			FVector Start = GetFollowCamera()->K2_GetComponentLocation();
			FVector End = Start + UKismetMathLibrary::GetForwardVector(FollowCamera->K2_GetComponentRotation()) *
				380.0f;
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(this);
			//ActorsToIgnore.Add(TestCharacter);
			ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);


			// if (!PlayerController)
			// {
			// 	UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::OnComponentBeginOverlap PlayerController None"));
			// 	return;
			// }

			if (OtherActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
			{
				UKismetSystemLibrary::LineTraceSingle(GetWorld(),
					Start, End, TraceType, false, ActorsToIgnore,
					EDrawDebugTrace::None, Hit, true,
					FLinearColor(1, 0, 0, 0),
					FLinearColor(0, 1, 0, 1));


				if (Hit.GetActor() != nullptr)
				{
					// FVector HitLocation = Hit.Location;
					// SetItemOfZ(HitLocation.Z);
					if (LookAtActor != Hit.GetActor())
					{
						LookAtActor = Hit.GetActor();

						IInteractInterface* InteractInterface = Cast<IInteractInterface>(LookAtActor);
						if (InteractInterface)
						{
							FText Result = InteractInterface->LookAt();
							UTexture2D* ResultImage = InteractInterface->SetKeyTexture();

							if (PlayerController->GetHudWidget() != nullptr)
							{
								PlayerController->GetHudWidget()->GetDisplayMessageItemWidget()->SetMessage(Result);
								PlayerController->GetHudWidget()->GetDisplayMessageItemWidget()->SetKeyTexture(ResultImage);
							}
							//InteractInterface->InteractWith();
						}
						else
						{
							LookAtActor = nullptr;
							if (PlayerController->GetHudWidget() != nullptr)
							{
								PlayerController->GetHudWidget()->GetDisplayMessageItemWidget()->SetVisibility(
									ESlateVisibility::Collapsed);
							}
							InventoryComponent->SetItem(nullptr);
						}
					}
				}
				else
				{
					LookAtActor = nullptr;
					if (PlayerController->GetHudWidget() != nullptr)
					{
						PlayerController->GetHudWidget()->GetDisplayMessageItemWidget()->SetVisibility(
							ESlateVisibility::Collapsed);
					}
					InventoryComponent->SetItem(nullptr);
				}
			}
		});

		GetWorldTimerManager().SetTimer(BeginOverlapTimerHandle, TimerDelegate, 0.1f, true);
	}
}

void APlayerCharacter::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (BeginOverlapCount > 0)
	{
		BeginOverlapCount -= 1;
		UE_LOG(LogTemp, Warning, TEXT("%d"), BeginOverlapCount);

		if (BeginOverlapCount == 0)
		{
			LookAtActor = nullptr;
			InventoryComponent->SetItem(nullptr);

			ABasePlayerController* PlayerController = Cast<ABasePlayerController>(GetController());

			if (PlayerController && PlayerController->GetHudWidget() != nullptr)
			{
				PlayerController->GetHudWidget()->GetDisplayMessageItemWidget()->SetVisibility(ESlateVisibility::Collapsed);
			}
			
			GetWorldTimerManager().ClearTimer(BeginOverlapTimerHandle);
		}
	}
}

void APlayerCharacter::OnDetectionItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                   bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Hello~~~"));

	if (AItemBase* ItemBase = Cast<AItemBase>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemBase->GetName());
		if (NearComponent != nullptr)
		{
			//NearComponent->GetGroundItems().Add(ItemBase);
			NearComponent->ServerAddGroundItem(ItemBase);
			//UE_LOG(LogTemp, Warning, TEXT("ItemBase : %s"), *ItemBase->GetItemStruct().Name.ToString());
			UE_LOG(LogTemp, Warning, TEXT("ItemBase : %s"),
			       *ItemBase->GetItemDataComponent()->GetItemRowName().ToString());

			UE_LOG(LogTemp, Warning, TEXT("ItemDataComponent : %s"),
			       *ItemBase->GetItemDataComponent()->GetItemRowName().ToString());
			UE_LOG(LogTemp, Warning, TEXT("GroundItem Num : %d"), NearComponent->GetGroundItems().Num());
			//NearComponent->UpdateNear();
			NearComponent->ServerUpdateNear();

			// if (ABasePlayerController* BasePlayerController = Cast<ABasePlayerController>(GetController()))
			// {
			// 	UE_LOG(LogTemp, Warning, TEXT("Call UpdateNearItemSlotWidget"));
			// 	BasePlayerController->GetInventoryWidget()->UpdateNearItemSlotWidget();
			// }
		}
	}
}

void APlayerCharacter::OnDetectionItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : OnDetectionItemEndOverlap"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : OnDetectionItemEndOverlap"));
	}

	UE_LOG(LogTemp, Warning, TEXT("OnDetectionItemEndOverlap"));

	if (AItemBase* ItemBase = Cast<AItemBase>(OtherActor))
	{
		for (int i = 0; i < NearComponent->GetGroundItems().Num(); i++)
		{
			if (ItemBase == NearComponent->GetGroundItems()[i])
			{
				NearComponent->ServerRemoveGroundItem(i);
				//NearComponent->ServerRequestGroundItems_Implementation();
				break;
			}
		}
	}
}
