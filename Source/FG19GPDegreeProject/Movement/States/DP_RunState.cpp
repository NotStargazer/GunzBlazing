// COPYRIGHT LOLOLOL


#include "DP_RunState.h"
#include "GameFramework/CharacterMovementComponent.h"

DP_RunState::DP_RunState()
{
	State = EMovementState::Run;
}

DP_RunState::~DP_RunState()
{
}

void DP_RunState::Initialize(UMovementStateMachine* machine)
{
	MovementState::Initialize(machine);
}

void DP_RunState::OnEnter()
{
	Machine->User->Server_MoveModify(MoveModifer);
	Machine->User->GetCharacterMovement()->MaxWalkSpeed = 1200.f;
}
void DP_RunState::OnTick(float Delta)
{
	bool bCannotRunAnymore = Machine->User->Stamina->Drain(Delta);
	if (Machine->User->GetCharacterMovement()->IsFalling())
	{
		Machine->ChangeState((MovementState*)Machine->JumpState);
	}
	else if (Machine->User->WillCrouch && Machine->User->GetVelocity().Size() >= 240)
	{
		Machine->ChangeState((MovementState*)Machine->SlideState);
	}
	else if (bCannotRunAnymore || Machine->User->GetVelocity().Size() == 0)
	{
		Machine->ChangeState((MovementState*)Machine->IdleState);
	}
	else if (bCannotRunAnymore || !Machine->User->MovesForward|| !Machine->User->WillRun)
		Machine->ChangeState((MovementState*)Machine->WalkState);
	
}
void DP_RunState::OnExit()
{
	Machine->User->Server_MoveModify(1.0f);
	Machine->User->GetCharacterMovement()->MaxWalkSpeed = 600.f;
}