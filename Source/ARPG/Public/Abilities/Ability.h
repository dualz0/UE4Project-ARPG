// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Ability.generated.h"

class UWorld;

/**
 * 
 */
UCLASS(Blueprintable)
class ARPG_API UAbility : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	void StartAbility(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	void StopAbility(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FName AbilityName;

	UWorld* GetWorld() const override;
};
