// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UAttributeComponent;
class UAnimMontage;
class UCameraComponent;
class UParticleSystem;
class USpringArmComponent;
class UHealthUserWidget;
class UUserWidget;
class UAbilityComponent;

UCLASS()
class ARPG_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;
	
	FTimerHandle TimerHandle_Reborn;
	
public:
	ACharacterBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAttributeComponent* AttributeComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAbilityComponent* AbilityComp;

	UHealthUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	void MoveForward(float Value);
	
	void MoveRight(float Value);

	void PrimaryAttack();
	
	void BlackHoleAttack();

	void Dash();

	void SprintStart();

	void SprintStop();
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnLifeChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp);

	virtual void PostInitializeComponents() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
