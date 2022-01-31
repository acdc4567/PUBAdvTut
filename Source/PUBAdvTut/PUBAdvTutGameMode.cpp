// Copyright Epic Games, Inc. All Rights Reserved.

#include "PUBAdvTutGameMode.h"
#include "PUBAdvTutCharacter.h"
#include "UObject/ConstructorHelpers.h"

APUBAdvTutGameMode::APUBAdvTutGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
