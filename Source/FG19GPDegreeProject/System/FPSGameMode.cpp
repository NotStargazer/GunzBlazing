// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameMode.h"
#include "FPSGameState.h"

void AFPSGameMode::BeginPlay() {

	Super::BeginPlay();

	AFPSGameState* state = GetGameState<AFPSGameState>();
	state->Initialize();
}