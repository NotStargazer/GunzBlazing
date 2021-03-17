// COPYRIGHT LOLOLOL


#include "DP_JumpState.h"
#include "GameFramework/CharacterMovementComponent.h"

DP_JumpState::DP_JumpState()
{
	State = EMovementState::Jump;
}

DP_JumpState::~DP_JumpState()
{
}

void DP_JumpState::Initialize(UMovementStateMachine* machine)
{
	MovementState::Initialize(machine);
}

void DP_JumpState::OnEnter()
{

}
void DP_JumpState::OnTick(float Delta)
{
	if (!Machine->User->GetCharacterMovement()->IsFalling()) {
		Machine->ChangeState((MovementState*)Machine->IdleState);

	}
}
void DP_JumpState::OnExit()
{

}