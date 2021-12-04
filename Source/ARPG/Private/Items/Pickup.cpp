// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Pickup.h"
#include "Abilities/AttributeComponent.h"
#include "Components/SphereComponent.h"

APickup::APickup()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Pickup");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnActorBeginOverlap);
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;
	HealValue = 50.0f;
}

void APickup::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UAttributeComponent* AttributeComp = Cast<UAttributeComponent>(OtherActor->GetComponentByClass(UAttributeComponent::StaticClass()));
		if (AttributeComp && AttributeComp->GetHealth() < AttributeComp->GetHealthMax())
		{
			AttributeComp->ApplyHealthChange(HealValue);
			HideAndCooldownPowerup();
		}
	}
}

void APickup::ShowPowerup()
{
	SetPowerupState(true);
}


void APickup::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &APickup::ShowPowerup, RespawnTime);
}

void APickup::SetPowerupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);
	
	RootComponent->SetVisibility(bNewIsActive, true);
}
