// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerCharacter.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

class ATestCharacter;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUBG_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual FText LookAt() = 0;
	//Set Image
	UFUNCTION()
	virtual UTexture2D* SetKeyTexture();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractWith(APlayerCharacter* Character);
};
