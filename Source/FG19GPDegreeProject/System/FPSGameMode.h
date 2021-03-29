// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPSGameMode.generated.h"

class AFPSPlayer;

USTRUCT(BlueprintType)
struct FScoreBoard
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int ATeamPoints;
	
	UPROPERTY(BlueprintReadOnly)
	int BTeamPoints;
};

UCLASS()
class FG19GPDEGREEPROJECT_API AFPSGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	int ATeamScore;

	UPROPERTY()
	int BTeamScore;
	
	UPROPERTY()
	TArray<AFPSPlayer*> ATeam;

	UPROPERTY()
	TArray<AFPSPlayer*> BTeam;

	void UpdatePlayerScores();

public:
	UFUNCTION(BlueprintCallable)
	FScoreBoard GetTeamScore();

	void AddPlayerToTeam(AFPSPlayer* Player);
};
