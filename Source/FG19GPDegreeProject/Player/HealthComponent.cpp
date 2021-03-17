// COPYRIGHT LOLOLOL

#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent(/*const FObjectInitializer& ObjectInitializer*/)
{
	Reset();
}



bool UHealthComponent::ReduceHealth(float p_damage, DamageType p_type)
{
	switch (p_type)
	{
	case DamageType::HealthOnly:
		if (Shield <= 0)
			Health -= p_damage;
		break;
	case DamageType::ShieldOnly:
		if (Shield <= 0) {
			Shield -= p_damage;
		}
		else
		break;
	case DamageType::All:
		if (Shield <= 0)
			Health -= p_damage;
		else {
			Shield -= p_damage;
		}
		break;
	default:
		break;
	}

	if (Health <= 0)
		Health = 0;
	if (Shield <= 0)
		Shield = 0;

	return HealthCheck();
}

void UHealthComponent::Reset()
{
	Health = MaxHealth;
	Shield = MaxShield;
}

bool UHealthComponent::HealthCheck()
{
	return Health <= 0;
}

