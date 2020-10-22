// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCpp.h"
#include "components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Scharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CoopGame.h"
#include "engine/Engine.h"
#include "timermanager.h"
#include "net/UnrealNetwork.h"


static int DebugVariableRayshooting = 0;
FAutoConsoleVariableRef CvarDebugRayshooting(TEXT("coop.ShowRay"), DebugVariableRayshooting, TEXT("Makes the shooting raycasts visible"), ECVF_Cheat);

// Sets default values
AWeaponCpp::AWeaponCpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SK_Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	RootComponent = SK_Weapon;
	hitparticle = deafulthitParticle;
	defaultDamage = 20;
	RateOfFire = 60;
	
	this->SetReplicates(true);
	ShootingDirection_Error_HalfRadius_Deg = 1.f;
}

// Called when the game starts or when spawned
void AWeaponCpp::BeginPlay()
{
	Super::BeginPlay();
	
	
	
}

// Called every frame
void AWeaponCpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponCpp::Fire()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, FString::Printf(TEXT("Some variable values: x: %f"),GetWorld()->TimeSeconds));
	
	numberofbulletsshot++;
	
	FVector EyesLocation;
	FRotator EyesRotatation;
	
	//updates last fire time after each shot
	LasttimeFired = GetWorld()->GetTimeSeconds();

	if (GetOwner()) {
		CharRef = Cast<AScharacter>(GetOwner());
		
		GetOwner()->GetActorEyesViewPoint(EyesLocation, EyesRotatation);

		if (CharRef)
		{
			EyesLocation = CharRef->cam->GetComponentLocation();
			APlayerController* pc = Cast<APlayerController>(CharRef->GetController());
			if (pc)
			{
				pc->ClientPlayCameraShake(camShakeClass, FMath::FRandRange(0.8f, 1.f));
			}
		}
		FHitResult hitR;
		FCollisionQueryParams HitQparams;
		HitQparams.AddIgnoredActor(this);
		HitQparams.AddIgnoredActor(GetOwner());
		HitQparams.bTraceComplex = true;
		HitQparams.bReturnPhysicalMaterial = true;
		// shooting spread error
		FVector ShootDir = EyesRotatation.Vector();
		float HalfRadError_Radians = FMath::DegreesToRadians(ShootingDirection_Error_HalfRadius_Deg);
		 ShootDir= FMath::VRandCone(ShootDir, HalfRadError_Radians, HalfRadError_Radians);
		if (GetWorld()->LineTraceSingleByChannel(hitR, EyesLocation, (EyesLocation + (ShootDir*10000)) , BUllet_COl_Ch ,HitQparams))
		{
			FireHitData.Hitpointt = hitR.ImpactPoint;
			FireHitData.ShotSomething = true;
			FireHitData.TracerEnd = hitR.ImpactPoint;
			float ActualDamage = defaultDamage;
			FireHitData.SurfaceTypee = UPhysicalMaterial::DetermineSurfaceType(hitR.PhysMaterial.Get());  //note : this has a weak pointer so we should get it
			if (FireHitData.SurfaceTypee== FLESH_VULNERABLE_S)
			{
				ActualDamage = defaultDamage * 4.f;
			}
			UGameplayStatics::ApplyPointDamage(hitR.GetActor(), ActualDamage, EyesRotatation.Vector(), hitR, GetOwner()->GetInstigatorController(), this->GetOwner(), DamgeTypeClass);	
		}else
		{
			FireHitData.TracerEnd = SK_Weapon->GetSocketLocation(MuzzleSockett) +(EyesRotatation.Vector() * 10000);
		}

		
		//Debug Draw is Activated by Console Command Variable
		if (DebugVariableRayshooting >0 ) {
			DrawDebugLine(GetWorld(), EyesLocation, (EyesLocation + (EyesRotatation.Vector() * 10000)), FColor::Red, false, 3.f, 0, 1.f);	
		}
		// since rep notify evets are not automatically called on th server we manually call them here
		if (Role == ROLE_Authority && GetNetMode() != NM_DedicatedServer)
		{
			OnRep_WeaponFireCosmetics();
		}
		
		
	}
	   
}



void AWeaponCpp::OnRep_WeaponFireCosmetics()
{
	//PrintTheString(2.0f, FString::Printf(TEXT("num :  %d"), numberofbulletsshot));
	UGameplayStatics::SpawnEmitterAttached(MuzzleParticle, SK_Weapon ,MuzzleSockett , FVector::ZeroVector,FRotator::ZeroRotator,FVector(2.f)) ;

	UParticleSystemComponent* beamParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticle, SK_Weapon->GetSocketLocation(MuzzleSockett), FRotator::ZeroRotator);
	beamParticleComp->SetVectorParameter(BeamParameterName, FireHitData.TracerEnd);

	if (FireHitData.ShotSomething)
	{
		
		switch (FireHitData.SurfaceTypee)
		{
		default:
			hitparticle = deafulthitParticle;
			break;
		case FLESH_DEFAULT_S:
			hitparticle = DefaultFleshHitParticle;
			break;
		case FLESH_VULNERABLE_S:
			hitparticle = VulnerableFleshHitParticle;
			break;
		}
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitparticle, FireHitData.Hitpointt, FRotator::ZeroRotator, true);
		
	}
	
	
}

void AWeaponCpp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponCpp, numberofbulletsshot );
	DOREPLIFETIME(AWeaponCpp, FireHitData );
	
}

void AWeaponCpp::StopFire_Implementation()
{
    GetWorldTimerManager().ClearTimer(ShootingTimer);
}

bool AWeaponCpp::StopFire_Validate()
{
	return true;
}


void AWeaponCpp::StartFire_Implementation()
{	
    float StartDelaytime =FMath::Max(LasttimeFired + (60 / RateOfFire) - GetWorld()->GetTimeSeconds() , 0.f);
	GetWorldTimerManager().SetTimer(ShootingTimer, this, &AWeaponCpp::Fire, 60 / RateOfFire, true,StartDelaytime);
}

bool AWeaponCpp::StartFire_Validate()
{
	return true;
}










