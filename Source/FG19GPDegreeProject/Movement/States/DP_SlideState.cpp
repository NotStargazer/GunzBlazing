// COPYRIGHT LOLOLOL


#include "DP_SlideState.h"
#include "GameFramework/CharacterMovementComponent.h"

DP_SlideState::DP_SlideState()
{
	State = EMovementState::Slide;
}

DP_SlideState::~DP_SlideState()
{
}

void DP_SlideState::Initialize(UMovementStateMachine* machine)
{
	MovementState::Initialize(machine);
}

void DP_SlideState::OnEnter()
{
	Machine->User->bIsSliding = true;
	Machine->User->GetCharacterMovement()->GroundFriction = 0.028571f;

	Machine->User->OnCrouch(true);
}

void DP_SlideState::OnTick(float Delta)
{
	if (Machine->User->GetVelocity().Size() <= minimumSlideVelocity)
	{
		if (Machine->User->WillCrouch)
			Machine->ChangeState((MovementState*)Machine->CrouchState);
		else
			Machine->ChangeState((MovementState*)Machine->IdleState);
	}
	else if (!Machine->User->WillCrouch)
		Machine->ChangeState((MovementState*)Machine->IdleState);
	else if (Machine->User->GetCharacterMovement()->IsFalling())
		Machine->ChangeState((MovementState*)Machine->JumpState);
}

void DP_SlideState::OnExit()
{
	Machine->User->bIsSliding = false;
	Machine->User->GetCharacterMovement()->GroundFriction = 8.0f;
	Machine->User->OnCrouch(false);
}