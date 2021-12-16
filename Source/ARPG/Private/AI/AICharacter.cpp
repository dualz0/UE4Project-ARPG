// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AICharacter.h"
#include "Abilities/AttributeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "BrainComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "HealthUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"

AAICharacter::AAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
    
    AttributeComp = CreateDefaultSubobject<UAttributeComponent>("AttributeComp");
		
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    
    TimeToHitParamName = "HitFlashTime";
}

void AAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &AAICharacter::OnHealthChanged);

	AttributeComp->SetLife(1);
	// 怪物血量随时间累计增强
	AttributeComp->SetMaxHealth(AttributeComp->GetHealth() + UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()));
	AttributeComp->SetHealthFull();
}


void AAICharacter::OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}
		
		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UHealthUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		
		if (NewHealth <= 0.0f)
		{
			// stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			// ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			
			SetLifeSpan(10.0f);
		}
	}
}


void AAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}


void AAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
}
