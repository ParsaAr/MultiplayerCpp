// Fill out copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Scharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AWeaponCpp;
class UInputComponent;
UCLASS()
class AScharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AScharacter();
	
protected:
	
	virtual void BeginPlay() override;

	void MoveFwd(float val);
	void MoveRight(float val);

public:	
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(VisibleAnywhere ,BlueprintReadOnly, Category = "camera")
	UCameraComponent* cam = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "camera")
	USpringArmComponent* SpringArmCam = nullptr;
	void startCrouch();
	void EndCrouch();
	void StarttoJump();
	void EndtheJump();

	virtual FVector GetPawnViewLocation() const override;

	void ZoomBegin();
	void ZoomEnd();

	float initialZoomFOV;

	UPROPERTY(EditAnywhere)
	float ZoomFOVamount;

	float targetFOV;

	UPROPERTY(EditAnywhere , meta= (ClampMin =0.1f , ClampMax=100.f))
	float InterpolationSpeed;

	UPROPERTY(VisibleAnywhere , Replicated)
	AWeaponCpp* currentweapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AWeaponCpp> RifleClass;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AWeaponCpp> DeafultWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponAttachmentSocket;

	UFUNCTION(BlueprintCallable)
		void fireCurrentWeapon();
	UFUNCTION(BlueprintCallable)
		void StopfireCurrentWeapon();

	void slot1();
	

	UFUNCTION(BlueprintCallable)
		void characterHealthChanged(class UHealthComponent* HealthComp, float currentHealth, float DeltaHealth, const class UDamageType* DamageType,
			class AController* InstigatedBy, AActor* DamageCauser);
	UFUNCTION(Client,Reliable)
		void RemoveClientUI();

	UFUNCTION(BlueprintImplementableEvent)
		void RemUI();
	UPROPERTY(VisibleAnywhere,Replicated,BlueprintReadWrite)
	bool isDead;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
