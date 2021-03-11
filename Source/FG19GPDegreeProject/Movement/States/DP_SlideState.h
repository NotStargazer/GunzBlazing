// COPYRIGHT LOLOLOL

#pragma once

#include "CoreMinimal.h"
#include "../MovementState.h"

/**
 * 
 */
class FG19GPDEGREEPROJECT_API DP_SlideState : MovementState
{
public:
	DP_SlideState();
	~DP_SlideState();

	virtual void OnEnter() override;
	virtual void OnTick(float Delta) override;
	virtual void OnExit() override;
};
