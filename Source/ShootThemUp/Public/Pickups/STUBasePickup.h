// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "STUBasePickup.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUBasePickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(VisibleAnywhere, Category="Pickup")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category="Pickup")
	float RespawnTime = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundCue* PickupTakenSound;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool CouldBeTaken() const;
	
private:
	float RotationYaw = 0.0f;
	FTimerHandle RespawnTimerHandle;
		
	virtual bool GivePickupTo(APawn* PlayerPawn);
	void PickupWasTaken();
	void Respawn();
	void GenerateRotationYaw();
};
