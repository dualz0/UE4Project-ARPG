// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AMyPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

/**
 * 
 */
UCLASS()
class ARPG_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	int32 Credits;

public:

	UFUNCTION(BlueprintCallable, Category = "PlayerState|Credits") // < Category|SubCategory
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "PlayerState|Credits")
	bool RemoveCredits(int32 Delta);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;
};