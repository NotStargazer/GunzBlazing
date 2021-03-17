// COPYRIGHT LOLOLOL


#include "DP_IdleState.h"
#include "GameFramework/CharacterMovementComponent.h"

DP_IdleState::DP_IdleState()
{
	State = EMovementState::Idle;
}

DP_IdleState::~DP_IdleState()
{
}

void DP_IdleState::Initialize(UMovementStateMachine* machine)
{
	MovementState::Initialize(machine);
}

void DP_IdleState::OnEnter()
{
}

void DP_IdleState::OnTick(float Delta)
{
	if (Machine->User->GetCharacterMovement()->IsFalling()) {
		Machine->ChangeState((MovementState*)Machine->JumpState);
	}
	else if (Machine->User->WillCrouch)
	{
		Machine->ChangeState((MovementState*)Machine->CrouchState);
	}
	else if (Machine->User->GetVelocity().Size() != 0)
	{
		if (!Machine->User->Stamina->Exhausted && Machine->User->MovesForward && Machine->User->WillRun)
		{
			Machine->ChangeState((MovementState*)Machine->RunState);
		}
		else
		{
			Machine->ChangeState((MovementState*)Machine->WalkState);
		}
	}
}

void DP_IdleState::OnExit()
{

}