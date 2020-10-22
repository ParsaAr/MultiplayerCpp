// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PS_CPP.generated.h"

/**
 * 
 */
UCLASS()
class APS_CPP : public APlayerState
{
	GENERATED_BODY()
public :
	UFUNCTION(BlueprintCallable)
	void Add_Score(float ScoreToadd);

	virtual void OnRep_Score() override;

	UFUNCTION(BlueprintImplementableEvent)
		void OnRep_Score_BP(float CurentScore);
};
