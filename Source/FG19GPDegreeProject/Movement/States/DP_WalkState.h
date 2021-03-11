// COPYRIGHT LOLOLOL

#pragma once

#include "CoreMinimal.h"
#include "../MovementStateMachine.h"
#include "../MovementState.h"

/**
 * 
 */
class FG19GPDEGREEPROJECT_API DP_WalkState : MovementState
{
public:
	DP_WalkState();
	~DP_WalkState();

	virtual void OnEnter() override;
	virtual void OnTick(float Delta) override;
	virtual void OnExit() override;
};
