// COPYRIGHT LOLOLOL

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Player/FPSPlayer.h"

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

	void ChangeState(MovementState* newState);

	AFPSPlayer* User;
	UFUNCTION(BlueprintCallable, Category = State)
	EMovementState GetState();

	class DP_IdleState* IdleState;
	class DP_WalkState* WalkState;
	class DP_JumpState* JumpState;
	class DP_RunState* RunState;
	class DP_CrouchState* CrouchState;
	class DP_SlideState* SlideState;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
private:

	class MovementState* CurrentState;

};
