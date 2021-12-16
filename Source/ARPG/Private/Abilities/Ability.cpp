// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Ability.h"
#include "Abilities/AbilityComponent.h"
#include "Net/UnrealNetwork.h"


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
	UAbilityComponent* Comp = GetOwningComponent();	
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}


void UAbility::StopAbility_Implementation(AActor* Instigator)
{
	UAbilityComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
}

UWorld* UAbility::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

UAbilityComponent* UAbility::GetOwningComponent() const
{
	return AbilityComp;
}

bool UAbility::IsRunning() const
{
	return RepData.bIsRunning;
}

void UAbility::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAbility(RepData.Instigator);
	}
	else
	{
		StopAbility(RepData.Instigator);
	}
}

void UAbility::Initialize(UAbilityComponent* NewAbilityComp)
{
	AbilityComp = NewAbilityComp;
}

void UAbility::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAbility, RepData);
	DOREPLIFETIME(UAbility, AbilityComp);
} 