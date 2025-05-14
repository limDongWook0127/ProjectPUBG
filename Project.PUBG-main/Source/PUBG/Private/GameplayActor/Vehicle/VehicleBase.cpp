#include "Vehicle/VehicleBase.h"
#include "Components/BoxComponent.h"
#include "Component/PUBGSpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Component/Input/BaseInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/BaseGameplayTag.h"
#include "DataAsset/Input/DataAsset_InputConfig.h"
#include "ChaosVehicleMovementComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AnimNodes/AnimNode_RandomPlayer.h"
#include "Component/Movement/PlayerMovementComponent.h"
#include "Controller/BasePlayerController.h"


void AVehicleBase::BeginPlay()
{
	Super::BeginPlay();
	if (HitCollisionComponent)
	{
		HitCollisionComponent->OnComponentHit.AddDynamic(this, &AVehicleBase::HitPlayerWithVehicle);
		UE_LOG(LogTemp, Warning, TEXT("HitCollisionComponent"));
	}
}

void AVehicleBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PlayerController = Cast<APlayerController>(NewController))
	{
		UE_LOG(LogTemp, Warning, TEXT("playercontroller"));
		Client_SetUpLocalPlayerInput(PlayerController);
	}
}

void AVehicleBase::Client_SetUpLocalPlayerInput_Implementation(APlayerController* PlayerController)
{
	if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		UE_LOG(LogTemp, Warning, TEXT("CAST LocalPlayer"));
		UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		check(Subsystem);
		Subsystem->AddMappingContext(VehicleInputConfigAsset->DefaultMappingContext, 0);
		UE_LOG(LogTemp, Warning, TEXT("ADDMAP"));
	}
}

AVehicleBase::AVehicleBase()
{
	bReplicates = true;
	bReplicateUsingRegisteredSubObjectList = true;


	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	InteractionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionComponent"));
	BoxComponent->SetupAttachment(GetMesh(), "RootComponent");
	InteractionComponent->SetupAttachment(GetMesh(), "RootComponent");
	BoxComponent->InitBoxExtent(FVector(40.f));
	InteractionComponent->InitBoxExtent(FVector(20.0f));

	CameraBoom = CreateDefaultSubobject<UPUBGSpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(GetMesh(), "RootComponent");
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, UPUBGSpringArmComponent::SocketName);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(GetMesh(), "RootComponent");
}


void AVehicleBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UBaseInputComponent* BaseInputComponent = CastChecked<UBaseInputComponent>(PlayerInputComponent);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_Throttle,
	                                          ETriggerEvent::Triggered, this, &AVehicleBase::Input_Throttle);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_Throttle,
	                                          ETriggerEvent::Completed, this, &AVehicleBase::Input_Throttle);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_Steering,
	                                          ETriggerEvent::Triggered, this, &AVehicleBase::Input_Steering);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_Steering,
	                                          ETriggerEvent::Completed, this, &AVehicleBase::Input_Steering);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_Break,
	                                          ETriggerEvent::Triggered, this, &AVehicleBase::Input_Break);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_Break,
	                                          ETriggerEvent::Completed, this, &AVehicleBase::Input_Break);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_HandBreak,
	                                          ETriggerEvent::Triggered, this, &AVehicleBase::Input_HandBreak);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_HandBreak,
	                                          ETriggerEvent::Completed, this, &AVehicleBase::Input_HandBreakCompleted);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_VehicleLook,
	                                          ETriggerEvent::Triggered, this, &AVehicleBase::Input_VehicleLook);
	BaseInputComponent->BindNativeInputAction(VehicleInputConfigAsset, BaseGameplayTag::InputTag_Vehicle_GetOut,
	                                          ETriggerEvent::Started, this, &AVehicleBase::Input_GetOut);
}

void AVehicleBase::Input_Throttle(const FInputActionValue& InputActionValue)
{
	GetVehicleMovement()->SetThrottleInput(InputActionValue.Get<float>());
}

void AVehicleBase::Input_Steering(const FInputActionValue& InputActionValue)
{
	GetVehicleMovement()->SetSteeringInput(InputActionValue.Get<float>());
}

void AVehicleBase::Input_Break(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Brake Inputvalue : %s"), *InputActionValue.ToString());
	GetVehicleMovement()->SetBrakeInput(InputActionValue.Get<float>());
}

void AVehicleBase::Input_HandBreak()
{
	GetVehicleMovement()->SetHandbrakeInput(true);
}

void AVehicleBase::Input_HandBreakCompleted()
{
	GetVehicleMovement()->SetHandbrakeInput(false);
}

void AVehicleBase::Input_VehicleLook(const FInputActionValue& InputActionValue)
{
	AddControllerYawInput(InputActionValue.Get<FVector>().X);
	AddControllerPitchInput(InputActionValue.Get<FVector>().Y);
}

void AVehicleBase::Input_GetOut_Implementation()
{
	if (PlayerCharacter->GetOnTheVehicle() == true)
	{
		PlayerCharacter->SetOnTheVehicle(false);
		APlayerController* PlayerController = Cast<APlayerController>(this->GetController());
		Client_SetUpRemotePlayerInput(PlayerController);
		FRotator VehicleRotator = this->GetCameraBoom()->GetTargetRotation();

		// if (HasAuthority())
		// {
		// 	PlayerController->Possess(PlayerCharacter);
		// 	UE_LOG(LogTemp, Warning, TEXT("Possess1"));
		// }
		Server_Possess_GetOut(PlayerController);
		PlayerController->SetControlRotation(VehicleRotator); // 차량에서 바라보던 카메라 위치로
		//FVector CurrentLocation = GetActorLocation();
		//CurrentLocation.Y -= 150; // Y 값을 -200만큼 감소시킴
		//CurrentLocation.Z += 140;
		// PlayerCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		// PlayerCharacter->SetActorLocation(CurrentLocation, true, nullptr, ETeleportType::None); // 액터 로케이션 차옆으로
		// PlayerCharacter->SetActorEnableCollision(true);
		MultiCast_SetActor(PlayerCharacter);
	}
}

bool AVehicleBase::Input_GetOut_Validate()
{
	return true;
}

// void AVehicleBase::Input_GetOut_Implementation()
// {
// 	if (PlayerCharacter->GetOnTheVehicle() == true)
// 	{
// 		PlayerCharacter->SetOnTheVehicle(false);
// 		APlayerController* PlayerController = Cast<APlayerController>(this->GetController());
// 		//PlayerController = Cast<APlayerController>(this->GetController());
// 		//Client_SetUpRemotePlayerInput(PlayerController);
// 		FRotator VehicleRotator = this->GetCameraBoom()->GetTargetRotation();
// 		//PlayerController->UnPossess();
// 		// if (HasAuthority())
// 		// {
// 		// 	PlayerController->Possess(PlayerCharacter);
// 		// 	UE_LOG(LogTemp, Warning, TEXT("Possess1"));
// 		// }
// 		//Server_Input_GetOut(PlayerController);
// 		PlayerController->SetControlRotation(VehicleRotator); // 차량에서 바라보던 카메라 위치로
// 		// FVector CurrentLocation = GetActorLocation();
// 		// CurrentLocation.Y -= 150; // Y 값을 -200만큼 감소시킴
// 		// CurrentLocation.Z += 140;
// 		MultiCast_SetActor(PlayerCharacter);
// 		// PlayerCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
// 		//  PlayerCharacter->SetActorLocation(CurrentLocation, true, nullptr, ETeleportType::None); // 액터 로케이션 차옆으로
// 		//  PlayerCharacter->SetActorEnableCollision(true);
// 	}
// }

// void AVehicleBase::Input_GetOut()
// {
// 	if (PlayerCharacter->GetOnTheVehicle() == true)
// 	{
// 		PlayerCharacter->SetOnTheVehicle(false);
// 		APlayerController* PlayerController = Cast<APlayerController>(this->GetController());
// 		Client_SetUpRemotePlayerInput(PlayerController);
// 		FRotator VehicleRotator = this->GetCameraBoom()->GetTargetRotation();
//
// 		// if (HasAuthority())
// 		// {
// 		// 	PlayerController->Possess(PlayerCharacter);
// 		// 	UE_LOG(LogTemp, Warning, TEXT("Possess1"));
// 		// }
// 		Server_Possess_GetOut(PlayerController);
// 		PlayerController->SetControlRotation(VehicleRotator); // 차량에서 바라보던 카메라 위치로
// 		//FVector CurrentLocation = GetActorLocation();
// 		//CurrentLocation.Y -= 150; // Y 값을 -200만큼 감소시킴
// 		//CurrentLocation.Z += 140;
// 		// PlayerCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
// 		// PlayerCharacter->SetActorLocation(CurrentLocation, true, nullptr, ETeleportType::None); // 액터 로케이션 차옆으로
// 		// PlayerCharacter->SetActorEnableCollision(true);
// 		MultiCast_SetActor(PlayerCharacter);
// 	}
// }

void AVehicleBase::Server_Possess_GetOut_Implementation(APlayerController* PlayerController)
{
	if (HasAuthority())
	{
		PlayerController->Possess(PlayerCharacter);
		UE_LOG(LogTemp, Warning, TEXT("Possess1"));
	}
}

bool AVehicleBase::Server_Possess_GetOut_Validate(APlayerController* PlayerController)
{
	return true;
}

void AVehicleBase::Client_SetUpRemotePlayerInput_Implementation(APlayerController* PlayerController)
{
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (LocalPlayer)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		check(Subsystem);
		Subsystem->RemoveMappingContext(VehicleInputConfigAsset->DefaultMappingContext);
	}
}

FText AVehicleBase::LookAt()
{
	return FText::FromString("LookAt");
}

void AVehicleBase::InteractWith_Implementation(APlayerCharacter* Character) //이거는 서버에서 실행되는 함수임
{
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	PlayerCharacter = Character;

	UE_LOG(LogTemp,Warning, TEXT("InteractWith"));
	
	if (PlayerController)
	{
		
		Character->SetOnTheVehicle(true); //애님인스턴스로 보내기 위함
		PlayerCharacter->WhenGetOntheVehicleUnequippedWeapon();//
		Character->SetActorEnableCollision(false); // 콜리전을 꺼서 차량안으로 들어갈 수 있도록
		SetCharacterCollision(Character);
		FRotator CharacterRotator = Character->GetCameraBoom()->GetTargetRotation();
		
		if (Character && Character->GetCharacterMovement())
		{
			Character->GetCharacterMovement()->StopMovementImmediately(); // 이동 즉시 멈추기
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		}
		
		Character->SetActorLocation(ArrowComponent->GetComponentLocation());
		Character->SetActorRotation(ArrowComponent->GetComponentRotation());
		Character->AttachToComponent(ArrowComponent, FAttachmentTransformRules::KeepWorldTransform);
		
		
		//PlayerController->UnPossess(); // 기존 포세스를 해제
		if (HasAuthority())
		{
			PlayerController->Possess(this);
			UE_LOG(LogTemp, Warning, TEXT("Possess"));
		}
		PlayerController->SetControlRotation(CharacterRotator);
	}
	MultiCast_InteractWith(Character);
}

void AVehicleBase::SetCharacterCollision(APlayerCharacter* Character)
{
	Character->SetActorEnableCollision(false);
}

void AVehicleBase::MultiCast_SetActor_Implementation(APlayerCharacter* Character)
{
	SetActor(Character);
}

void AVehicleBase::SetActor(APlayerCharacter* Character)
{
	Character->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Y -= 150; // Y 값을 -200만큼 감소시킴
	CurrentLocation.Z += 140;
	Character->SetActorLocation(CurrentLocation, true, nullptr, ETeleportType::None); // 액터 로케이션 차옆으로
	Character->SetActorEnableCollision(true);
}

// void AVehicleBase::ServerInteractWith_Implementation(APlayerCharacter* Character)
// {
// 	MultiCast_InteractWith(Character);
// }
//
// bool AVehicleBase::ServerInteractWith_Validate(APlayerCharacter* Character)
// {
// 	return true;
// }

void AVehicleBase::Server_Attach_Implementation(APlayerCharacter* Character)
{
	// Character->SetOnTheVehicle(true); //애님인스턴스로 보내기 위함
	// PlayerCharacter->WhenGetOntheVehicleUnequippedWeapon();//
	// Character->SetActorEnableCollision(false); // 콜리전을 꺼서 차량안으로 들어갈 수 있도록
	// SetCharacterCollision(Character);
	//
	// FRotator CharacterRotator = Character->GetCameraBoom()->GetTargetRotation();
	// Character->SetActorLocation(ArrowComponent->GetComponentLocation());
	// Character->SetActorRotation(ArrowComponent->GetComponentRotation());
	// Character->AttachToComponent(ArrowComponent, FAttachmentTransformRules::KeepWorldTransform);
}

bool AVehicleBase::Server_Attach_Validate(APlayerCharacter* Character)
{
	return true;
}
void AVehicleBase::MultiCast_InteractWith_Implementation(APlayerCharacter* Character)
{
	SetCharacterCollision(Character);
}


float AVehicleBase::GetVehicleVelocity()
{
	return GetVehicleMovement()->GetForwardSpeed();
}

void AVehicleBase::HitPlayerWithVehicle(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Server_HitPlayerWithVehicle(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

bool AVehicleBase::Server_HitPlayerWithVehicle_Validate(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                                        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                                                        const FHitResult& Hit)
{
	return true;
}

void AVehicleBase::Server_HitPlayerWithVehicle_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                                              UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                                                              const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("HitPlayerWithVehicle1"));
	if (PlayerCharacter != nullptr && PlayerCharacter->GetOnTheVehicle() == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("HitPlayerWithVehicle"));
		FGameplayEventData Payload;
		
		Payload.Instigator = PlayerCharacter; //가해자
		Payload.Target = OtherActor; //피해자
		Payload.EventMagnitude = CalculateSpeedDamage(); //데미지
		if (Payload.Instigator)
		{
			UE_LOG(LogTemp, Warning, TEXT("Payload.Instigator:%s"), *Payload.Instigator->GetName());
		}
		UE_LOG(LogTemp, Warning, TEXT("Payload.Target:%s"), *Payload.Target->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Payload.EventMagnitude:%f"), Payload.EventMagnitude);

		if ((OtherActor!=PlayerCharacter)&&OtherActor && OtherActor != this)
		{
			UE_LOG(LogTemp, Warning, TEXT("OtherActor:%s"), *OtherActor->GetName());
			ABasePlayerController* VehicleController = Cast<ABasePlayerController>(GetController());
			if (VehicleController)
			{
				VehicleController->HitEventServer(OtherActor, BaseGameplayTag::Player_Event_Action_Hit,
				                                  Payload);
			}
		}
	}
}

float AVehicleBase::CalculateSpeedDamage()
{
	float CurrentVelocity = GetVehicleMovement()->GetForwardSpeed();

	float LowDamage = 30.f;
	float MiddleDamage = 50.f;
	float HighDamage = 100.f;
	if (30.f > CurrentVelocity && CurrentVelocity > -30.f)
	{
		return 0;
	}
	if ((-100.f <= CurrentVelocity && CurrentVelocity < -30.f) || (30.f < CurrentVelocity && CurrentVelocity <= 100.f))
	{
		return LowDamage;
	}
	else if ((-300 <= CurrentVelocity && CurrentVelocity < 100.f) || (100 < CurrentVelocity && CurrentVelocity <= 300))
	{
		return MiddleDamage;
	}
	else
	{
		return HighDamage;
	}
}
