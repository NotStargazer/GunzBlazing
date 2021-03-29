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

	PrimaryWeapon = DuplicateObject(PrimaryWeaponClass.GetDefaultObject(), nullptr);
	SecondaryWeapon = DuplicateObject(SecondaryWeaponClass.GetDefaultObject(), nullptr);
	MeleeWeapon = DuplicateObject(MeleeWeaponClass.GetDefaultObject(), nullptr);
}

void UDP_WeaponInventory::Initialize(AFPSPlayer* Player, USkeletalMeshComponent* Model)
{
	PlayerReference = Player;
	WeaponModelComp = Model;

	if (IsValid(PrimaryWeapon))
	{
		PrimaryWeapon->SetupWeapon(PlayerReference);
	}
	if (IsValid(SecondaryWeapon))
	{
		SecondaryWeapon->SetupWeapon(PlayerReference);
	}
	if (IsValid(MeleeWeapon))
	{
		MeleeWeapon->SetupWeapon(PlayerReference);
	}
}

void UDP_WeaponInventory::EquipWeapon(UINT Slot)
{
	UDP_Weapon* OldWeapon = EquipedWeapon;

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

	if (EquipedWeapon == OldWeapon)
	{
		return;
	}

	if (!IsValid(EquipedWeapon))
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Weapon!!!"));
		EquipedWeapon = OldWeapon;
		return;
	}

	if (!IsValid(EquipedWeapon->WeaponModel))
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Weapon Model!!!"));
		EquipedWeapon = OldWeapon;
		return;
	}

	WeaponModelComp->SetSkeletalMesh(EquipedWeapon->WeaponModel);
	//TODO: Equip Animation, Position Weapon
}

void UDP_WeaponInventory::UseWeapon(UseType Use, bool Shoot)
{
	switch (Use % 3)
	{
	case 0:
		if (Shoot)
			EquipedWeapon->ShootWeapon_Implementation();
		else
			EquipedWeapon->StopShootWeapon_Implementation();
		break;

	case 1:
		if (Shoot)
			EquipedWeapon->AltShootWeapon_Implementation();
		else
			EquipedWeapon->StopAltShootWeapon_Implementation();
		break;

	case 2:
		EquipedWeapon->Reload_Implementation();
		break;
	}
}

void UDP_WeaponInventory::AddAmmo(float Percentage)
{
	if (IsValid(PrimaryWeapon))
		PrimaryWeapon->AddAmmo(Percentage);
	if (IsValid(SecondaryWeapon))
		SecondaryWeapon->AddAmmo(Percentage);
	if (IsValid(MeleeWeapon))
		MeleeWeapon->AddAmmo(Percentage);
}

bool UDP_WeaponInventory::IsMaxAmmo()
{
	bool Primary = false;
	bool Secondary = false;
	bool Melee = false;

	if (IsValid(PrimaryWeapon))
		Primary = PrimaryWeapon->IsMaxAmmo();
	if (IsValid(SecondaryWeapon))
		Secondary = SecondaryWeapon->IsMaxAmmo();
	if (IsValid(MeleeWeapon))
		Melee = MeleeWeapon->IsMaxAmmo();

	return (Primary && Secondary && Melee);
}

bool UDP_WeaponInventory::IsEmptyClip()
{
	return EquipedWeapon->IsEmptyClip();
}

FWeaponData UDP_WeaponInventory::GetWeaponData()
{
	return EquipedWeapon->GetWeaponData();
}
