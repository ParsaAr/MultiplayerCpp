// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamComponent.h"

// Sets default values for this component's properties
UTeamComponent::UTeamComponent()
{
	TeamNumber = 255;
}


// Called when the game starts
void UTeamComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}

bool UTeamComponent::Check_Is_Friendly(AActor* AA, AActor* BB)
{
	if (AA && BB)
	{
		UTeamComponent* TCA = Cast<UTeamComponent>(AA->GetComponentByClass(UTeamComponent::StaticClass()));
		UTeamComponent* TCB = Cast<UTeamComponent>(BB->GetComponentByClass(UTeamComponent::StaticClass()));
		if (TCA && TCB) 
		{
			return (TCA->TeamNumber == TCB->TeamNumber);
		}
		else
		{
			return true;
		}		
	}else
	{
		return true;
	}
}



