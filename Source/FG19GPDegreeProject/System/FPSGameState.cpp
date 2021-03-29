// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AFPSGameState::Initialize() {

#pragma region	Find all available respawn position
	RespawnPositions.Init(nullptr, 0);
	for (TActorIterator<APlayerStart> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		APlayerStart* playerStart = Cast<APlayerStart>(*ActorItr);
		if (playerStart)
		{
			RespawnPositions.Add(playerStart);
			//LOG("APrimaryPlayerController: GetAreaLightingController(): controller found!");
		}
	}
#pragma endregion



#pragma region	Creating players

	UGameplayStatics::RemovePlayer(GetWorld()->GetFirstPlayerController(), true);

	bool isLocalState = IsLocalState();
	if (!isLocalState)
	{
		return;
	}

	PlayerControllers.Init(nullptr, MaxPlayers);

	// Create Players
	for (int32 i = 0; i < MaxPlayers; i++)
	{
		PlayerControllers[i] = Cast<APlayerController>(UGameplayStatics::CreatePlayer(GetWorld(), i));
		PlayerControllers[i]->Tags.Init(FName(""), 1);
		PlayerControllers[i]->Tags[0] = FName(*FString::FromInt(i));

		AFPSPlayer* player = Cast<AFPSPlayer>(PlayerControllers[i]->GetCharacter());

		if (player != nullptr )
			player->PlayerDeath.BindUObject(this, &AFPSGameState::Respawn);
		
		Respawn(player);
	}

#pragma endregion
}

void AFPSGameState::Respawn(AFPSPlayer* respawningPlayer)
{
	if (RespawnPositions.IsValidIndex(0)) {
		float rn = FMath::RandRange(0, RespawnPositions.Num()-1);

		FVector respawnLocation = RespawnPositions[rn]->GetActorLocation();
		FRotator respawnRotation = RespawnPositions[rn]->GetActorRotation();

		respawningPlayer->SetActorLocation(respawnLocation);
		respawningPlayer->SetActorRotation(respawnRotation);
	}
}

bool AFPSGameState::IsLocalState()
{
	const ENetMode NetMode = GetNetMode();

	if (NetMode != NM_DedicatedServer)
	{
		// Not networked.
		return true;
	}
	/*
	if (NetMode == NM_Client && GetLocalRole() == ROLE_AutonomousProxy)
	{
		// Networked client in control.
		return true;
	}

	if (GetRemoteRole() != ROLE_AutonomousProxy && GetLocalRole() == ROLE_Authority)
	{
		// Local authority in control.
		return true;
	}
	*/
	return false;
}