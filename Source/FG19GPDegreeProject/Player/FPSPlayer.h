// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "StaminaComponent.h"
#include "GameFramework/Character.h"
#include "../FG19GPDegreeProjectCharacter.h"
#include "../System/Damagable.h"
#include "FPSPlayer.generated.h"

DECLARE_DELEGATE_OneParam(PlayerDeathDelegate, AFPSPlayer*);
UCLASS()
class FG19GPDEGREEPROJECT_API AFPSPlayer : public ACharacter, public IDamagable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSPlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float MoveModifier = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		bool MovesForward = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		bool WillRun = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		bool WillCrouch = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		bool ToggleRun = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		float RunModifier = 1.5f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		float CrouchModifier = .5f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AFG19GPDegreeProjectProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** Fires a projectile. */
	void OnFire();
	
	/** Reload currently equipped weapon */
	void ReloadWeapon();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	void Run();
	void StopRun();

	void Crouch();
	void StopCrouch();

	const FString EnumToString(const TCHAR* Enum, int32 EnumValue);
	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadOnly)
	class UMovementStateMachine* Movement;

	UFUNCTION(BlueprintImplementableEvent, Category = "Crouch State")
	void OnCrouch(bool startCrouch);
	bool bIsSliding;

	void ShootWeapon();
	void StopShootWeapon();

	UPROPERTY(Category = Player, VisibleAnywhere, BlueprintReadWrite)
	class UHealthComponent* Health;
	UPROPERTY(Category = Player, VisibleAnywhere, BlueprintReadWrite)
	class UStaminaComponent* Stamina;

	UPROPERTY(Category = Player, VisibleAnywhere, BlueprintReadWrite)
	class UDP_WeaponInventory* WeaponInventory;

	FORCEINLINE float GetHealth() const { return Health->Health; }
	FORCEINLINE float GetArmor() const { return Health->Shield; }
	FORCEINLINE float GetStamina() const { return Stamina->Stamina; }

	FVector GetWeaponFirePoint() const;

	PlayerDeathDelegate PlayerDeath;
	void OnDeath();
	
	virtual void OnDamage_Implementation(float damage, FVector force, DamageType damageType) override;
	//UFUNCTION()
	//void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
