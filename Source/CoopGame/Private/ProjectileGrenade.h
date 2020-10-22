// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameframework/ProjectileMovementComponent.h"
#include "ProjectileGrenade.generated.h"

class UProjectileMovementComponent;
UCLASS()
class AProjectileGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProjectileMovementComponent* ProjectileComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* meshcomp;

	FTimerHandle mytimer;

	UFUNCTION()
		void explosion();
	UPROPERTY(EditDefaultsOnly , BlueprintReadWrite)
	TSubclassOf<UDamageType> Damageclass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UParticleSystem* exp_particle;
};
