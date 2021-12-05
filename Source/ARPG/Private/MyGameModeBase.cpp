// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "AI/AICharacter.h"
#include "Abilities/AttributeComponent.h"
#include "EngineUtils.h"
#include "CharacterBase.h"



AMyGameModeBase::AMyGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	
	MaxBotCount = 4.0;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
}


void AMyGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AMyGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AMyGameModeBase::OnQueryCompleted);
	}
}


void AMyGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
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
		else
		{
			// TODO: 积分结算等处理
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
		// TODO: AI角色死亡处理
		
	}
}