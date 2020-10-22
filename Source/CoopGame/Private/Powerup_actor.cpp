// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup_actor.h"


APowerup_actor::APowerup_actor()
{
	PrimaryActorTick.bCanEverTick = true;
	TypeOfPowerUp = EpowerUpType::InstantEffect;
	SetReplicates(true);
}



void APowerup_actor::BeginPlay()
{
	Super::BeginPlay();
	
}



void APowerup_actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

