// COPYRIGHT LOLOLOL


#include "DP_AutoFireLauncher.h"
#include "Player/FPSPlayer.h"
#include "TimerManager.h"

void UDP_AutoFireLauncher::ShootWeapon_Implementation()
{
	UDP_Weapon::ShootWeapon_Implementation();
	float Rate = 1 / FireRate;
	Fire();
	Owner->GetWorldTimerManager().SetTimer(FireTimer, this, &UDP_AutoFireLauncher::Fire, Rate, true);
}

void UDP_AutoFireLauncher::StopShootWeapon_Implementation()
{
	Owner->GetWorldTimerManager().ClearTimer(FireTimer);
}

void UDP_AutoFireLauncher::Fire()
{
	ShootProjectile(Speed, Damage);
}
