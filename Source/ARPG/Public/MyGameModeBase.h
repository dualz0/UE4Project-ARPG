// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/AttributeComponent.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "MyGameModeBase.generated.h"

class ACharacterBase;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;


UCLASS()
class ARPG_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	FTimerHandle TimerHandle_SpawnBots;
	FTimerHandle TimerHandle_RespawnDelay;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	int32 CreditsPerKill;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	UEnvQuery* PickupSpawnQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TArray<TSubclassOf<AActor>> PickupClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	float RequiredPickupDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	int32 DesiredPickupCount;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	int32 MaxBotCount;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(ACharacterBase* Player, AController* Controller, UAttributeComponent* Attributes);

	UFUNCTION()
	int32 GetAliveBotsNum();
	
public:

	AMyGameModeBase();

	virtual void StartPlay() override;
	
	virtual void OnActorKilled(AActor* VictimActor, UAttributeComponent* VictimAttributes, AActor* Killer);
};
