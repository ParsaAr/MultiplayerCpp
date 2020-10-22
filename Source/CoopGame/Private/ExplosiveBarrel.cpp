// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "Private/Components/HealthComponent.h"
#include "kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "net/UnrealNetwork.h"
#include "CoopGame.h"
// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isExploded = false;
	LaunchupPower = 1000;
	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MymeshComp"));
	RootComponent = meshcomp;
	HealthCompp = CreateDefaultSubobject<UHealthComponent>(TEXT("MyhealthComp"));
	HealthCompp->OnHealthChangedEvent.AddDynamic(this, &AExplosiveBarrel::IsRecievingDamage);
	RadialFComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceCompemte"));
	RadialFComp->SetupAttachment(RootComponent);

	SetReplicates(true);
	SetReplicateMovement(true);
	
	
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AExplosiveBarrel::OnRep_OnExplosionRepNotifyEvent()
{
	if (isExploded)
	{
		
		UMaterialInstanceDynamic* DM = meshcomp->CreateDynamicMaterialInstance(0);
		DM->SetScalarParameterValue(FName("IsExploded"), 1);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, this->GetActorLocation(), FRotator::ZeroRotator, true);
		
	}
}

void AExplosiveBarrel::IsRecievingDamage(class UHealthComponent* HealthComp, float currentHealth, float DeltaHealth, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	
	if (isExploded)
	{
		return;
	}
	if (currentHealth<=0 && !isExploded)
	{	
		isExploded = true;
		
		
		if ( GetNetMode()!=NM_DedicatedServer)
		{
			OnRep_OnExplosionRepNotifyEvent();
		}
		
		RadialFComp->FireImpulse();
		Explosion();
		if (meshcomp->IsSimulatingPhysics()) 
		{
			meshcomp->AddImpulse((meshcomp->GetUpVector()+ meshcomp->GetRightVector()*0.5) * LaunchupPower ,NAME_None ,true);
		}
		
	}
}

void AExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AExplosiveBarrel, isExploded);
}

void AExplosiveBarrel::ExplosionMultiCast_Implementation()
{

}

