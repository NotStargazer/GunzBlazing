// COPYRIGHT LOLOLOL


#include "DP_WalkState.h"
#include "GameFramework/CharacterMovementComponent.h"

DP_WalkState::DP_WalkState()
{
	State = EMovementState::Walk;
}

DP_WalkState::~DP_WalkState()
{
}

void DP_WalkState::Initialize(UMovementStateMachine* machine)
{
	MovementState::Initialize(machine);
}

void DP_WalkState::OnEnter()
{
	Machine->User->Server_MoveModify(MoveModifer);
}

void DP_WalkState::OnTick(float Delta)
{
	if (Machine->User->GetCharacterMovement()->IsFalling()) {
		Machine->ChangeState((MovementState*)Machine->JumpState);
	}
	else if (Machine->User->GetVelocity().Size() == 0)
	{
		Machine->ChangeState((MovementState*)Machine->IdleState);
	}
	else if (!Machine->User->Stamina->Exhausted && Machine->User->MovesForward && Machine->User->WillRun)
		Machine->ChangeState((MovementState*)Machine->RunState);
	else if (Machine->User->WillCrouch)
		Machine->ChangeState((MovementState*)Machine->CrouchState);
	
}

void DP_WalkState::OnExit()
{

}