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
	Health += Delta;
	if (Health < 0)
	{
		Health = 0;
		MinusLife();
	}
	else if (Health > HealthMax)
	{
		Health = HealthMax;
	}

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
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
	return Health > 0.0f;
}

float UAttributeComponent::GetLife() const
{
	return Life;
}

float UAttributeComponent::GetLifeMax() const
{
	return LifeMax;
}