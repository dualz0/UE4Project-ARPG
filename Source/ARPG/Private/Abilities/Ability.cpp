// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Ability.h"
#include "Abilities/AbilityComponent.h"



bool UAbility::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	UAbilityComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void UAbility::StartAbility_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	UAbilityComponent* Comp = GetOwningComponent();	
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}


void UAbility::StopAbility_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	
	ensureAlways(bIsRunning);

	UAbilityComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* UAbility::GetWorld() const
{
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}

UAbilityComponent* UAbility::GetOwningComponent() const
{
	return Cast<UAbilityComponent>(GetOuter());
}

bool UAbility::IsRunning() const
{
	return bIsRunning;
}