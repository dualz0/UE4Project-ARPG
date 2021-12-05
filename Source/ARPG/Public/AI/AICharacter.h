// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "AICharacter.generated.h"

class UAttributeComponent;
class UPawnSensingComponent;
class UUserWidget;
class UHealthUserWidget;

UCLASS()
class ARPG_API AAICharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	AAICharacter();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	void SetTargetActor(AActor* NewTarget);
	
	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
    void OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAttributeComponent* AttributeComp;
	
	UHealthUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;


	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

};
