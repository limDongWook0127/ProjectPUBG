// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PUBG/PUBGCharacter.h"
#include "TestCharacter.generated.h"

class UNearComponent;
class UInventoryComponent;
class UInventoryWidget;
/**
 * 
 */
UCLASS()
class PUBG_API ATestCharacter : public APUBGCharacter
{
	GENERATED_BODY()

// 함수 부분
public:

	ATestCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

protected:

	UFUNCTION(BlueprintCallable)
	void InputModeUI();
	UFUNCTION(BlueprintCallable)
	void InputModeGame();

public:
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> PlayerInventoryClass;

	UPROPERTY(BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	UInventoryWidget* InventoryWidget;


	UPROPERTY(BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	AActor* LookAtActor;

	UPROPERTY()
	int32 BeginOverlapCount = 0;
	
	FTimerHandle BeginOverlapTimerHandle;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNearComponent* NearComponent;
	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);	

public:
	UFUNCTION(BlueprintCallable)
	UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
};
