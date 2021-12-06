// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityComponent.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Ability.generated.h"

class UWorld;

/**
 * 
 */
UCLASS(Blueprintable)
class ARPG_API UAbility : public UObject
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "Action")
	UAbilityComponent* GetOwningComponent() const;

	/* Tags added to owning actor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/* Action can only start if OwningActor has none of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	void StartAbility(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ability")
	void StopAbility(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FName AbilityName;

	UWorld* GetWorld() const override;
};
