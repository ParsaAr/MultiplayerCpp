// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileGrenade.h"
#include "kismet/GameplayStatics.h"
#include "components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
// Sets default values
AProjectileGrenade::AProjectileGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshcomp"));
	RootComponent = meshcomp;
	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
}

// Called when the game starts or when spawned
void AProjectileGrenade::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(mytimer ,this ,&AProjectileGrenade::explosion ,1.f ,false);
}

// Called every frame
void AProjectileGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AProjectileGrenade::explosion()
{	
	TArray<AActor*> ExplosionIgnoreActors;
	ExplosionIgnoreActors.Add(this);
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("boom"));
	UGameplayStatics::ApplyRadialDamage(this, 10.f, this->GetActorLocation(), 200, Damageclass, ExplosionIgnoreActors, this, GetInstigatorController(), true);
	//DrawDebugSphere(GetWorld(), GetActorLocation(), 200.f, 12, FColor::Orange, false, 1, 0.f, 1.f);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), exp_particle, this->GetActorLocation(), FRotator::ZeroRotator);
	this->Destroy();
}

