// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponCpp.generated.h"


USTRUCT()
struct FShootHitData
{
	GENERATED_BODY()


	UPROPERTY()
		bool ShotSomething;
	UPROPERTY()
		FVector_NetQuantize Hitpointt;
	UPROPERTY()
		TEnumAsByte<EPhysicalSurface> SurfaceTypee;
	UPROPERTY()
		FVector_NetQuantize TracerEnd;
};




class AScharacter;
class USkeletalMeshComponent;
class UHealthComponent;
UCLASS()
class AWeaponCpp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponCpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SK_Weapon;
	UFUNCTION(BlueprintCallable)
	 virtual void Fire();
	AScharacter* CharRef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<UDamageType> DamgeTypeClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UParticleSystem* MuzzleParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FName MuzzleSockett;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UParticleSystem* BeamParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FName BeamParameterName;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShake> camShakeClass;

	UParticleSystem* hitparticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UParticleSystem* deafulthitParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UParticleSystem* DefaultFleshHitParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UParticleSystem* VulnerableFleshHitParticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float defaultDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite , meta = (ClampMin = 0.0f))
		float ShootingDirection_Error_HalfRadius_Deg;
	UFUNCTION(Server , Reliable , WithValidation)
	void StartFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void StopFire();

	FTimerHandle ShootingTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RateOfFire;
	float LasttimeFired;

	UPROPERTY(ReplicatedUsing=OnRep_WeaponFireCosmetics)
	int numberofbulletsshot;
	
	UFUNCTION()
	void OnRep_WeaponFireCosmetics();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	FShootHitData FireHitData;
};
