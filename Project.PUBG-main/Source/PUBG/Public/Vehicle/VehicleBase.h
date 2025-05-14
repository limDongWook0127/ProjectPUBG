// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Interface/InteractInterface.h"
#include "VehicleBase.generated.h"
class UDataAsset_InputConfig;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class PUBG_API AVehicleBase : public AWheeledVehiclePawn, public IInteractInterface
{
	GENERATED_BODY()
public:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
protected:
	AVehicleBase();
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))  / AAwheeledVehiclePawn에 이미 메시있음
	// USkeletalMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* InteractionComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* ArrowComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* HitCollisionComponent;

	//카메라 관련
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = "Camera", meta = (AllowPrivateAccess = "true"))
	UPUBGSpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	//Input
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "VehicleData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* VehicleInputConfigAsset;
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Input_Throttle(const FInputActionValue& InputActionValue);
	void Input_Steering(const FInputActionValue& InputActionValue);
	void Input_Break(const FInputActionValue& InputActionValue);
	void Input_HandBreak();
	void Input_HandBreakCompleted();
	void Input_VehicleLook(const FInputActionValue& InputActionValue);
	UFUNCTION(Server, Reliable, WithValidation)
	void Input_GetOut();
	
public:
	virtual FText LookAt() override;
	// UFUNCTION(Server, Reliable, WithValidation)
	// void ServerInteractWith(APlayerCharacter* Character);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_InteractWith(APlayerCharacter* Character);
	virtual void InteractWith_Implementation(APlayerCharacter* Character) override;
	void SetCharacterCollision(APlayerCharacter* Character);
	UFUNCTION(Client, Reliable)
	void Client_SetUpLocalPlayerInput(APlayerController* PlayerController);
	UFUNCTION(Client, Reliable)
	void Client_SetUpRemotePlayerInput(APlayerController* PlayerController);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Attach(APlayerCharacter* Character);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Possess_GetOut(APlayerController* PlayerController);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_SetActor(APlayerCharacter* Character);
	void SetActor(APlayerCharacter* Character);
	UFUNCTION()
	float GetVehicleVelocity();
	FORCEINLINE UPUBGSpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//애니메이션 관련
	bool VehicleFacetoBackward = false;
	
	//히트 판정
	UFUNCTION()
	void HitPlayerWithVehicle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(Server, Reliable,WithValidation)
	void Server_HitPlayerWithVehicle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	float CalculateSpeedDamage();
};













