// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

class UAbility;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void AddAbility(TSubclassOf<UAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool StartAbilityByName(AActor* Instigator, FName AbilityName);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool StopAbilityByName(AActor* Instigator, FName AbilityName);

	UAbilityComponent();


protected:

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UAbility>> DefaultAbilities;

	UPROPERTY()
	TArray<UAbility*> Abilities;
	
	virtual void BeginPlay() override;

public:
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
