// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Powerup_actor.generated.h"



UENUM(BlueprintType)
enum class EpowerUpType : uint8 
{
	//triggers an Event Instantly and goes off
	InstantEffect,
	//starts an effects  and the effects remains in a specified time then goes off
	DurationEffect,

};

UCLASS()
class APowerup_actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerup_actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
		void OnPoweUpActivated(APawn* ActivateFor);
	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
		void PowerUpExpired();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EpowerUpType TypeOfPowerUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite , meta = (EditCondition = "TypeOfPowerUp == EpowerUpType::DurationEffect" ) )
		float PoweupEffectTime;
};
