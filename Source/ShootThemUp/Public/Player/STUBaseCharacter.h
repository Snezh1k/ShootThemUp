// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/STUHealthComponent.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "STUBaseCharacter.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTUBaseCharacter(const FObjectInitializer& ObjInit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnHealthChanged(float Health, float HealthDelta);
	virtual void OnDeath();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USTUHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USTUWeaponComponent* WeaponComponent;

	UPROPERTY(EditAnywhere, Category="Animations")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1500.0f);

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	UPROPERTY(EditDefaultsOnly, Category="Material")
	FName MaterialColorName = "Paint Color";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound")
	USoundCue* DeathSound;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void TurnOff() override;
	virtual void Reset() override;

	UFUNCTION(BlueprintCallable, Category="Movement")
	virtual bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category="Movement")
	float GetMovementDirection() const;

	void SetPlayerColor(const FLinearColor& Color);

private:
	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

};
