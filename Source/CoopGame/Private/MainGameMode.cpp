// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include <Engine/World.h>
#include"GameFramework/Pawn.h"
#include"GameFramework/PlayerController.h"
#include "Components/HealthComponent.h"
#include "CoopGame.h"
AMainGameMode::AMainGameMode()
{
	//PrimaryActorTick.bCanEverTick = true;
	CheckPlayersDeadInterval = 2.f;
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld()->GetName()!="StartingLvl")
	{
		GetWorldTimerManager().SetTimer(CheckPlayersTimer, this, &AMainGameMode::CheckIfAllPlayersDead, CheckPlayersDeadInterval, true);
	}
}


void AMainGameMode::CheckIfAllPlayersDead()
{
	for (FConstPlayerControllerIterator i = GetWorld()->GetPlayerControllerIterator();i;i++)
	{
		APlayerController* pc = i->Get();
		if (ensure(pc) && pc->IsPlayerController() && pc->GetPawn()) {
			APawn* ppp = pc->GetPawn();
			if (ppp) {
			UHealthComponent* HealthC =Cast<UHealthComponent>(ppp->GetComponentByClass(UHealthComponent::StaticClass()));
				if (HealthC)
				{
					if (HealthC->CurrentHealth > 0) {
						//return as soon as found an alive player
						return;
					}
				}
			}

		}

	}
	GameOver();
	
}
