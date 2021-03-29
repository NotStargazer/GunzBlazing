// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameMode.h"
#include "FPSGameState.h"
#include "../Player/FPSPlayer.h"

void AFPSGameMode::BeginPlay() {

	Super::BeginPlay();

	AFPSGameState* state = GetGameState<AFPSGameState>();
	state->Initialize();
}

void AFPSGameMode::UpdatePlayerScores()
{
	ATeamScore = 0;
	BTeamScore = 0;
	
	for (AFPSPlayer* Player : ATeam)
	{
		ATeamScore += Player->GetPlayerData().Kills;
	}

	for (AFPSPlayer* Player : BTeam)
	{
		BTeamScore += Player->GetPlayerData().Kills;
	}
}

FScoreBoard AFPSGameMode::GetTeamScore()
{
	UpdatePlayerScores();

	FScoreBoard Score;
	Score.ATeamPoints = ATeamScore;
	Score.BTeamPoints = ATeamScore;
	
	return Score;
}

void AFPSGameMode::AddPlayerToTeam(AFPSPlayer* Player)
{
	if (ATeam.Num() < BTeam.Num())
		ATeam.Add(Player);
	else
		BTeam.Add(Player);
}
