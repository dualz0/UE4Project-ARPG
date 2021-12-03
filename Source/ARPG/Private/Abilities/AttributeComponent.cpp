// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	Health = 100;
	HealthMax = 100;

	Life = 3;
	LifeMax = 3;
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

	// TODO: Test
	USkeletalMeshComponent* MeshComp = Cast<USkeletalMeshComponent>(GetOwner()->FindComponentByClass(USkeletalMeshComponent::StaticClass()));
	if (MeshComp)
	{
		MeshComp->SetScalarParameterValueOnMaterials("HitFlashTime", GetWorld()->TimeSeconds);
	}
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

float UAttributeComponent::GetHealth()
{
	return Health;
}

float UAttributeComponent::GetHealthMax()
{
	return HealthMax;
}

bool UAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

float UAttributeComponent::GetLife()
{
	return Life;
}

float UAttributeComponent::GetLifeMax()
{
	return LifeMax;
}