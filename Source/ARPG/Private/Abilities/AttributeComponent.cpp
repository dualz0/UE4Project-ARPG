// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AttributeComponent.h"
#include "MyGameModeBase.h"
#include "Net/UnrealNetwork.h"

UAttributeComponent::UAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
	
	LifeMax = 3;
	Life = LifeMax;

	SetIsReplicatedByDefault(true);
}


bool UAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	
	float ActualDelta = Health - OldHealth;
	// OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if (ActualDelta != 0.0f)
	{
		MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
	}
	
	if (Health == 0 && Life > 0)
	{
		ApplyLifeChange(InstigatorActor, -1);

		AMyGameModeBase* GM = GetWorld()->GetAuthGameMode<AMyGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), this, InstigatorActor);
		}
	}
	
	return ActualDelta != 0;
}

bool UAttributeComponent::ApplyLifeChange(AActor* InstigatorActor, float Delta)
{
	if (Life <= 0)
	{
		return false;
	}
	Life--;
	// OnLifeChanged.Broadcast(InstigatorActor, this);
	MulticastLifeChanged_Implementation(InstigatorActor, Delta);

	AMyGameModeBase* GM = GetWorld()->GetAuthGameMode<AMyGameModeBase>();
	if (GM)
	{
		
	}

	return true;
}

bool UAttributeComponent::SetLife(int32 NewLife)
{
	if (0 <= NewLife && NewLife <= LifeMax)
	{
		Life = NewLife;
		return true;
	}
	return false;
}

float UAttributeComponent::GetHealth() const
{
	return Health;
}

float UAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

void UAttributeComponent::SetHealthFull()
{
	Health = HealthMax;
}

bool UAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

bool UAttributeComponent::IsAlive() const
{
	// return Life > 0;
	return Health > 0;
}

float UAttributeComponent::GetLife() const
{
	return Life;
}

float UAttributeComponent::GetLifeMax() const
{
	return LifeMax;
}

UAttributeComponent* UAttributeComponent::GetAttributeComp(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UAttributeComponent>(FromActor->GetComponentByClass(UAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool UAttributeComponent::IsActorAlive(AActor* Actor)
{
	UAttributeComponent* AttributeComp = GetAttributeComp(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

void UAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void UAttributeComponent::MulticastLifeChanged_Implementation(AActor* InstigatorActor, float Delta)
{
	OnLifeChanged.Broadcast(InstigatorActor, this);
}

bool UAttributeComponent::IsPlayer() const
{
	return bIsPlayer;
}

void UAttributeComponent::SetIsPlayer(bool IsPlayer)
{
	bIsPlayer = IsPlayer;
}


void UAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAttributeComponent, Health);
	DOREPLIFETIME(UAttributeComponent, HealthMax);
}