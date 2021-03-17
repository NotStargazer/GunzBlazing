// COPYRIGHT LOLOLOL


#include "MovementStateMachine.h"

#include "States/DP_IdleState.h"
#include "States/DP_WalkState.h"
#include "States/DP_JumpState.h"
#include "States/DP_RunState.h"
#include "States/DP_CrouchState.h"
#include "States/DP_SlideState.h"

// Sets default values for this component's properties
UMovementStateMachine::UMovementStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	IdleState = new DP_IdleState();
	IdleState->Initialize(this);
	WalkState = new DP_WalkState();
	WalkState->Initialize(this);
	JumpState = new DP_JumpState();
	JumpState->Initialize(this);
	RunState = new DP_RunState();
	RunState->Initialize(this);
	CrouchState = new DP_CrouchState();
	CrouchState->Initialize(this);
	SlideState = new DP_SlideState();
	SlideState->Initialize(this);

	CurrentState = (MovementState*)IdleState;
}


// Called when the game starts
void UMovementStateMachine::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UMovementStateMachine::Initialize(AFPSPlayer* user)
{
	User = user;
}

// Called every frame
void UMovementStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentState->OnTick(DeltaTime);
}

void UMovementStateMachine::ChangeState(MovementState* newState)
{
	CurrentState->OnExit();

	CurrentState = newState;

	CurrentState->OnEnter();
}

EMovementState UMovementStateMachine::GetState()
{
	return CurrentState->State;
}

