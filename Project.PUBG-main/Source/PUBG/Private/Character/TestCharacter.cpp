// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TestCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Component/Inventory/InventoryComponent.h"
#include "Component/NearArea/NearComponent.h"
#include "Components/WrapBox.h"
#include "Interface/InteractInterface.h"
#include "Item/ItemBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/Inventory/InventoryWidget.h"
#include "Widgets/Inventory/ItemSlotWidget.h"


ATestCharacter::ATestCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;


	static ConstructorHelpers::FClassFinder<UInventoryWidget> InventoryWidgetAsset(TEXT("/Game/Blueprint/Widgets/Inventory/WBP_Inventory.WBP_Inventory_C"));

	if (InventoryWidgetAsset.Succeeded())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Inventory Widget Loaded"));
		PlayerInventoryClass = InventoryWidgetAsset.Class;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Inventory Widget not found"));
	}

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	NearComponent = CreateDefaultSubobject<UNearComponent>(TEXT("Near"));
	
}


void ATestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APUBGCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APUBGCharacter::Look);
	}
	else
	{
		//UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	
}

void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(PlayerInventoryClass))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Inventory Widget Loaded"));
		APlayerController* PlayerController = Cast<APlayerController>(GetController()); 
		
		InventoryWidget = Cast<UInventoryWidget>(CreateWidget(PlayerController, PlayerInventoryClass));
		if (InventoryWidget)
		{
			InventoryWidget->AddToViewport();
			InventoryWidget->SetInventoryComponent(InventoryComponent);
			InventoryWidget->GetWrapBox_Inventory()->ClearChildren();
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Inventory Widget not found"));
	}

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATestCharacter::OnComponentBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ATestCharacter::OnComponentEndOverlap);
}

void ATestCharacter::InputModeUI()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->SetShowMouseCursor(true);
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, InventoryWidget, EMouseLockMode::DoNotLock, true);
	}
	
	
}

void ATestCharacter::InputModeGame()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->SetShowMouseCursor(false);
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
	}
}

void ATestCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		BeginOverlapCount +=1;
		//UE_LOG(LogTemp, Warning, TEXT("%d"), BeginOverlapCount);
	}
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, OtherActor]()
	{
		FHitResult Hit;
		FVector Start = GetFollowCamera()->K2_GetComponentLocation();
		FVector End = Start + UKismetMathLibrary::GetForwardVector(FollowCamera->K2_GetComponentRotation()) * 380.0f;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		//ActorsToIgnore.Add(TestCharacter);
		ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);

		if (OtherActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceType, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true, FLinearColor(1, 0, 0, 0), FLinearColor (0, 1, 0, 1));
			if (Hit.GetActor() != nullptr)
			{
				if (LookAtActor != Hit.GetActor())
				{
					LookAtActor = Hit.GetActor();
					IInteractInterface* InteractInterface = Cast<IInteractInterface>(LookAtActor);
			
					if (InteractInterface)
					{
						FText result = InteractInterface->LookAt();
						//InteractInterface->InteractWith();
					}
					else
					{
						LookAtActor = nullptr;
						InventoryComponent->SetItem(nullptr);
					}
				}
				
			}
		}
		
	});

	GetWorldTimerManager().SetTimer(BeginOverlapTimerHandle, TimerDelegate, 0.1f, true);

	
	
}

void ATestCharacter::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	BeginOverlapCount -=1;
	//UE_LOG(LogTemp, Warning, TEXT("%d"), BeginOverlapCount);
	LookAtActor = nullptr;
	InventoryComponent->SetItem(nullptr);
	if (BeginOverlapCount == 0)
	{
		
		GetWorldTimerManager().ClearTimer(BeginOverlapTimerHandle);
	}
}

// void ATestCharacter::Move(const FInputActionValue& Value)
// {
// 	Super::Move(Value);
// }
//
// void ATestCharacter::Look(const FInputActionValue& Value)
// {
// 	Super::Look(Value);
// }
