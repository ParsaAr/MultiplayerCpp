// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class APawn;

//Notes : We declare delegates here, first parameter is the type of event (delegate type) , other parameters are the input types and names , there is Kama between each type and name 
//based on the ue4 macro system , the delegate type must start with F , the number of parmas should be identified in the declaration of the macro (six here) , events are exposable to ue editor with the U property Bp assignable macro 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSigniture, class UHealthComponent*, HealthComp, float, currentHealth, float, DeltaHealth, const class UDamageType*  ,DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthCHANGENotifySigniture, float, C_health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetKilledSigniture, class APawn*, KilledPawn, class APawn*, Killer);
UCLASS( ClassGroup=(COOP), Blueprintable , meta=(BlueprintSpawnableComponent) )
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_HealthChangeEvent, BlueprintReadWrite)
		float CurrentHealth;

	UFUNCTION()
		void OnRep_HealthChangeEvent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHealth;

	UFUNCTION()
		void OnDamageRecieve(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedSigniture OnHealthChangedEvent;
	
	UPROPERTY(BlueprintAssignable)
		FHealthCHANGENotifySigniture OnHealthChangeNotifyEvent;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable)
		FGetKilledSigniture onKilledEvent;

	bool iSdead;
};
