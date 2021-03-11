// COPYRIGHT LOLOLOL

#pragma once

#include "Engine/StaticMesh.h"
#include "Player/FPSPlayer.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DP_Weapon.generated.h"

UENUM()
enum class WeaponType
{
	Primary,
	Secondry,
	Melee
};

UCLASS(Abstract)
class FG19GPDEGREEPROJECT_API UDP_Weapon : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = BaseWeapon)
	USkeletalMesh* WeaponModel = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = BaseWeapon)
	int ClipSize;
	int Clip;

	UPROPERTY(EditDefaultsOnly, Category = BaseWeapon)
	int MaxAmmo;
	int Ammo;

	bool bReloading;

	UPROPERTY(EditDefaultsOnly, Category = BaseWeapon)
	WeaponType type;

	AFPSPlayer* Owner;

	void SetupWeapon();

	UFUNCTION()
	void ShootWeapon(FVector StartLocation, FVector Direction, float Range) {};
	virtual void ShootWeapon_Implementation(FVector StartLocation, FVector Direction, float Range) { check(0 && "No ShootWeapon override."); };
	UFUNCTION()
	void AltShootWeapon(FVector StartLocation, FVector Direction, float Range) {};
	virtual void AltShootWeapon_Implementation(FVector StartLocation, FVector Direction, float Range) { check(0 && "No ShootWeapon override."); };
	UFUNCTION()
	void Reload() {};
	virtual void Reload_Implementation();
};
