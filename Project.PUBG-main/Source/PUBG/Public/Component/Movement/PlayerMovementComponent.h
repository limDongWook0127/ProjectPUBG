// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FGDSavedMove : public FSavedMove_Character
	{
	public:

		typedef FSavedMove_Character Super;

		///@brief Resets all saved variables.
		virtual void Clear() override;

		///@brief Store input commands in the compressed flags.
		virtual uint8 GetCompressedFlags() const override;

		///@brief This is used to check whether or not two moves can be combined into one.
		///Basically you just check to make sure that the saved variables are the same.
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;

		///@brief Sets up the move before sending it to the server. 
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData) override;
		///@brief Sets variables on character movement component before making a predictive correction.
		virtual void PrepMoveFor(class ACharacter* Character) override;

		// Sprint
		uint8 SavedRequestToStartSprinting : 1;

		// Walking
		uint8 SavedRequestToStartWalking : 1;
		
		// Prone		
		uint8 SavedRequestToStartProne : 1;
		
		// BackMovement		
		uint8 SavedRequestToBackMovement : 1;

		uint8 SavedRequestFreefalling : 1;
		uint8 SavedRequestParachute : 1;
	};
	
	class FGDNetworkPredictionData_Client : public FNetworkPredictionData_Client_Character
	{
	public:
		FGDNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		///@brief Allocates a new copy of our custom saved move
		virtual FSavedMovePtr AllocateNewMove() override;
	};


public:
	UPlayerMovementComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	uint8 RequestToStartSprinting : 1;
	uint8 RequestToStartWalking : 1;
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	uint8 RequestToStartProne : 1;
	UPROPERTY(BlueprintReadOnly, Replicated)
	uint8 RequestToBackMovement : 1;
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	uint8 RequestFreefalling : 1;
	UPROPERTY(BlueprintReadOnly, Replicated)
	uint8 RequestParachute : 1;
	
	UPROPERTY(BlueprintReadOnly, Replicated)
	float LeaningValue;
	
	virtual float GetMaxSpeed() const override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	// Sprint
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StartSprinting();
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StopSprinting();

	// Walking
	UFUNCTION(BlueprintCallable, Category = "Walking")
	void StartWalking();
	UFUNCTION(BlueprintCallable, Category = "Walking")
	void StopWalking();
	
	// Prone
	UFUNCTION(BlueprintCallable, Category = "Prone")
	void StartProne();
	UFUNCTION(BlueprintCallable, Category = "Prone")
	void StopProne();
	
	// Prone
	UFUNCTION(BlueprintCallable, Category = "Leaning")
	void StartLeaning(float Value);
	UFUNCTION(BlueprintCallable, Category = "Leaning")
	void StopLeaning();

	// BackMovement
	UFUNCTION(BlueprintCallable, Category = "Back")
	void StartBackMovement();
	UFUNCTION(BlueprintCallable, Category = "Back")
	void StopBackMovement();
	

	UFUNCTION(BlueprintCallable, Category = "FreeFalling")
	void StartFreeFalling();
	UFUNCTION(BlueprintCallable, Category = "FreeFalling")
	void StopFreeFalling();

	UFUNCTION(BlueprintCallable, Category = "Parachute")
	void StartParachute();
	UFUNCTION(BlueprintCallable, Category = "Parachute")
	void StopParachute();
	
	float AddSHIFTSprint = 150.0f;
	float SubProne = 240.0f;
	float Subcrouch = 100.f;
	float SubCTRLWalk = 200.f;
	float SubCTRLProne = 60.f;
	float SubCTRLCrouch =170.f;

	// UFUNCTION()
	// void FreefallingVelocitySettings(FVector2D MovementVector);
};
