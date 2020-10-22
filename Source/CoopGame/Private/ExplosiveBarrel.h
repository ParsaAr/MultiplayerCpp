// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"

class UHealthComponent;
class URadialForceComponent;

UCLASS()
class AExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* meshcomp;

	UPROPERTY(VisibleAnywhere)
		UHealthComponent* HealthCompp;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_OnExplosionRepNotifyEvent)
	bool isExploded;

	UFUNCTION()
		void OnRep_OnExplosionRepNotifyEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void Explosion();
	UFUNCTION()
	void IsRecievingDamage(class UHealthComponent* HealthComp, float currentHealth, float DeltaHealth, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UPROPERTY(EditAnywhere)
		UParticleSystem* ExplosionParticle;
	UPROPERTY(EditAnywhere)
		float LaunchupPower;
	UPROPERTY(VisibleAnywhere , BlueprintReadWrite)
	URadialForceComponent* RadialFComp;

	UFUNCTION(NetMulticast, Reliable)
		void ExplosionMultiCast();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
