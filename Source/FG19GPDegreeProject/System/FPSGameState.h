// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\Player\FPSPlayer.h"
#include "GameFramework/GameState.h"
#include "FPSGameState.generated.h"

/**
 * 
 */
UCLASS()
class FG19GPDEGREEPROJECT_API AFPSGameState : public AGameState
{
	GENERATED_BODY()

	TArray<AActor*> RespawnPositions;

	TArray<AActor*> Players;

public:

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
		int MaxPlayers = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Settings")
		TArray<APlayerController*> PlayerControllers;

	void Respawn(AFPSPlayer* respawningPlayer);

	void Initialize();
};
