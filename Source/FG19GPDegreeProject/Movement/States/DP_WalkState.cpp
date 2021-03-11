// COPYRIGHT LOLOLOL


#include "DP_WalkState.h"

DP_WalkState::DP_WalkState()
{
	State = EMovementState::Walk;
}

DP_WalkState::~DP_WalkState()
{
}

void DP_WalkState::OnEnter()
{
	Machine->User->RunModifier;
}
void DP_WalkState::OnTick(float Delta)
{

}
void DP_WalkState::OnExit()
{

}