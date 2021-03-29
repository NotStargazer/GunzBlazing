// COPYRIGHT LOLOLOL


#include "DP_CrouchState.h"
#include "GameFramework/CharacterMovementComponent.h"

DP_CrouchState::DP_CrouchState()
{
	State = EMovementState::Crouch;
}

DP_CrouchState::~DP_CrouchState()
{
}

void DP_CrouchState::Initialize(UMovementStateMachine* machine)
{
	MovementState::Initialize(machine);
}

void DP_CrouchState::OnEnter()
{
	Machine->User->Server_Run(false);
	Machine->User->Server_MoveModify(MoveModifer);
	Machine->User->OnCrouch(true);
}
void DP_CrouchState::OnTick(float Delta)
{
	if (Machine->User->GetCharacterMovement()->IsFalling()) {
		Machine->ChangeState((MovementState*)Machine->JumpState);
	}
	else if (!Machine->User->WillCrouch)
		Machine->ChangeState((MovementState*)Machine->IdleState);
	/*else if (!Machine->User->Stamina->Exhausted && Machine->User->MovesForward && Machine->User->WillRun)
		Machine->ChangeState((MovementState*)Machine->RunState);*/
}
void DP_CrouchState::OnExit()
{
	Machine->User->Server_MoveModify(1.0f);
	Machine->User->OnCrouch(false);
}