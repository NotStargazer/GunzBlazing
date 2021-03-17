// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "../FG19GPDegreeProjectProjectile.h"
#include "../Movement/EMovementState.h"
#include "../Movement/MovementStateMachine.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "DP_WeaponInventory.h"
#include <Runtime\Engine\Classes\Engine\Engine.h>

// Sets default values
AFPSPlayer::AFPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Movement = CreateDefaultSubobject<UMovementStateMachine>(TEXT("Movement State Machine"));
	Movement->Initialize(this);

	//HealthComponent = CreateDefaultSubobject<UHealthComponent>(AFPSPlayer::HealthComponentName);
	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	//GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AFPSPlayer::OnCompHit);

	Stamina = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina"));

	//UCapsuleComponent* MyComp = GetCapsuleComponent();
	//MyComp->OnComponentHit.AddDynamic(this, &AFPSPlayer::OnCompHit);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	//GetCharacterMovement()->MaxWalkSpeed = 600.0f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

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
	
	
}

// Called when the game starts or when spawned
void AFPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	WeaponInventory->Initialize(this, FP_Gun);
	WeaponInventory->EquipWeapon(0);
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

// Called every frame
void AFPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FString message = TEXT("Our enum value: ") + EnumToString(TEXT("EMovementState"), static_cast<uint8>(Movement->GetState()));
	if (GEngine) {
		//GEngine->ClearOnScreenDebugMessages();
		GEngine->AddOnScreenDebugMessage(-1, .0000625f, FColor::Green, message);
	}
}


void AFPSPlayer::ShootWeapon()
{
	WeaponInventory->UseWeapon(UseType::Fire, true);
}

void AFPSPlayer::StopShootWeapon()
{
	WeaponInventory->UseWeapon(UseType::Fire, false);
}

FVector AFPSPlayer::GetWeaponFirePoint() const
{
	return FP_Gun->GetSocketLocation("Muzzle");
}

void AFPSPlayer::OnDeath()
{
	PlayerDeath.Execute(this);
	Health->Reset();
}

void AFPSPlayer::OnDamage_Implementation(float damage, FVector force, DamageType damageType)
{
	LaunchCharacter(force, false, false);
	if (Health->ReduceHealth(damage, damageType))
	{
		OnDeath();
	}
}

/*
void AFPSPlayer::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (OtherActor->IsA(AFG19GPDegreeProjectProjectile::StaticClass())) {
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
		}
	}
}
*/

void AFPSPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSPlayer::ShootWeapon);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPSPlayer::StopShootWeapon);

	PlayerInputComponent->BindAction("Reload", IE_Released, this, &AFPSPlayer::ReloadWeapon);

	// Bind jump events
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AFPSPlayer::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AFPSPlayer::StopRun);

	// Bind jump events
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFPSPlayer::Crouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFPSPlayer::StopCrouch);

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
	if (bIsSliding)
		return;
	if (Value != 0.0f)
	{
		if (Value > 0.0f)
			MovesForward = true;
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value * MoveModifier);
		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Move Modifier is: %f"), MoveModifier));
	}
	else
		MovesForward = false;
}

void AFPSPlayer::MoveRight(float Value)
{
	if (bIsSliding)
		return;
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value * MoveModifier);
	}
}

void AFPSPlayer::Run()
{
	if (!ToggleRun)
		WillRun = true;
	else {
		WillRun = !WillRun;
	}
}

void AFPSPlayer::StopRun()
{
	if (!ToggleRun)
		WillRun = false;
}

void AFPSPlayer::Crouch()
{
	WillCrouch = true;
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Will Crouch")));
}

void AFPSPlayer::StopCrouch()
{
	WillCrouch = false;
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Will Stop Crouch")));
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

//#pragma optimize("", off)
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

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}
//#pragma optimize("", on)

void AFPSPlayer::ReloadWeapon()
{
	if (WeaponInventory->GetWeaponData().CurrentClip < WeaponInventory->GetWeaponData().MaxClip) {
		WeaponInventory->UseWeapon(UseType::Reload, false);
	}
}

const FString AFPSPlayer::EnumToString(const TCHAR* Enum, int32 EnumValue)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, Enum, true);
	if (!EnumPtr)
		return NSLOCTEXT("Invalid", "Invalid", "Invalid").ToString();

#if WITH_EDITOR
	return EnumPtr->GetDisplayNameText(EnumValue).ToString();
#else
	return EnumPtr->GetEnumName(EnumValue);
#endif
}