// COPYRIGHT LOLOLOL

#pragma once

#include "EMovementState.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Walk			UMETA(DisplayName = "Walk"),
	Run				UMETA(DisplayName = "Run"),
	Jump			UMETA(DisplayName = "Jump"),
	Crouch			UMETA(DisplayName = "Crouch"),
	Slide			UMETA(DisplayName = "Slide"),
	WallBounce		UMETA(DisplayName = "Wall Bounce")
};