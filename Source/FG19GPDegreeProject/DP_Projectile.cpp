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

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetGenerateOverlapEvents(false);
	CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	// Set as root component
	RootComponent = CollisionComp;

	Model = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileModel"));
	Model->SetupAttachment(CollisionComp);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;
}

void ADP_Projectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovement->ProjectileGravityScale = bHasGravity;
	ProjectileMovement->bRotationFollowsVelocity = !bHasGravity;
}

//TODO: APlayerController IS TEMPORARY, REPLACE WITH PLAYER FOR MORE CONTROL
void ADP_Projectile::FireProjectile(FVector Direction, float Speed, float Damage)
{
	ProjectileMovement->Velocity = Direction * Speed;
	MaximumDamage = Damage;

	//ProjectileOwner = ProjOwner;
}

void ADP_Projectile::Explode(TArray<AActor*> IgnoreActors)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	IgnoreActors.Add(this);
	TArray<AActor*> OutActors;

	bool Hit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), ExplosionRadius, ObjectTypes, nullptr, IgnoreActors, OutActors);
	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, true, 10.0f);

	if (Hit)
	{
		for (AActor* Overlap : OutActors)
		{
			UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s"), *Overlap->GetName());

			//TODO: Interface with all things that can take damage
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
	// Only add impulse and destroy projectile if we hit a physics
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		IDamagable* DamagableActor = Cast<IDamagable>(OtherActor);

		if (DamagableActor)
		{
			DamagableActor->OnDamage_Implementation(MaximumDamage, NormalImpulse * PushForceMultiplier, DamageType::All);
		}

		//TODO: Only Explode on Hit if Not Fuse Projectile
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(OtherActor);
		Explode(IgnoreActors);
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
	Explode(IgnoreActors);
}

void ADP_Projectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

