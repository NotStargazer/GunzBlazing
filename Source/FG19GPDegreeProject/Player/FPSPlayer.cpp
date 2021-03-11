// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayer.h"
#include "../FG19GPDegreeProjectProjectile.h"
//#include "../Movement/MovementStateMachine.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "DP_WeaponInventory.h"
#include <Runtime\Engine\Classes\Engine\Engine.h>

FName AFPSPlayer::HealthComponentName(TEXT("HealthComp"));

// Sets default values
AFPSPlayer::AFPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//Movement = CreateDefaultSubobject<UMovementStateMachine>(TEXT("Movement State Machine"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(AFPSPlayer::HealthComponentName);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AFPSPlayer::OnCompHit);

	//UCapsuleComponent* MyComp = GetCapsuleComponent();
	//MyComp->OnComponentHit.AddDynamic(this, &AFPSPlayer::OnCompHit);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	WeaponInventory = CreateDefaultSubobject<UDP_WeaponInventory>(TEXT("Weapon Inventory"));
	WeaponInventory->Initialize(this, FP_Gun);
}

// Called when the game starts or when spawned
void AFPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	WeaponInventory->EquipWeapon(0);
}

// Called every frame
void AFPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
/*void AFPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}*/

void AFPSPlayer::ShootWeapon()
{
	WeaponInventory->UseWeapon(UseType::Fire, GetActorLocation() + GetActorForwardVector() * 100, GetActorForwardVector());
}

void AFPSPlayer::OnDeath()
{
	PlayerDeath.Execute(this);
	HealthComponent->Reset();
}

void AFPSPlayer::OnDamage_Implementation(float damage, FVector force, DamageType damageType)
{
	if (HealthComponent->ReduceHealth(damage))
	{
		OnDeath();
	}
}

void AFPSPlayer::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		/*if (OtherActor->IsA(AFG19GPDegreeProjectProjectile::StaticClass())) {
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
		}*/
	}
}

void AFPSPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSPlayer::ShootWeapon);


	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSPlayer::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPSPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFPSPlayer::LookUpAtRate);
}

void AFPSPlayer::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value * (RunModifier) * (CrouchModifier));
	}
}

void AFPSPlayer::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value * (RunModifier) * (CrouchModifier));
	}
}

void AFPSPlayer::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPSPlayer::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFPSPlayer::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AFG19GPDegreeProjectProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}