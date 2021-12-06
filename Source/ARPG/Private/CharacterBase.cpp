// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealthUserWidget.h"
#include "Abilities/AbilityComponent.h"
#include "Abilities/AttributeComponent.h"


ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
    SpringArmComp->SetupAttachment(RootComponent);

    CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
    CameraComp->SetupAttachment(SpringArmComp);
	
	AttributeComp = CreateDefaultSubobject<UAttributeComponent>("AttributeComp");
	AttributeComp->SetIsPlayer(true);
	
	AbilityComp = CreateDefaultSubobject<UAbilityComponent>("AbilityComp");
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ACharacterBase::OnHealthChanged);
	AttributeComp->OnLifeChanged.AddDynamic(this, &ACharacterBase::OnLifeChanged);

	ActiveHealthBar = CreateWidget<UHealthUserWidget>(GetWorld(), HealthBarWidgetClass);
	ActiveHealthBar->AttachedActor = this;
	ActiveHealthBar->AddToViewport();
}

void ACharacterBase::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void ACharacterBase::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ACharacterBase::PrimaryAttack()
{
	AbilityComp->StartAbilityByName(this, "PrimaryAttack");
}

void ACharacterBase::BlackHoleAttack()
{
	AbilityComp->StartAbilityByName(this, "BlackHole");
}

void ACharacterBase::Dash()
{
	AbilityComp->StartAbilityByName(this, "Dash");
}

void ACharacterBase::SprintStart()
{
	AbilityComp->StartAbilityByName(this, "Sprint");
}

void ACharacterBase::SprintStop()
{
	AbilityComp->StopAbilityByName(this, "Sprint");

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ACharacterBase::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ACharacterBase::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ACharacterBase::Dash);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACharacterBase::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACharacterBase::SprintStop);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
}

void ACharacterBase::OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}
}

void ACharacterBase::OnLifeChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp)
{
	// APlayerController* PC = Cast<APlayerController>(GetController());
	// DisableInput(PC);
	
	if (AttributeComp->GetLife() > 0)
	{
		// reborn
		// AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax());

		// ActiveHealthBar = CreateWidget<UHealthUserWidget>(GetWorld(), HealthBarWidgetClass);
		// ActiveHealthBar->AttachedActor = this;
		// ActiveHealthBar->AddToViewport();
	}
	else
	{
		// GameOver
		
	}
} 