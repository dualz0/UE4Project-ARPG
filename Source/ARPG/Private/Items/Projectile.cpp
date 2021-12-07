// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Projectile.h"
#include "Abilities/AttributeComponent.h"
#include "Components/SphereComponent.h"



AProjectile::AProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnActorOverlap);

	DamageAmount = 20.0f;
}

void AProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UAttributeComponent* AttributeComp = Cast<UAttributeComponent>(OtherActor->GetComponentByClass(UAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			// AttributeComp->ApplyHealthChange(this, -DamageAmount);
			AttributeComp->ApplyHealthChange(GetInstigator(), -DamageAmount);

			Explode();
		}
	}
}