// COPYRIGHT LOLOLOL


#include "DP_ProjectileLauncher.h"
#include "DP_Projectile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

UDP_ProjectileLauncher::UDP_ProjectileLauncher()
{
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	WeaponModelComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Model"));
}

void UDP_ProjectileLauncher::ShootWeapon_Implementation(FVector StartLocation, FVector Direction, float Range)
{
	if (Clip <= 0)
	{
		Reload_Implementation();
		return;
	}

	Clip--;
	Ammo--;

	FTransform Transform = FTransform::Identity;
	Transform.SetLocation(StartLocation);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	UWorld* const World = Owner->GetWorld();

	ADP_Projectile* Proj = World->SpawnActor<ADP_Projectile>(ProjectileActor, Transform.GetLocation(), Transform.Rotator(), ActorSpawnParams);

	Proj->FireProjectile(Direction, Speed, Damage);
	Proj->SetupOwners(Owner, this);

	Proj->FinishSpawning(Transform);
}

void UDP_ProjectileLauncher::AltShootWeapon_Implementation(FVector StartLocation, FVector Direction, float Range)
{
}
