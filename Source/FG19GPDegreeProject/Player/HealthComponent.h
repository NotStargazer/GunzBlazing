// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../System/Damagable.h"
#include "HealthComponent.generated.h"

UCLASS(ClassGroup = (FPSArena), meta = (BlueprintSpawnableComponent))
class FG19GPDEGREEPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent(/*const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()*/);

	UPROPERTY(Category = "General Settings", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "100"))
	float MaxHealth = 100.f;
	UPROPERTY(Category = "General Settings", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "100"))
	float MaxShield = 100.f;
	
	bool ReduceHealth(float p_damage, DamageType p_type);

	void Reset();
	;
	UPROPERTY(Category = "Values", BlueprintReadOnly)
	float Health;;
	UPROPERTY(Category = "Values", BlueprintReadOnly)
	float Shield;

private:	

	bool HealthCheck();

};
