// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	Health = 100;
	HealthMax = 100;
}


// void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick Tick, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);
// 	
// 	ApplyHealthChange(-1);
// }

bool UAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

float UAttributeComponent::GetHealth()
{
	return Health;
}

float UAttributeComponent::GetHealthMax()
{
	return HealthMax;
}