// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
	
	LifeMax = 3;
	Life = LifeMax;
}


bool UAttributeComponent::ApplyHealthChange(float Delta)
{
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	
	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);
	
	return ActualDelta != 0;
}

bool UAttributeComponent::MinusLife()
{
	if (Life < 0)
	{
		return false;
	}

	Life--;
	Health = HealthMax;
	OnLifeChanged.Broadcast(nullptr, this);
	
	return true;
}

float UAttributeComponent::GetHealth() const
{
	return Health;
}

float UAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool UAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

bool UAttributeComponent::IsAlive() const
{
	// return Health > 0.0f;
	return Life >= 0;
}

float UAttributeComponent::GetLife() const
{
	return Life;
}

float UAttributeComponent::GetLifeMax() const
{
	return LifeMax;
}