// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeLuncher_CPP.h"
#include "kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "ProjectileGrenade.h"
#include "Scharacter.h"
void AGrenadeLuncher_CPP::Fire()
{
	if (charref)
	{
		APlayerController* pc = Cast<APlayerController>(charref->GetController());
		if (pc)
		{
			pc->ClientPlayCameraShake(camShakeClass);
		}
		FRotator eyerot = charref->GetControlRotation();

		FActorSpawnParameters spawnparams;
		spawnparams.Owner = this->GetInstigator();
		spawnparams.Instigator = this->GetInstigator();
		spawnparams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AProjectileGrenade* Spawnedprojectile = GetWorld()->SpawnActor<AProjectileGrenade>(ProjectileClass, SK_Weapon->GetSocketLocation(MuzzleSockett), eyerot, spawnparams);
	}
	
}

void AGrenadeLuncher_CPP::BeginPlay()
{
	charref = Cast<AScharacter>(GetOwner());
}
