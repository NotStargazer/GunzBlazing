// COPYRIGHT LOLOLOL

#pragma once

#include "CoreMinimal.h"
#include "../MovementState.h"

/**
 * 
 */
class FG19GPDEGREEPROJECT_API DP_RunState : MovementState
{
public:
	DP_RunState();
	~DP_RunState();

	virtual void OnEnter() override;
	virtual void OnTick(float Delta) override;
	virtual void OnExit() override;
};