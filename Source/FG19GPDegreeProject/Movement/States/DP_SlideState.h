// COPYRIGHT LOLOLOL

#pragma once

#include "CoreMinimal.h"
#include "../MovementStateMachine.h"
#include "../MovementState.h"

/**
 * 
 */
class FG19GPDEGREEPROJECT_API DP_SlideState : MovementState
{
public:
	DP_SlideState();
	~DP_SlideState();

	virtual void Initialize(UMovementStateMachine* machine) override;

	virtual void OnEnter() override;
	virtual void OnTick(float Delta) override;
	virtual void OnExit() override;

private:

	float minimumSlideVelocity = 120.0f;
};
