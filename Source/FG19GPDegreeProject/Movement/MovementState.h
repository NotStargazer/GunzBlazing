// COPYRIGHT LOLOLOL

#pragma once

#include "CoreMinimal.h"
//#include "MovementStateMachine.h"
#include "EMovementState.h"

/**
 * 
 */
class FG19GPDEGREEPROJECT_API MovementState
{
public:
	MovementState();
	~MovementState();

	class UMovementStateMachine* Machine;
	EMovementState State;

	virtual void OnEnter() PURE_VIRTUAL(MovementState::OnEnter,);
	virtual void OnTick(float Delta) PURE_VIRTUAL(MovementState::OnTick,);
	virtual void OnExit() PURE_VIRTUAL(MovementState::OnExit,);
};
