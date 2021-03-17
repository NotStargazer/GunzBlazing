// COPYRIGHT LOLOLOL

#pragma once

#include "CoreMinimal.h"
#include "../MovementStateMachine.h"
#include "../MovementState.h"

/**
 * 
 */
class FG19GPDEGREEPROJECT_API DP_CrouchState : MovementState
{
public:
	DP_CrouchState();
	~DP_CrouchState();

	virtual void Initialize(UMovementStateMachine* machine) override;

	virtual void OnEnter() override;
	virtual void OnTick(float Delta) override;
	virtual void OnExit() override;

	float MoveModifer = 0.4f;
};
