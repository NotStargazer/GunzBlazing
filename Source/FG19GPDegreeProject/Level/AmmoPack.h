// COPYRIGHT LOLOLOL

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoPack.generated.h"

UCLASS()
class FG19GPDEGREEPROJECT_API AAmmoPack : public AActor
{
	GENERATED_BODY()
	
public:	
	AAmmoPack();

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* Collider;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0", ClampMax = "1"))
	float AmmoRestorePercentage;

	UPROPERTY(EditDefaultsOnly)
	float RespawnTime;

	UPROPERTY(EditDefaultsOnly)
	float RotationSpeed;

	FTimerHandle AmmoRespawnTimer;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCollect(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void RespawnAmmoPack();
};
