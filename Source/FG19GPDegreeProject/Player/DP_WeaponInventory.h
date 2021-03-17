// COPYRIGHT LOLOLOL

#pragma once

#include "FPSPlayer.h"
#include "../DP_Weapon.h"
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DP_WeaponInventory.generated.h"

UENUM()
enum UseType
{
	Fire,
	AltFire,
	Reload
};

UCLASS( ClassGroup=(FPSArena), meta=(BlueprintSpawnableComponent) )
class FG19GPDEGREEPROJECT_API UDP_WeaponInventory : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = DefaultWeapons)
	TSubclassOf<UDP_Weapon> PrimaryWeaponClass;
	UPROPERTY(EditDefaultsOnly, Category = DefaultWeapons)
	TSubclassOf<UDP_Weapon> SecondaryWeaponClass;
	UPROPERTY(EditDefaultsOnly, Category = DefaultWeapons)
	TSubclassOf<UDP_Weapon> MeleeWeaponClass;
	
	UPROPERTY(VisibleInstanceOnly)
	class UDP_Weapon* PrimaryWeapon;
	UPROPERTY(VisibleInstanceOnly)
	class UDP_Weapon* SecondaryWeapon;
	UPROPERTY(VisibleInstanceOnly)
	class UDP_Weapon* MeleeWeapon;

	class UDP_Weapon* EquipedWeapon;

	class USkeletalMeshComponent* WeaponModelComp;

	class AFPSPlayer* PlayerReference;

	bool bReloading;

public:	
	// Sets default values for this component's properties
	UDP_WeaponInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;		

public:
	void Initialize(AFPSPlayer* Player, USkeletalMeshComponent* Model);

	void EquipWeapon(UINT Slot);

	void UseWeapon(UseType Use, bool Shoot);

	UFUNCTION(BlueprintCallable)
	FWeaponData GetWeaponData();
};
