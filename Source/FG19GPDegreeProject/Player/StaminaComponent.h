// COPYRIGHT LOLOLOL

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


UCLASS( ClassGroup=(FPSArena), meta=(BlueprintSpawnableComponent) )
class FG19GPDEGREEPROJECT_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStaminaComponent(/*const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()*/);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool Drain(float DeltaTime);
	void Reset();

	UPROPERTY(Category = "Values", BlueprintReadOnly)
	float Stamina;
	UPROPERTY(Category = "Values", BlueprintReadOnly)
	bool Exhausted;

	UPROPERTY(Category = "Stamina", EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "100"))
	float MaxStamina = 100.0f;

	UPROPERTY(Category = "Stamina", EditDefaultsOnly)
	float StaminaDrainageRate = 10.0f;
	UPROPERTY(Category = "Stamina", EditDefaultsOnly)
	float StaminaRegenerationDelay = 2.0f;
	UPROPERTY(Category = "Stamina", EditDefaultsOnly)
	float StaminaRegenerationRate = 5.0f;

private:

	void Regenerate(float DeltaTime);

	float StaminaRegenerationTime = 0.0f;
};
