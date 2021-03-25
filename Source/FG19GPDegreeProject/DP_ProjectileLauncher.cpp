// COPYRIGHT LOLOLOL


#include "DP_ProjectileLauncher.h"
#include "Player/FPSPlayer.h"

void UDP_ProjectileLauncher::ShootWeapon_Implementation()
{
	if (Clip > 0)
	{
		UDP_Weapon::ShootWeapon_Implementation();
	}

	ShootProjectile(Speed, Damage);
}
