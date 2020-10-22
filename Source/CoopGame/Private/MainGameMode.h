// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AMainGameMode();
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver();

	UFUNCTION()
	void CheckIfAllPlayersDead();
	UPROPERTY(EditAnywhere)
		float CheckPlayersDeadInterval;
	FTimerHandle CheckPlayersTimer;
	virtual void BeginPlay() override;
};
