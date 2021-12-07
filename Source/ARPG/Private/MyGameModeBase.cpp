// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/AICharacter.h"
#include "Abilities/AttributeComponent.h"
#include "EngineUtils.h"
#include "CharacterBase.h"
#include "MyPlayerState.h"



AMyGameModeBase::AMyGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	
	MaxBotCount = 4.0;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	CreditsPerKill = 10;

	DesiredPickupCount = 10;
	RequiredPickupDistance = 2000;

	PlayerStateClass = AMyPlayerState::StaticClass();
}


void AMyGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AMyGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	if (ensure(PickupClasses.Num() > 0))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, PickupSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if (ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AMyGameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
}


void AMyGameModeBase::SpawnBotTimerElapsed()
{
	int32 AliveBotsNum = GetAliveBotsNum();

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	UE_LOG(LogTemp, Warning, TEXT("%d"), AliveBotsNum);
	
	
	if (AliveBotsNum >= MaxBotCount)
	{
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AMyGameModeBase::OnBotSpawnQueryCompleted);
	}
}


void AMyGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}

void AMyGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	
	while (SpawnCounter < DesiredPickupCount && Locations.Num() > 0)
	{
		// Pick a random location from remaining points.
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		// Remove to avoid picking again
		Locations.RemoveAt(RandomLocationIndex);

		// Check minimum distance requirement
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPickupDistance)
			{
				// too close, skip to next attempt
				bValidLocation = false;
				break;
			}
		}

		// Failed the distance test
		if (!bValidLocation)
		{
			continue;
		}

		// Pick a random powerup-class
		int32 RandomClassIndex = FMath::RandRange(0, PickupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PickupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);

		// Keep for distance checks
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}

int32 AMyGameModeBase::GetAliveBotsNum()
{
	int32 AliveBotsNum = 0;
	for (TActorIterator<AAICharacter> It(GetWorld()); It; ++It)
	{
		AAICharacter* Bot = *It;

		UAttributeComponent* AttributeComp = Cast<UAttributeComponent>(Bot->GetComponentByClass(UAttributeComponent::StaticClass()));
		if (AttributeComp && AttributeComp->IsAlive())
		{
			AliveBotsNum++;
		}
	}
	return AliveBotsNum;
}

void AMyGameModeBase::RespawnPlayerElapsed(ACharacterBase* Player, AController* Controller, UAttributeComponent* Attributes)
{
	if (ensure(Controller))
	{
		// Controller->UnPossess();
		// RestartPlayer(Controller);

		Player->EnableInput(Cast<APlayerController>(Controller));

		Attributes->SetHealthFull();
	}
}


void AMyGameModeBase::OnActorKilled(AActor* VictimActor, UAttributeComponent* VictimAttributes, AActor* Killer)
{
	
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));

	ACharacterBase* Player = Cast<ACharacterBase>(VictimActor);
	if (Player)
	{
		Player->DisableInput(Cast<APlayerController>(Player->GetController()));

		if (VictimAttributes->GetLife() > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("重生"));
			
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player, Player->GetController(), VictimAttributes);

			float RespawnDelay = 5.0f;
			GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
		}
	}
	
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		if (AMyPlayerState* PS = KillerPawn->GetPlayerState<AMyPlayerState>())
		{
			PS->AddCredits(CreditsPerKill);
		}
	}
}