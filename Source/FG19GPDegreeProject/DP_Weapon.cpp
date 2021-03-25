// COPYRIGHT LOLOLOL


#include "DP_Weapon.h"
#include "DP_Projectile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "Math/OrientedBox.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

void UDP_Weapon::SetupWeapon(AFPSPlayer* Player)
{
	Clip = ClipSize;
	MaxAmmo += ClipSize;
	Ammo = MaxAmmo;
	Owner = Player;
}

void UDP_Weapon::ShootWeapon_Implementation()
{
	bReloading = false;
	Owner->GetWorldTimerManager().ClearTimer(ReloadHandle);
}

void UDP_Weapon::AltShootWeapon_Implementation()
{
	Owner->GetWorldTimerManager().ClearTimer(ReloadHandle);
}

void UDP_Weapon::Reload_Implementation()
{
	if (Ammo <= 0)
		return;

	if (bReloading)
		return;

	bReloading = true;

	switch (LoadType)
	{
	case ReloadType::OnePerLoad:
		//TODO: Start Reload Animation Here!
		Owner->GetWorldTimerManager().SetTimer(ReloadHandle, this, &UDP_Weapon::LoadWeapon, ReloadTime, true);
		break;
	case ReloadType::FullClipLoad:
		//TODO: Start Reload Animation Here!
		Owner->GetWorldTimerManager().SetTimer(ReloadHandle, this, &UDP_Weapon::LoadWeapon, ReloadTime, false);
		break;
	case ReloadType::NoReload:
		break;
	case ReloadType::DebugInstant:
		//Will Ignore Reserve Ammo!
		if (Clip < ClipSize)
			Clip = ClipSize;
		break;
	}
}

void UDP_Weapon::ShootProjectile(float Speed, float Damage)
{
	if (ProjectileActor == nullptr)
	{
		return;
	}

	if (Clip <= 0)
	{
		StopShootWeapon_Implementation();
		Reload_Implementation();
		return;
	}

	Clip--;
	Ammo--;

	FTransform Transform = FTransform::Identity;
	Transform.SetLocation(Owner->GetWeaponFirePoint());

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UWorld* const World = Owner->GetWorld();

	ADP_Projectile* Proj = World->SpawnActor<ADP_Projectile>(ProjectileActor, Transform, ActorSpawnParams);

	if (Proj != nullptr)
	{
		Proj->SetupOwners(Owner, this);
		if (Owner->IsLocallyControlled())
		{
			UE_LOG(LogTemp, Warning, TEXT("Bitch"))
		}
		AFPSPlayer* player = ((AFPSPlayer*)Owner);
		FVector Direction = Owner->GetCapsuleComponent()->GetComponentRotation().Vector();
		//FVector Direction = player->AimDirection.Vector();
		//FVector Direction = Owner->GetControlRotation().Vector();
		Proj->FireProjectile(Direction, Speed, Damage, PushForceMultiplier);
		if (player != nullptr)
		{
			player->OnFire();
#if !UE_BUILD_SHIPPING
			if (player->bDebugDrawProjectilePath)
			{
				const float ArrowLength = 3000.0f;
				const float ArrowSize = 50.0f;
				DrawDebugDirectionalArrow(World, Transform.GetLocation(), Transform.GetLocation() + Direction * ArrowLength, ArrowSize, FColor::Red, false, 2.5f);
				//DrawDebugDirectionalArrow(World, Transform.GetLocation(), Transform.GetLocation() + FacingRotationStart * ArrowLength, ArrowSize, FColor::Green);
			}
#endif
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile failed the spawn, are you missing a muzzle point on the gun model? Or are you missing a projectile in the weapon class?"))
	}
}

void UDP_Weapon::ShootHitscan(float Damage, float MaxRange, float MinRange)
{
	TArray<AActor*> Ignores;
	Ignores.Add(Owner);

	FHitResult HitResult;

	bool Hit = UKismetSystemLibrary::LineTraceSingle(Owner->GetWorld(), Owner->GetActorLocation(), Owner->GetControlRotation().Vector() * 100000, ETraceTypeQuery::TraceTypeQuery_MAX, true,
		Ignores, EDrawDebugTrace::ForDuration, HitResult, true);

	if (Hit)
	{
		IDamagable* DamagableActor = Cast<IDamagable>(HitResult.Actor);

		float FalloffPercentage = 1 - FMath::Clamp(((HitResult.Distance - MinRange) / MaxRange), 0.f, 1.f);
		float EndDamage = FalloffPercentage * Damage;

		DamagableActor->OnDamage(EndDamage, -HitResult.Normal * EndDamage * PushForceMultiplier, DamageType::HealthOnly);
	}
}

void UDP_Weapon::MeleeAttack(float Damage, float Range)
{
	TArray<AActor*> Ignores;
	Ignores.Add(Owner);

	FHitResult HitResult;

	FVector EndTrace = Owner->GetActorLocation() + Owner->GetActorRotation().Vector() * Range;

	bool Hit = UKismetSystemLibrary::BoxTraceSingle(Owner->GetWorld(), Owner->GetActorLocation(), EndTrace, FVector(75), Owner->GetControlRotation(), ETraceTypeQuery::TraceTypeQuery_MAX, true,
		Ignores, EDrawDebugTrace::ForDuration, HitResult, true);
	
	if (Hit)
	{
		IDamagable* DamagableActor = Cast<IDamagable>(HitResult.Actor);

		DamagableActor->OnDamage(Damage, -HitResult.Normal * Damage * PushForceMultiplier, DamageType::All);
	}
}

void UDP_Weapon::LoadWeapon()
{
	if (LoadType == ReloadType::FullClipLoad)
	{
		if (Ammo < ClipSize)
			Clip = Ammo;
		else
			Clip = ClipSize;

		FinishLoad();
	}

	if (LoadType == ReloadType::OnePerLoad)
	{
		Clip++;

		if (Ammo < ClipSize)
		{
			if (Clip >= Ammo)
			{
				Clip = Ammo;
				FinishLoad();
			}
		}
		else
		{
			if (Clip >= ClipSize)
			{
				Clip = ClipSize;
				FinishLoad();
			}
		}
	}
}

void UDP_Weapon::AddAmmo(float Percentage)
{
	Ammo = FMath::Min(FMath::FloorToInt(MaxAmmo * Percentage + Ammo), MaxAmmo);
}

bool UDP_Weapon::IsMaxAmmo()
{
	return Ammo >= MaxAmmo;
}

bool UDP_Weapon::IsEmptyClip()
{
	return Clip <= 0;
}

void UDP_Weapon::FinishLoad()
{
	//Play Finish Reload Animation Here
	bReloading = false;
	Owner->GetWorldTimerManager().ClearTimer(ReloadHandle);
}

FWeaponData UDP_Weapon::GetWeaponData() const
{
	FWeaponData data;
	data.CurrentAmmo = Ammo - Clip;
	data.CurrentClip = Clip;
	data.MaxAmmo = MaxAmmo;
	data.MaxClip = ClipSize;

	return data;
}
