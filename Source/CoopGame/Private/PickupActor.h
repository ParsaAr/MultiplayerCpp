// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupActor.generated.h"

class USphereComponent;
class UDecalComponent;
class APowerup_actor;

UCLASS()
class APickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USphereComponent* spherecomp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UDecalComponent* DecalComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SphereRadiuss;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DecalTickness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<APowerup_actor> PowerUpClass;

	APowerup_actor* Powerup_Instance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsRestorablePowerUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TimeToRestorePowerUp;
	FTimerHandle RestoreTimer;
	UFUNCTION()
		void SpawnPowerUp();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
