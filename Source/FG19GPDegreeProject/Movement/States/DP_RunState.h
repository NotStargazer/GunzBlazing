// COPYRIGHT LOLOLOL

#pragma once

#include "CoreMinimal.h"
#include "../MovementStateMachine.h"
#include "../MovementState.h"

/**
 * 
 */
class FG19GPDEGREEPROJECT_API DP_RunState : MovementState
{
public:
	DP_RunState();
	~DP_RunState();

	virtual void Initialize(UMovementStateMachine* machine) override;

	virtual void OnEnter() override;
	virtual void OnTick(float Delta) override;
	virtual void OnExit() override;

private:

	float MoveModifer = 2.0f;

};