// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor.h"
#include <Components/DecalComponent.h>
#include <Components/SphereComponent.h>
#include "Powerup_actor.h"
#include <GameFramework/Character.h>
// Sets default values
APickupActor::APickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	spherecomp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	RootComponent = spherecomp;
	DecalComp= CreateDefaultSubobject<UDecalComponent>(TEXT("Decal Comp"));
	DecalComp->SetupAttachment(RootComponent);
	SphereRadiuss = 75.0f;
	spherecomp->SetSphereRadius(SphereRadiuss);
	DecalComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	DecalTickness = 10.0f;
	DecalComp->DecalSize = FVector(DecalTickness, SphereRadiuss, SphereRadiuss);
	spherecomp->SetCollisionResponseToAllChannels(ECR_Ignore);
	spherecomp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();
	if (Role==ROLE_Authority)
	{
		SpawnPowerUp();
	}
	
}

// Called every frame
void APickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupActor::SpawnPowerUp()
{
	if (PowerUpClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Powerup_Instance = GetWorld()->SpawnActor<APowerup_actor>(PowerUpClass, this->GetActorLocation(), GetActorRotation(), SpawnParams);
	}
}

void APickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (Role == ROLE_Authority)
	{
		Super::NotifyActorBeginOverlap(OtherActor);
		ACharacter* Char_ref = Cast<ACharacter>(OtherActor);
		if (Char_ref)
		{
			if (Powerup_Instance)
			{
				Powerup_Instance->OnPoweUpActivated(Char_ref);
				Powerup_Instance = nullptr;
				if (IsRestorablePowerUp)
				{
					GetWorldTimerManager().SetTimer(RestoreTimer, this, &APickupActor::SpawnPowerUp, TimeToRestorePowerUp);
				}

			}
		}
	}
}

