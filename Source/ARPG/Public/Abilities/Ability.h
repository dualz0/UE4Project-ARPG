// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityComponent.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Ability.generated.h"

class UWorld;

USTRUCT()
struct FAbilityRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};


UCLASS(Blueprintable)
class ARPG_API UAbility : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
	UAbilityComponent* AbilityComp;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UAbilityComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FAbilityRepData RepData;

	UFUNCTION()
	void OnRep_RepData();
	
public:
	
	void Initialize(UAbilityComponent* NewAbilityComp);
	
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

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

	bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
