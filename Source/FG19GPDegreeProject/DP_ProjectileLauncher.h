// COPYRIGHT LOLOLOL

#pragma once

#include "CoreMinimal.h"
#include "DP_Weapon.h"
#include "DP_ProjectileLauncher.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FG19GPDEGREEPROJECT_API UDP_ProjectileLauncher : public UDP_Weapon
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Weapon, meta = (DisplayName = "Projectile Speed"))
	float Speed;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	class USceneComponent* MuzzleLocation;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	class UStaticMeshComponent* WeaponModelComp;
	
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<class ADP_Projectile> ProjectileActor;

public:
	UDP_ProjectileLauncher();

	virtual void ShootWeapon_Implementation(FVector StartLocation, FVector Direction, float Range) override;
	virtual void AltShootWeapon_Implementation(FVector StartLocation, FVector Direction, float Range) override;

};
