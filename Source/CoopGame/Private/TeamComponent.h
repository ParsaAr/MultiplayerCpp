// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeamComponent.generated.h"


UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class UTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UTeamComponent();

protected:
	
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	uint8 TeamNumber;
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static bool Check_Is_Friendly(AActor* AA, AActor* BB);
};
