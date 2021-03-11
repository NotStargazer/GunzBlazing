// COPYRIGHT LOLOLOL

#pragma once

#include "CoreMinimal.h"
#include "../MovementState.h"

/**
 * 
 */
class FG19GPDEGREEPROJECT_API DP_IdleState : MovementState
{
public:
	DP_IdleState();
	~DP_IdleState();

	virtual void OnEnter() override;
	virtual void OnTick(float Delta) override;
	virtual void OnExit() override;
};
