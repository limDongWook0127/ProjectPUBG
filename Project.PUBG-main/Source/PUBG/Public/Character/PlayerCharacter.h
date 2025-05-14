// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.generated.h"


class UEquippedComponent;
class UNearComponent;
class AItemBase;
class UPUBGSpringArmComponent;
class UCameraComponent;

class UDataAsset_InputConfig;
class UInputMappingContext;
class UInputAction;
class UInventoryComponent;
class UInventoryWidget;

//자기장
class UPostProcessComponent;

class UPlayerAnimInstance;

struct FInputActionValue;

UENUM(BlueprintType)
enum class EPlayerMeshType : uint8
{
	// 플레이어 뼈대를 가진 파츠들 나열 ex) 머리, 손, 발, 상의, 하의 등등
	// 바디는 기본 mesh로 설정 후 리더포즈컴포넌트로 다른 파츠들의 애니메이션 따라오게 만듬
	Head UMETA(DisplayName = "Head"),
	Top UMETA(DisplayName = "Top"),
	Bottom UMETA(DisplayName = "Bottom"),
	Body UMETA(DisplayName = "Body"),
	Face UMETA(DisplayName = "Face"),
	Hair UMETA(DisplayName = "Hair"),
};

UENUM()
enum class PlayerCameraMode : uint8
{
	FPPCamera UMETA(DisplayName = "TPP"),
	TPPCamera UMETA(DisplayName = "FPP")
};

/**
 * 
 */
UCLASS()
class PUBG_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter(const class FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
#pragma region Mesh
	// 캐릭터가 장착할 파츠들을 맵으로 저장, enum값으로 원하는 파츠 불러올 수 있게 설정
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TMap<EPlayerMeshType, USkeletalMeshComponent*> CharacterEquipmentMap;
	
	UFUNCTION(BlueprintCallable, Category = "Mesh")
	USkeletalMeshComponent* FindMeshComponent(EPlayerMeshType PlayerMeshType);
	UFUNCTION(BlueprintCallable, Category = "Mesh")
	void SetMeshComponent(EPlayerMeshType PlayerMeshType, USkeletalMesh* SkeletalMesh);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Mesh")
	void Multicast_SetMeshComponent(EPlayerMeshType PlayerMeshType, USkeletalMesh* SkeletalMesh);
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Mesh")
	void Client_SetMeshComponent(EPlayerMeshType PlayerMeshType, USkeletalMesh* SkeletalMesh);
	
	UFUNCTION()
	void CallCheckZoomAbility();
	
	UFUNCTION()
	void CallCheckZoomOutAbility();
	
#pragma endregion

#pragma region Animation
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Animation")
	void Server_SetAnimLayer(TSubclassOf<UPlayerAnimInstance> PlayerAnimInstance);
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Animation")
	void NetMulticast_SetAnimLayer(TSubclassOf<UPlayerAnimInstance> PlayerAnimInstance);
	
	// UPROPERTY()
	// FRotator AimRotation;
	// UPROPERTY()
	// FRotator ActorRotation;
	UPROPERTY(ReplicatedUsing = OnRep_RotationValues)
	FRotator NormalDeltaRotator;
	UPROPERTY(Replicated)
	float Yaw;
	// UPROPERTY(Replicated)
	// float Pitch;
	UFUNCTION(Server, Reliable, WithValidation)
	void UpdateRotationValues();
	UFUNCTION()
	void OnRep_RotationValues();


#pragma endregion

private:
#pragma region Components
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = "Camera", meta = (AllowPrivateAccess = "true"))
	UPUBGSpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCamera;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CurrentCamera;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* PunchCapsuleLeft;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* PunchCapsuleRight;

#pragma endregion

public:
#pragma region Inputs

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Input_Move(const FInputActionValue& InputActionValue);
	virtual void Input_Move_Implementation(const FInputActionValue& InputActionValue);
	void Input_MoveReleased(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Jump(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_Prone(const FInputActionValue& InputActionValue);
	void OnQkey();
	void OnRightClick();
	void Input_AbilityInputPressed(FGameplayTag InputTag);
	void Input_AbilityInputReleased(FGameplayTag InputTag);
	void Input_AbilityInputTab(FGameplayTag InputTag);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetActorRotation(FRotator Rotator);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_SetActorRotation(FRotator Rotator);

#pragma endregion

	// Client only
	virtual void OnRep_PlayerState() override;

public:
	virtual void PossessedBy(AController* NewController) override;
	bool bIsProne;
	UPROPERTY(BlueprintReadOnly, Category = "Input")
	bool bAnimationIsPlaying;
	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetbAnimationIsPlaying(bool bNewAnimaitonIsPlaying) { bAnimationIsPlaying = bNewAnimaitonIsPlaying; }

	UPROPERTY(BlueprintReadWrite, Category = "Input")
	bool IsZoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character")
	FVector2D MoveForwardVecter;

	FGameplayTag DeadTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundBase* DeadSound;

	void RemoveCharacterAbilities();
	void Die();

	// Getter

	FORCEINLINE class UPUBGSpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE UCameraComponent* GetFirstPersonCamera() { return FirstPersonCamera; }
	FORCEINLINE UCameraComponent* GetCurrentCamera() { return CurrentCamera; }
	void SetCurrentCamera(UCameraComponent* NewCamera);

	// 이준수
public:
	// On_Rep
	void OnRep_Controller();

	//Getter
	UFUNCTION(Blueprintable)
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	UNearComponent* GetNearComponent() const { return NearComponent; }
	UFUNCTION(Blueprintable)
	AActor* GetLookAtActor() const { return LookAtActor; }

	UFUNCTION(BlueprintCallable)
	UBoxComponent* GetDetectionItem() const { return DetectionItem; }

	UFUNCTION(BlueprintCallable)
	UEquippedComponent* GetEquippedComponent() const { return EquippedComponent; }

	// Setter
	UFUNCTION(BlueprintCallable)
	void SetLookAtActor(AActor* NewActor) { LookAtActor = NewActor; }
	//자기장
public:
	UPROPERTY(VisibleAnywhere, Category="Post Process")
	UPostProcessComponent* PostProcessComponent;

	//
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnDetectionItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                 const FHitResult& SweepResult);
	UFUNCTION()
	void OnDetectionItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	AActor* LookAtActor;

	UPROPERTY()
	int32 BeginOverlapCount = 0;

	FTimerHandle BeginOverlapTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* DetectionItem;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNearComponent* NearComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UEquippedComponent* EquippedComponent;

public:
	void OnMouseMoved(FVector2D MouseMovement);
	void CheckRotationForTurn();

	UFUNCTION(BlueprintCallable, Category = "CharacterLean")
	void LeftLeanCameraMovement();
	UFUNCTION(BlueprintCallable, Category = "CharacterLean")
	void LeftDefaultCameraMovement();
	UFUNCTION(BlueprintCallable, Category = "CharacterLean")
	void RightDefaultCameraMovement();
	UFUNCTION(BlueprintCallable, Category = "CharacterLean")
	void RightLeanCameraMovement();

private:
	PlayerCameraMode CameraMode;

public:
	FORCEINLINE PlayerCameraMode GetCameraMode() const { return CameraMode; }
	FORCEINLINE void SetCameraMode(PlayerCameraMode NewCameraMode) { CameraMode = NewCameraMode; }

	//차랑관련
private:
	UPROPERTY(Replicated)
	bool OntheVehicle = false; // 탈것 스테이트 머신 전환
	UPROPERTY(Replicated)
	bool VehicleFacetoBackward = false; //90~180도면 스테이트머신의 애니메이션 변경을 위해서
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimLayer", meta = (AllowPrivateAccess = "true"))
	bool FirstAttribute = false; //어빌리티 한번이상 주입하지 않기 위해 (possessdby에서)
	UPROPERTY(Replicated)
	bool VehicleVelocityBackWard = false;

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	FORCEINLINE bool GetOnTheVehicle() const { return OntheVehicle; }
	FORCEINLINE void SetOnTheVehicle(bool NewOnTheVehicle) { OntheVehicle = NewOnTheVehicle; }
	FORCEINLINE bool GetVehicleFacetoBackward() const { return VehicleFacetoBackward; }
	FORCEINLINE void SetVehicleFacetoBackward(bool NewOnTheVehicle) { VehicleFacetoBackward = NewOnTheVehicle; }
	UFUNCTION()
	void WhenGetOntheVehicleUnequippedWeapon();
	UFUNCTION(BlueprintCallable)
	void WeaponDisarmament();
	
	UFUNCTION(Client, Reliable)
	void Client_InputMappingContextRemove(UInputMappingContext* MappingContext);
	FORCEINLINE bool GetVehicleVelocityBackWard() const { return VehicleVelocityBackWard; }
	FORCEINLINE void SetVehicleVelocityBackWard(bool NewVelocityBackWard)
	{
		VehicleVelocityBackWard = NewVelocityBackWard;
	}

	// FreeFalling 관련
private:
	UPROPERTY(Replicated,EditAnywhere, Category = "Freefall")
	bool InFreefall;
	float FreefallingMoveInputY;
	float FreefallingMoveInputX;

public:
	FORCEINLINE bool GetInFreefall() const { return InFreefall; }
	UFUNCTION(Server, Reliable)
	void Server_SetInFreefall(bool NewInFreefall);
	
	FORCEINLINE float GetFreefallingMoveInputY() const { return FreefallingMoveInputY; }
	FORCEINLINE void SetFreefallingMoveInputY(float NewFreefallingMoveInput) { FreefallingMoveInputY = NewFreefallingMoveInput; }
	FORCEINLINE float GetFreefallingMoveInputX() const { return FreefallingMoveInputX; }
	FORCEINLINE void SetFreefallingMoveInputX(float NewFreefallingMoveInput) { FreefallingMoveInputX = NewFreefallingMoveInput; }

	//Swim관련
private:
	UPROPERTY(EditAnywhere,Replicated, BlueprintReadWrite, Category = "Swim", meta = (AllowPrivateAccess = "true"))
	bool IsSwimming;
	
public:
	FORCEINLINE bool GetIsSwimming() const { return IsSwimming; }

private:
	//Parachute 관련
	UPROPERTY(EditAnywhere,Replicated, BlueprintReadWrite, Category = "Parachute", meta = (AllowPrivateAccess = "true"))
	bool OntheParachute = false;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Parachute", meta = (AllowPrivateAccess = "true"))
	float ActorRotate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parachute", meta = (AllowPrivateAccess = "true"))
	float RotateSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parachute", meta = (AllowPrivateAccess = "true"))
	FVector RotateVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parachute", meta = (AllowPrivateAccess = "true"))
	bool OnTheFirstAirPlane = false;

	UFUNCTION(server, Reliable, WithValidation)
	void Server_ModifyGravity(float GravityMultiplier);
	UFUNCTION(NetMulticast, Reliable)
	void Multi_ModifyGravity(float GravityMultiplier);
	UFUNCTION(BlueprintCallable, Category = "Character")
	void ModifyGravity(float GravityMultiplier);
public:
	FORCEINLINE bool GetFirstAirplane() const { return OnTheFirstAirPlane; }
	FORCEINLINE void SetFirstAirplane(bool NewOnTheFirstAirPlane) { OnTheFirstAirPlane=NewOnTheFirstAirPlane; }
	
public:
	UFUNCTION(BlueprintCallable)
	bool GetOntheParachute() const;
	UFUNCTION(BlueprintCallable)
	void SetOntheParachute(bool NewParachuteState);
	
};



inline void APlayerCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	//UE_LOG(LogTemp, Warning, TEXT("OnRep_Controller"));
}