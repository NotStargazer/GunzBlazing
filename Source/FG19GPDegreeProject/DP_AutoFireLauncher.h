// COPYRIGHT LOLOLOL

#pragma once

#include "CoreMinimal.h"
#include "DP_Weapon.h"
#include "DP_AutoFireLauncher.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FG19GPDEGREEPROJECT_API UDP_AutoFireLauncher : public UDP_Weapon
{
	GENERATED_BODY()

public:
	FTimerHandle FireTimer;

	UPROPERTY(EditDefaultsOnly, Category = Weapon, meta = (DisplayName = "Fire Rate (Bullets Per Second)"))
	float FireRate = 4;

	UPROPERTY(EditDefaultsOnly, Category = Weapon, meta = (DisplayName = "Projectile Speed"))
	float Speed = 10000;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	float Damage = 10;

	virtual void ShootWeapon_Implementation() override;
	virtual void StopShootWeapon_Implementation() override;

	void Fire();
};
