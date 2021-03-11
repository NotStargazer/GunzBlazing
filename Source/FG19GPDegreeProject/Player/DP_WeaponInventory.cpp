// COPYRIGHT LOLOLOL

#include "DP_WeaponInventory.h"
#include "../DP_Weapon.h"
#include "Components/SkeletalMeshComponent.h"

UDP_WeaponInventory::UDP_WeaponInventory()
{
}

void UDP_WeaponInventory::BeginPlay()
{
	Super::BeginPlay();

	PrimaryWeapon = PrimaryWeaponClass.GetDefaultObject();
	if (IsValid(PrimaryWeapon))
	{
		PrimaryWeapon->SetupWeapon();
	}
	SecondaryWeapon = SecondaryWeaponClass.GetDefaultObject();
	if (IsValid(SecondaryWeapon))
	{
		SecondaryWeapon->SetupWeapon();
	}
	MeleeWeapon = MeleeWeaponClass.GetDefaultObject();
	if (IsValid(MeleeWeapon))
	{
		MeleeWeapon->SetupWeapon();
	}
}

void UDP_WeaponInventory::Initialize(AFPSPlayer* Player, USkeletalMeshComponent* Model)
{
	PlayerReference = Player;
	WeaponModelComp = Model;
}

void UDP_WeaponInventory::EquipWeapon(UINT Slot)
{
	switch (Slot % 3)
	{
	case 0:
		EquipedWeapon = PrimaryWeapon;
		break;

	case 1:
		EquipedWeapon = SecondaryWeapon;
		break;

	case 2:
		EquipedWeapon = MeleeWeapon;
		break;
	}

	if (!IsValid(EquipedWeapon))
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Weapon!!!"));
		return;
	}

	if (!IsValid(EquipedWeapon->WeaponModel))
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Weapon Model!!!"));
		return;
	}

	WeaponModelComp->SetSkeletalMesh(EquipedWeapon->WeaponModel);
	EquipedWeapon->Owner = PlayerReference;
	//TODO: Equip Animation, Position Weapon
}

void UDP_WeaponInventory::UseWeapon(UseType Use, FVector FirePoint, FVector Direction)
{
	switch (Use % 3)
	{
	case 0:
		EquipedWeapon->ShootWeapon_Implementation(FirePoint, Direction, 100000);
		break;

	case 1:
		EquipedWeapon->AltShootWeapon_Implementation(FirePoint, Direction, 100000);
		break;

	case 2:
		EquipedWeapon->Reload_Implementation();
		break;
	}
}