// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AbilityComponent.h"
#include "Abilities/Ability.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UAbility> AbilityClass : DefaultAbilities)
	{
		AddAbility(AbilityClass);
	}
}


void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

void UAbilityComponent::AddAbility(TSubclassOf<UAbility> AbilityClass)
{
	if (!ensure(AbilityClass))
	{
		return;
	}

	UAbility* NewAbility = NewObject<UAbility>(this, AbilityClass);
	if (ensure(NewAbility))
	{
		Abilities.Add(NewAbility);
	}
}


bool UAbilityComponent::StartAbilityByName(AActor* Instigator, FName AbilityName)
{
	for (UAbility* Ability : Abilities)
	{
		if (Ability && Ability->AbilityName == AbilityName)
		{
			if (!Ability->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *AbilityName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}
			
			Ability->StartAbility(Instigator);
			return true;
		}
	}

	return false;
}


bool UAbilityComponent::StopAbilityByName(AActor* Instigator, FName AbilityName)
{
	for (UAbility* Ability : Abilities)
	{
		if (Ability && Ability->AbilityName == AbilityName)
		{
			if (Ability->IsRunning())
			{
				Ability->StopAbility(Instigator);
				return true;
			}
		}
	}

	return false;
}