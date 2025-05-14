// Copyright Epic Games, Inc. All Rights Reserved.

#include "PUBGGameMode.h"
#include "PUBGCharacter.h"
#include "UObject/ConstructorHelpers.h"

APUBGGameMode::APUBGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
