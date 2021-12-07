// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/AbilityEffect.h"

UAbilityEffect::UAbilityEffect()
{
	bAutoStart = true;
}


void UAbilityEffect::StartAbility_Implementation(AActor* Instigator)
{
	Super::StartAbility_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAbility", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}


void UAbilityEffect::StopAbility_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAbility_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	UAbilityComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAbility(this);
	}
}


void UAbilityEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{

}