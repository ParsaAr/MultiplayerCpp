// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_CPP.h"

void APS_CPP::Add_Score(float ScoretoAdd)
{
	Score = FMath::Max(Score + ScoretoAdd, 0.f);
}

void APS_CPP::OnRep_Score()
{
	Super::OnRep_Score();

	OnRep_Score_BP(Score);
}
