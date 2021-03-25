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

UENUM()
enum class ReloadType
{
	OnePerLoad,
	FullClipLoad,
	NoReload,
	DebugInstant
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int CurrentClip;
	UPROPERTY(BlueprintReadOnly)
	int MaxClip;
	UPROPERTY(BlueprintReadOnly)
	int CurrentAmmo;
	UPROPERTY(BlueprintReadOnly)
	int MaxAmmo;
};

UCLASS(Abstract)
class FG19GPDEGREEPROJECT_API UDP_Weapon : public UObject
{
	GENERATED_BODY()

protected:
	AFPSPlayer* Owner;

public:
	FTimerHandle ReloadHandle;

	UPROPERTY(EditDefaultsOnly, Category = BaseWeapon)
	USkeletalMesh* WeaponModel = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = BaseWeapon)
	int ClipSize;
	int Clip;

	UPROPERTY(EditDefaultsOnly, Category = BaseWeapon)
	int MaxAmmo;
	int Ammo;

	UPROPERTY(EditDefaultsOnly, Category = BaseWeapon)
	ReloadType LoadType;
	UPROPERTY(EditDefaultsOnly, Category = BaseWeapon)
	float ReloadTime;
	bool bReloading;

	UPROPERTY(EditDefaultsOnly, Category = BaseWeapon)
	float PushForceMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = BaseWeapon)
	WeaponType type;

	UPROPERTY(EditDefaultsOnly, Category = BaseWeapon)
	TSubclassOf<class ADP_Projectile> ProjectileActor;

	void SetupWeapon(AFPSPlayer* Player);

	UFUNCTION()
	void ShootWeapon() {};
	virtual void ShootWeapon_Implementation();
	UFUNCTION()
	void StopShootWeapon() {};
	virtual void StopShootWeapon_Implementation() {};
	UFUNCTION()
	void AltShootWeapon() {};
	virtual void AltShootWeapon_Implementation();
	UFUNCTION()
	void StopAltShootWeapon() {};
	virtual void StopAltShootWeapon_Implementation() {};
	UFUNCTION()
	void Reload() {};
	virtual void Reload_Implementation();

	UFUNCTION()
	void ShootProjectile(float Speed, float Damage);

	UFUNCTION()
	void ShootHitscan(float Damage, float MaxRange, float MinRange);

	UFUNCTION()
	void MeleeAttack(float Damage, float Range);

	void LoadWeapon();

	void AddAmmo(float Percentage = 0.5f);

	bool IsMaxAmmo();

	bool IsEmptyClip();

	void FinishLoad();

	FWeaponData GetWeaponData() const;
};