#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent(const FObjectInitializer& ObjectInitializer)
{
	Reset();
}



bool UHealthComponent::ReduceHealth(float p_damage)
{
	Health -= p_damage;

	return HealthCheck();
}

void UHealthComponent::Reset()
{
	Health = MaxHealth;
	Armor = MaxArmor;
}

bool UHealthComponent::HealthCheck()
{
	return Health <= 0;
}

