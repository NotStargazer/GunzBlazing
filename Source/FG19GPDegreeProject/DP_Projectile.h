// COPYRIGHT LOLOLOL

#pragma once

#include "System/Damagable.h"
#include "Player/FPSPlayer.h"
#include "DP_Weapon.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DP_Projectile.generated.h"

UENUM()
enum class ProjectileType : uint8
{
	Physical,
	Energy
};

UCLASS(config=Weapon)
class FG19GPDEGREEPROJECT_API ADP_Projectile : public AActor, public IDamagable
{
	GENERATED_BODY()

	ECollisionEnabled::Type CollisionType;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	class UCapsuleComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	class UStaticMeshComponent* Model;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = Behaviour)
	bool bHasGravity;

	UPROPERTY(EditDefaultsOnly, Category = Behaviour)
	bool bFuseProjectile;

	UPROPERTY(EditDefaultsOnly, Category = Behaviour, meta = (EditCondition = "bFuseProjectile"))
	float FuseDuration;

	bool bFuseLit;
	float FuseTimer;

	UPROPERTY(EditDefaultsOnly, Category = Behaviour, meta = (DisplayName = "Projectile Type"))
	ProjectileType ProjType;

	UPROPERTY(EditDefaultsOnly, Category = Values)
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = Values)
	float PushForceMultiplier;

	float MaximumDamage;

	class AFPSPlayer* PlayerOwner;
	class UDP_Weapon* WeaponOwner;

	bool bExploded;

protected:
	virtual void BeginPlay();

public:	
	ADP_Projectile();

	UFUNCTION()
	void FireProjectile(FVector Direction, float Speed, float Damage);

	UFUNCTION()
	void Explode(FVector Point, TArray<AActor*> IgnoreActors);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetupOwners(AFPSPlayer* Player, UDP_Weapon* Weapon);

	UFUNCTION()
	virtual void OnDamage_Implementation(float damage, FVector force, DamageType damageType) override;

	UFUNCTION(meta = (Latent, LatentInfo = "Tick"))
	virtual void Tick(float DeltaSeconds) override;

	/** Returns CollisionComp subobject **/
	FORCEINLINE class UCapsuleComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	//FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
