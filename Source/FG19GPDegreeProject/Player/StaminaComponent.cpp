// COPYRIGHT LOLOLOL

#include "StaminaComponent.h"
#include <Runtime\Engine\Classes\Engine\Engine.h>

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent(/*const FObjectInitializer& ObjectInitializer*/)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Reset();
}

void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Stamina < MaxStamina)
	{
		if (StaminaRegenerationDelay > StaminaRegenerationTime)
			StaminaRegenerationTime += DeltaTime;
		else
			Regenerate(DeltaTime);
	}
}

bool UStaminaComponent::Drain(float DeltaTime)
{
	Stamina -= StaminaDrainageRate * DeltaTime;
	StaminaRegenerationTime = 0.0f;
	
	if (Stamina <= 0.0f) {
		Stamina = 0.0f;
		Exhausted = true;
		//if (GEngine) {
		//	GEngine->AddOnScreenDebugMessage(-1, .0000625f, FColor::Red, TEXT("Total Exhaustion"));
		//}
	}

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, .0000625f, FColor::Cyan, FString::Printf(TEXT("Stamina is currently: %f"), Stamina));

	return Exhausted;
}

void UStaminaComponent::Regenerate(float DeltaTime)
{
	Stamina += StaminaRegenerationRate * DeltaTime;
	if (Stamina >= MaxStamina)
	{
		Exhausted = false;
	}
	if (GEngine && GetOwnerRole() == ENetRole::ROLE_AutonomousProxy)
		GEngine->AddOnScreenDebugMessage(-1, .0000625f, FColor::Orange, FString::Printf(TEXT("Stamina is currently: %f"), Stamina));
}

void UStaminaComponent::Reset()
{
	Stamina = MaxStamina;
	Exhausted = false;
	StaminaRegenerationTime = 0.0f;
}

