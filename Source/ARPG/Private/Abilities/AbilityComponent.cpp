// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AbilityComponent.h"
#include "Abilities/Ability.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UAbilityComponent::ServerStopAbility_Implementation(AActor* Instigator, FName AbilityName)
{
	StopAbilityByName(Instigator, AbilityName);
}

void UAbilityComponent::ServerStartAbility_Implementation(AActor* Instigator, FName AbilityName)
{
	StartAbilityByName(Instigator, AbilityName);
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<UAbility> AbilityClass : DefaultAbilities)
		{
			AddAbility(GetOwner(), AbilityClass);
		}
	}
}

bool UAbilityComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (UAbility* Ability : Abilities)
	{
		if (Ability)
		{
			WroteSomething |= Channel->ReplicateSubobject(Ability, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}


void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAbilityComponent::AddAbility(AActor* Instigator, TSubclassOf<UAbility> AbilityClass)
{
	if (!ensure(AbilityClass))
	{
		return;
	}

	// Skip for clients
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(AbilityClass));
		return;
	}

	UAbility* NewAbility = NewObject<UAbility>(GetOwner(), AbilityClass);
	if (ensure(NewAbility))
	{
		NewAbility->Initialize(this);
		
		Abilities.Add(NewAbility);
		
		if (NewAbility->bAutoStart && ensure(NewAbility->CanStart(Instigator)))
		{
			NewAbility->StartAbility(Instigator);
		}
	}
}

void UAbilityComponent::RemoveAbility(UAbility* AbilityToRemove)
{
	if (!ensure(AbilityToRemove && !AbilityToRemove->IsRunning()))
	{
		return;
	}

	Abilities.Remove(AbilityToRemove);
}

bool UAbilityComponent::StartAbilityByName(AActor* Instigator, FName AbilityName)
{
	for (UAbility* Ability : Abilities)
	{
		if (Ability && Ability->AbilityName == AbilityName)
		{
			if (!Ability->CanStart(Instigator))
			{
				continue;
			}

			if (!GetOwner()->HasAuthority())
			{
				ServerStartAbility(Instigator, AbilityName);
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
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAbility(Instigator, AbilityName);
				}

				Ability->StopAbility(Instigator);
				return true;
			}
		}
	}

	return false;
}

void UAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAbilityComponent, Abilities);
} 