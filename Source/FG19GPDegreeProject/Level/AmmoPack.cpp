// COPYRIGHT LOLOLOL


#include "AmmoPack.h"
#include "../Player/FPSPlayer.h"
#include "../Player/DP_WeaponInventory.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

AAmmoPack::AAmmoPack()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPack::OnCollect);
	Collider->SetGenerateOverlapEvents(true);
	Collider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ammo Pack Mesh"));
	Mesh->SetupAttachment(Collider);

	SetReplicates(true);
}

void AAmmoPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Mesh->AddLocalRotation(FRotator(0, RotationSpeed, 0));
}

void AAmmoPack::OnCollect(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		AFPSPlayer* Player = Cast<AFPSPlayer>(OtherActor);

		if (Player != nullptr)
		{
			UDP_WeaponInventory* Inventory = Player->WeaponInventory;
			if (!Inventory->IsMaxAmmo())
			{
				Inventory->AddAmmo(AmmoRestorePercentage);
				Collider->Deactivate();
				Mesh->SetVisibility(false);
				GetWorldTimerManager().SetTimer(AmmoRespawnTimer, this, &AAmmoPack::RespawnAmmoPack, RespawnTime, false);
			}
		}
	}
}

void AAmmoPack::RespawnAmmoPack()
{
	Collider->Activate();
	Mesh->SetVisibility(true);
}
