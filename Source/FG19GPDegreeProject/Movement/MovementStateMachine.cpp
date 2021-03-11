// COPYRIGHT LOLOLOL


#include "MovementStateMachine.h"

// Sets default values for this component's properties
UMovementStateMachine::UMovementStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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

