// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	Health = 100;
	HealthMax = 100;
}


bool UAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	// TODO: Test
	USkeletalMeshComponent* MeshComp = Cast<USkeletalMeshComponent>(GetOwner()->FindComponentByClass(USkeletalMeshComponent::StaticClass()));
	if (MeshComp)
	{
		MeshComp->SetScalarParameterValueOnMaterials("HitFlashTime", GetWorld()->TimeSeconds);
	}
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