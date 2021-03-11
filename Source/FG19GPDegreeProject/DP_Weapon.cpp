// COPYRIGHT LOLOLOL


#include "DP_Weapon.h"

void UDP_Weapon::SetupWeapon()
{
	Clip = ClipSize;
	Ammo = MaxAmmo + ClipSize;
}

void UDP_Weapon::Reload_Implementation()
{
	if (Clip < ClipSize)
	{
		Clip++;
	}
}
