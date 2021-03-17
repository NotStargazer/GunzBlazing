// COPYRIGHT LOLOLOL


#include "DP_ProjectileLauncher.h"
#include "Player/FPSPlayer.h"

void UDP_ProjectileLauncher::ShootWeapon_Implementation()
{
	UDP_Weapon::ShootWeapon_Implementation();
	ShootProjectile(Speed, Damage);
}
