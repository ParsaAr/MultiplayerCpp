// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponCpp.h"
#include "GrenadeLuncher_CPP.generated.h"
class AScharacter;
/**
 * 
 */
UCLASS()
class AGrenadeLuncher_CPP : public AWeaponCpp
{
	GENERATED_BODY()
	

public:

	virtual void Fire() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Projectilepower;

	virtual void BeginPlay() override;
	AScharacter* charref;
};
