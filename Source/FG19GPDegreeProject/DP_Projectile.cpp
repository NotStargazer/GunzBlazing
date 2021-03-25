// COPYRIGHT LOLOLOL


#include "DP_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

ADP_Projectile::ADP_Projectile()
{
	// Capsule can be used as a sphere and a capsule
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CollisionComp->OnComponentHit.AddDynamic(this, &ADP_Projectile::OnHit);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ADP_Projectile::BeginOverlap);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	CollisionComp->SetSimulatePhysics(false);
	CollisionComp->SetRelativeRotation(FRotator(90, 0, 0));

	// Set as root component
	RootComponent = CollisionComp;

	Model = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileModel"));
	Model->SetupAttachment(CollisionComp);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	//SetReplicates(true);
}

void ADP_Projectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->SetAllUseCCD(true);
	CollisionType = CollisionComp->GetCollisionEnabled();
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	ProjectileMovement->ProjectileGravityScale = sizeof(bHasGravity);
}

void ADP_Projectile::FireProjectile(FVector Direction, float Speed, float Damage, float Force)
{
	ProjectileMovement->bRotationFollowsVelocity = bHasGravity;
	MaximumDamage = Damage;
	PushForceMultiplier = Force;

	ProjectileMovement->Velocity = Direction * Speed;
	CollisionComp->SetCollisionEnabled(CollisionType);
}

void ADP_Projectile::Explode(FVector Point, TArray<AActor*> IgnoreActors)
{
	if (bExploded)
	{
		return;
	}
	else
	{
		bExploded = true;
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	IgnoreActors.Add(this);
	TArray<AActor*> OutActors;

	bool Hit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Point, ExplosionRadius, ObjectTypes, nullptr, IgnoreActors, OutActors);
	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Point, ExplosionRadius, 12, FColor::Red, true, 10.0f);

	if (Hit)
	{
		for (AActor* Overlap : OutActors)
		{
			UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s"), *Overlap->GetName());

			IDamagable* DamagableActor = Cast<IDamagable>(Overlap);

			if (DamagableActor)
			{
				FVector Difference = Overlap->GetActorLocation() - GetActorLocation();

				float Distance = FMath::Clamp(1 - Difference.Size() / ExplosionRadius, 0.f, 1.f);

				FVector ForceDirection = Difference.GetSafeNormal();
				FVector Force = ForceDirection * Distance * PushForceMultiplier;

				DamagableActor->OnDamage_Implementation(MaximumDamage * Distance, Force, DamageType::All);
			}
		}
	}

	Destroy();
}

void ADP_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor != (AActor*)PlayerOwner)
	{
		ProjectileMovement->bSimulationEnabled = false;
		CollisionComp->SetSimulatePhysics(true);

		IDamagable* DamagableActor = Cast<IDamagable>(OtherActor);

		if (DamagableActor)
		{
			DamagableActor->OnDamage_Implementation(MaximumDamage, NormalImpulse * PushForceMultiplier, DamageType::All);

			TArray<AActor*> IgnoreActors;
			IgnoreActors.Add(OtherActor);
			Explode(Hit.ImpactPoint, IgnoreActors);
			return;
		}

		if (bFuseProjectile)
		{
			bFuseLit = true;
		}
		else
		{
			TArray<AActor*> IgnoreActors;
			Explode(Hit.ImpactPoint, IgnoreActors);
		}
	}
}

void ADP_Projectile::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor != (AActor*)PlayerOwner && !bFuseLit)
	{
		ProjectileMovement->bSimulationEnabled = false;
		CollisionComp->SetSimulatePhysics(true);

		IDamagable* DamagableActor = Cast<IDamagable>(OtherActor);
		FVector ProjLocation = GetActorLocation();
		FVector OtherLoc = OtherActor->GetActorLocation();

		if (DamagableActor)
		{
			FVector Impact = SweepResult.ImpactPoint - OtherLoc;
			FVector ImpactNormal = Impact.GetSafeNormal();
			if (SweepResult.Time > 0.999999999f)
				ImpactNormal = (ProjLocation - OtherLoc).GetSafeNormal();

			DamagableActor->OnDamage_Implementation(MaximumDamage, -ImpactNormal * PushForceMultiplier, DamageType::All);

			TArray<AActor*> IgnoreActors;
			IgnoreActors.Add(OtherActor);
			if (SweepResult.Time > 0.999999999f)
				Explode(ProjLocation, IgnoreActors);
			else
				Explode(SweepResult.ImpactPoint, IgnoreActors);
			return;
		}

		if (bFuseProjectile)
		{
			bFuseLit = true;
		}
		else
		{
			TArray<AActor*> IgnoreActors;
			if (SweepResult.Time > 0.999999999f)
				Explode(ProjLocation, IgnoreActors);
			else
				Explode(SweepResult.ImpactPoint, IgnoreActors);
		}
	}
}

void ADP_Projectile::SetupOwners(AFPSPlayer* Player, UDP_Weapon* Weapon)
{
	PlayerOwner = Player;
	WeaponOwner = Weapon;
}

void ADP_Projectile::OnDamage_Implementation(float damage, FVector force, DamageType damageType)
{
	TArray<AActor*> IgnoreActors;
	Explode(GetActorLocation(), IgnoreActors);
}

void ADP_Projectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bFuseLit)
	{
		FuseTimer += DeltaSeconds;

		if (FuseTimer >= FuseDuration)
		{
			TArray<AActor*> IgnoreActors;
			Explode(GetActorLocation(), IgnoreActors);
		}
	}
}

