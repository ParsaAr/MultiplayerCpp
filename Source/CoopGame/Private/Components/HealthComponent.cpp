// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "math/UnrealMathUtility.h"
#include "net/UnrealNetwork.h"
#include <GameFramework/Pawn.h>
#include <GameFramework/Controller.h>
#include "TeamComponent.h"
// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.f;
	iSdead = false;
	SetIsReplicated(true);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	
	if (GetOwnerRole() ==  ROLE_Authority)
	{
		CurrentHealth = MaxHealth;
		AActor* Myowner = this->GetOwner();
		if (Myowner)
		{
			Myowner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnDamageRecieve);
		}
	}
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}


void UHealthComponent::OnRep_HealthChangeEvent()
{
	OnHealthChangeNotifyEvent.Broadcast(CurrentHealth);
}

void UHealthComponent::OnDamageRecieve(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	
	if (this->GetOwner() != DamageCauser &&  UTeamComponent::Check_Is_Friendly(this->GetOwner(), DamageCauser))
	{
		return;
	}
		
	if (Damage <= 0) {
		return;
	} 
	
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f , MaxHealth);
	
	OnHealthChangedEvent.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);
	if (CurrentHealth<=0 && !iSdead)
	{	
		iSdead = true;
		//PawnGotKilled(Cast<APawn>(GetOwner()), InstigatedBy->GetPawn(), DamageCauser);
		if (Cast<APawn>(GetOwner()) && InstigatedBy && InstigatedBy->GetPawn())
		{	
			onKilledEvent.Broadcast(Cast<APawn>(GetOwner()), InstigatedBy->GetPawn());
		}
	
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

