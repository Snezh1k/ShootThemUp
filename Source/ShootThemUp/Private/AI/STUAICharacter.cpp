// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STUAICharacter.h"
#include "STUAIWeaponComponent.h"
#include "STUHealthBarWidget.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit):
Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = ASTUAIController::StaticClass();

	bUseControllerRotationYaw = false;
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void ASTUAICharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateHealthWidgetVisibility();
}

void ASTUAICharacter::BeginPlay()
{
	Super::BeginPlay();
	check(HealthWidgetComponent);
}

void ASTUAICharacter::OnDeath(){
	Super::OnDeath();

	const auto STUController = Cast<AAIController>(Controller);
	if(STUController && STUController->BrainComponent)
	{
		STUController->BrainComponent->Cleanup();
	}
}

void ASTUAICharacter::OnHealthChanged(float Health, float HealthDelta)
{
	Super::OnHealthChanged(Health, HealthDelta);

	const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
	if(!HealthBarWidget) return;
	HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}

void ASTUAICharacter::UpdateHealthWidgetVisibility()
{
	if(!GetWorld() || !GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->GetPawn()) return;
	const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
	HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}
