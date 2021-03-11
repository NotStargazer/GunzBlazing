// COPYRIGHT LOLOLOL

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Player/FPSPlayer.h"

#include "States/DP_CrouchState.h"
#include "States/DP_IdleState.h"
#include "States/DP_JumpState.h"
#include "States/DP_RunState.h"
#include "States/DP_SlideState.h"
#include "States/DP_WalkState.h"

#include "EMovementState.h"
#include "MovementState.h"
#include "MovementStateMachine.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FG19GPDEGREEPROJECT_API UMovementStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMovementStateMachine();

	void Initialize(AFPSPlayer* user);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AFPSPlayer* User;
	EMovementState* State;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
private:

	class MovementState* CurrentState;


	void ChangeState(MovementState* newState);
};
