// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "AbilityComponent.generated.h"

class UAbility;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void AddAbility(AActor* Instigator, TSubclassOf<UAbility> AbilityClass);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void RemoveAbility(UAbility* AbilityToRemove);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool StartAbilityByName(AActor* Instigator, FName AbilityName);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool StopAbilityByName(AActor* Instigator, FName AbilityName);

	UAbilityComponent();


protected:

	UFUNCTION(Server, Reliable)
	void ServerStartAbility(AActor* Instigator, FName AbilityName);
	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UAbility>> DefaultAbilities;

	UPROPERTY()
	TArray<UAbility*> Abilities;
	
	virtual void BeginPlay() override;

public:
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
