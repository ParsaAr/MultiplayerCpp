// Fill out copyright notice in the Description page of Project Settings.


#include "Scharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Gameframework/PawnMovementComponent.h"
#include "WeaponCpp.h"
#include "components/InputComponent.h"
#include "CoopGame.h"
#include "Components/CapsuleComponent.h"
#include "Private/components/HealthComponent.h"
#include "net/UnrealNetwork.h"


AScharacter::AScharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	isDead = false;
	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	SpringArmCam = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmCam->SetupAttachment(RootComponent);
	cam->SetupAttachment(SpringArmCam);
	this->GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	this->GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;
	initialZoomFOV = 90;
	this->GetCapsuleComponent()->SetCollisionResponseToChannel(BUllet_COl_Ch, ECR_Ignore);
	
}


void AScharacter::BeginPlay()
{
	Super::BeginPlay();
	targetFOV = initialZoomFOV;

	
	FActorSpawnParameters spawnparams;
	spawnparams.Owner = this;
	spawnparams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (Role == ROLE_Authority)
	{
		currentweapon = GetWorld()->SpawnActor<AWeaponCpp>(DeafultWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnparams);
		if (currentweapon) 
		{
			currentweapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachmentSocket);
		}
	
	}
	
	
}
	

void AScharacter::MoveFwd(float val)
{
	AddMovementInput(GetActorForwardVector() * val);
}


void AScharacter::MoveRight(float val)
{
	AddMovementInput(GetActorRightVector() * val);
	
}


void AScharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float curretnFOV = FMath::FInterpTo(cam->FieldOfView, targetFOV, DeltaTime, InterpolationSpeed);
	cam->SetFieldOfView(curretnFOV);
	

}


void AScharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	

	PlayerInputComponent->BindAxis("MoveFWD", this, &AScharacter::MoveFwd);
	PlayerInputComponent->BindAxis("MoveRight", this, &AScharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AScharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AScharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AScharacter::startCrouch);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &AScharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AScharacter::Jump);
	
	PlayerInputComponent->BindAction("Zoom", EInputEvent::IE_Pressed, this, &AScharacter::ZoomBegin);
	PlayerInputComponent->BindAction("Zoom", EInputEvent::IE_Released, this, &AScharacter::ZoomEnd);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AScharacter::fireCurrentWeapon);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AScharacter::StopfireCurrentWeapon);

	PlayerInputComponent->BindAction("Slot1", IE_Pressed, this, &AScharacter::slot1);
	
}

void AScharacter::startCrouch()
{
	Crouch();
	
	
}

void AScharacter::EndCrouch()
{
	UnCrouch();
	
	
}

void AScharacter::StarttoJump()
{
	Jump();
}

void AScharacter::EndtheJump()
{
	StopJumping();
}

FVector AScharacter::GetPawnViewLocation()  const
{
	if (cam) {
		return cam->GetComponentLocation();
	}
	else
	{
	 return	Super::GetPawnViewLocation();
	}
}

void AScharacter::ZoomBegin()
{
	targetFOV = initialZoomFOV + ZoomFOVamount;
	
}

void AScharacter::ZoomEnd()
{
	targetFOV = initialZoomFOV;
	
}

void AScharacter::fireCurrentWeapon()
{
	if (currentweapon)
	{
		currentweapon->StartFire();
	}
}

void AScharacter::StopfireCurrentWeapon()
{
	if (currentweapon)
	{
		currentweapon->StopFire();
	}
}

void AScharacter::slot1()
{
	if (currentweapon->StaticClass() != RifleClass)
	{
		currentweapon->Destroy();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		currentweapon =GetWorld()->SpawnActor<AWeaponCpp>(RifleClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (currentweapon)
		{
			currentweapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachmentSocket);
		}
	}         
}




void AScharacter::characterHealthChanged(class UHealthComponent* HealthComp, float currentHealth, float DeltaHealth, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (currentHealth<=0 && !isDead)
	{
		isDead = true;
		RemoveClientUI();
		this->GetMovementComponent()->StopMovementImmediately();
		this->DetachFromControllerPendingDestroy();
		this->SetLifeSpan(10.0f);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	}
}




void AScharacter::RemoveClientUI_Implementation()
{
	RemUI();
}

void AScharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AScharacter, currentweapon);
	DOREPLIFETIME(AScharacter, isDead);
}

