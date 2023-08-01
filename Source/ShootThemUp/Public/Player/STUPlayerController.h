// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "STURespawnComponent.h"
#include "GameFramework/PlayerController.h"
#include "STUPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASTUPlayerController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="AI")
	USTURespawnComponent* RespawnComponent;

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

private:
	void OnPauseGame();
	void OnMatchStateChanged(ESTUMatchState State);
	void OnMuteSound();
};
