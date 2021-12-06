// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability.h"
#include "Ability_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;



/**
 * 
 */
UCLASS()
class ARPG_API UAbility_ProjectileAttack : public UAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* CastingEffect;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;
	
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:

	virtual void StartAbility_Implementation(AActor* Instigator) override;

	UAbility_ProjectileAttack();
	
};
