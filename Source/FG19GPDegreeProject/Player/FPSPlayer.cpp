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
	
	bDebugDrawProjectilePath = false;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	WeaponInventory->Initialize(this, FP_Gun);
	WeaponInventory->EquipWeapon(0);
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	CreateDebugWidget();
	if (DebugMenuInstance != nullptr)
	{
		DebugMenuInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AFPSPlayer::CreateDebugWidget()
{
	if (DebugMenuClass == nullptr)
		return;

	if (!IsLocallyControlled())
		return;

	if (DebugMenuInstance == nullptr)
	{
		DebugMenuInstance = CreateWidget<UNetDebugWidget>(GetWorld(), DebugMenuClass);
		DebugMenuInstance->AddToViewport();
	}
}

void AFPSPlayer::OnDebugMenuToggle()
{
	bShowDebugMenu = !bShowDebugMenu;

	if (bShowDebugMenu)
		ShowDebugMenu();
	else
		HideDebugMenu();
}

void AFPSPlayer::ShowDebugMenu()
{
	CreateDebugWidget();

	if (DebugMenuInstance == nullptr)
		return;

	DebugMenuInstance->SetVisibility(ESlateVisibility::Visible);
	DebugMenuInstance->BP_OnShowWidget();
}

void AFPSPlayer::HideDebugMenu()
{
	if (DebugMenuInstance == nullptr)
		return;

	DebugMenuInstance->SetVisibility(ESlateVisibility::Collapsed);
	DebugMenuInstance->BP_OnHideWidget();
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
	if (WeaponInventory->IsEmptyClip())
		return;

	if (GetLocalRole() >= ROLE_AutonomousProxy)
	{
		if (HasAuthority())
		{
			Server_Fire(GetActorLocation(), GetControlRotation());
		}
		else
		{
			AimDirection = GetControlRotation();
			WeaponInventory->UseWeapon(UseType::Fire, true);
			Server_Fire(GetActorLocation(), GetControlRotation());
		}
	}
}

void AFPSPlayer::StopShootWeapon()
{
	Server_CeaseFire();
}

void AFPSPlayer::SwitchWeapon(const int32 Index)
{
	WeaponInventory->EquipWeapon(Index);
}

FVector AFPSPlayer::GetWeaponFirePoint() const
{
	return FP_Gun->GetSocketLocation("Muzzle");
}

void AFPSPlayer::OnDeath()
{
	if (PlayerDeath.IsBound())
	{
		PlayerDeath.Execute(this);
	}
	Health->Reset();
}

void AFPSPlayer::Server_Fire_Implementation(const FVector& StartLocation, const FRotator& FacingRotation)
{
	Multicast_Fire(StartLocation, FacingRotation);
}

void AFPSPlayer::Multicast_Fire_Implementation(const FVector& StartLocation, const FRotator& FacingRotation)
{
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		//Apply Correction of already fired projectile
	}
	else
	{
		//()->SetControlRotation(FacingRotation);
		AimDirection = FacingRotation;
		WeaponInventory->UseWeapon(UseType::Fire, true);
	}
}

void AFPSPlayer::Server_CeaseFire_Implementation()
{
	Multicast_CeaseFire();
}

void AFPSPlayer::Multicast_CeaseFire_Implementation()
{
	WeaponInventory->UseWeapon(UseType::Fire, false);
}

void AFPSPlayer::Server_Reload_Implementation()
{
	Multicast_Reload();
}

void AFPSPlayer::Multicast_Reload_Implementation()
{
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
	}
	else
	{
		WeaponInventory->UseWeapon(UseType::Reload, false);
	}
}

void AFPSPlayer::OnDamage_Implementation(float damage, FVector force, DamageType damageType)
{
	LaunchCharacter(force, false, false);
	if (Health->ReduceHealth(damage, damageType))
	{
		OnDeath();
	}
}

void AFPSPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSPlayer::Jump);
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

	PlayerInputComponent->BindAction<SwitchWeaponInput>("Weapon1", IE_Pressed, this, &AFPSPlayer::SwitchWeapon, 0);
	PlayerInputComponent->BindAction<SwitchWeaponInput>("Weapon2", IE_Pressed, this, &AFPSPlayer::SwitchWeapon, 1);
	PlayerInputComponent->BindAction<SwitchWeaponInput>("Weapon3", IE_Pressed, this, &AFPSPlayer::SwitchWeapon, 2);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSPlayer::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &AFPSPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSPlayer::LookUpAtRate);

	PlayerInputComponent->BindAction(TEXT("Net Debug"), IE_Pressed, this, &AFPSPlayer::OnDebugMenuToggle);
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

	StrafeDirection = Value;

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

void AFPSPlayer::Jump()
{
	if (Movement->GetState() == EMovementState::Jump)
	{
		WallKick();
	}

	ACharacter::Jump();
}

void AFPSPlayer::WallKick()
{
	UWorld* WorldContext = GetWorld();
	FVector ActorLocation = GetActorLocation();

	TArray<AActor*> Ignores;
	Ignores.Add(this);

	FHitResult Hit;

	bool IsWall;
	IsWall = UKismetSystemLibrary::LineTraceSingle(WorldContext, ActorLocation, ActorLocation + GetActorRightVector() * GetCapsuleComponent()->GetScaledCapsuleRadius() * WallKickRange,
		ETraceTypeQuery::TraceTypeQuery1, true, Ignores, EDrawDebugTrace::ForDuration, Hit, true);

	if (IsWall)
	{
		UCharacterMovementComponent* CharacterMovment = GetCharacterMovement();
		FVector2D XYVel = FVector2D(CharacterMovment->Velocity);
		FVector2D Forward = FVector2D(GetActorForwardVector());
		float Dot = FVector2D::DotProduct(XYVel.GetSafeNormal(), Forward);

		bool IsFacingForward = Dot > WallKickLeniency;

		if (IsFacingForward)
		{
			float ZVel = FMath::Max(CharacterMovment->Velocity.Z, CharacterMovment->JumpZVelocity);
			FVector ReflectVector = FMath::GetReflectionVector(FVector(XYVel.X, XYVel.Y, 0), Hit.Normal);
			CharacterMovment->Velocity = FVector(ReflectVector.X, ReflectVector.Y, ZVel);
		}

	}
}

void AFPSPlayer::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	if (FMath::Abs(Rate) > 0 && FMath::Abs(StrafeDirection) > 0 && Movement->GetState() == EMovementState::Jump)
	{
		AirStrafe(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AFPSPlayer::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFPSPlayer::AirStrafe(float Rate)
{	
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	FVector CurrentVelocity = FVector(MovementComp->Velocity.X, MovementComp->Velocity.Y, 0);
	FVector Velocity = CurrentVelocity;
	float MaxSpeed = Velocity.Size();

	Velocity = Velocity + GetActorRightVector() * StrafeDirection * AirStrafeMultiplier / FMath::Abs(Rate);
	if (Velocity.Size() > MaxSpeed)
	{
		Velocity = CurrentVelocity.GetSafeNormal() * MaxSpeed;
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Speed: %f"), Velocity.Size()));

	MovementComp->Velocity = FVector(Velocity.X, Velocity.Y, MovementComp->Velocity.Z);
}

//#pragma optimize("", off)
void AFPSPlayer::OnFire()
{
	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), GetActorRotation(), .25f);
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
	if (WeaponInventory->GetWeaponData().CurrentClip < WeaponInventory->GetWeaponData().MaxClip)
	{
		if (GetLocalRole() >= ROLE_AutonomousProxy)
		{
			if (HasAuthority())
			{
				Server_Reload();
			}
			else
			{
				WeaponInventory->UseWeapon(UseType::Reload, false);
				Server_Reload();
			}
		}
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