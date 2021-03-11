// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FG19GPDegreeProjectGameMode.h"
#include "FG19GPDegreeProjectHUD.h"
#include "FG19GPDegreeProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFG19GPDegreeProjectGameMode::AFG19GPDegreeProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFG19GPDegreeProjectHUD::StaticClass();
}
