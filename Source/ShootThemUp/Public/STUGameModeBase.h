// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STUCoreTypes.h"
#include "GameFramework/GameModeBase.h"
#include "STUGameModeBase.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASTUGameModeBase();

	FOnMatchStateChangedSignature OnMatchStateChanged;

	virtual void StartPlay() override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	void Killed(AController* KillerController, AController* VictimController);

	FGameData GetGameData() const {return GameData;}
	int32 GetCurrentRoundNum() const {return  CurrentRound;}
	int32 GetRoundSecondsRemaining() const {return  RoundCountDown;}

	void RespawnRequest(AController* Controller);

	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	virtual bool ClearPause() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	TSubclassOf<APawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	FGameData GameData;

	UPROPERTY(EditAnywhere, Category="Game")
	int32 MinRoundTimeForRespawn = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game")
	int32 TeamsNum = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game")
	bool LastManStandingGameMode = false;

private:
	void SpawnBots();
	void StartRound();
	void GameTimerUpdate();
	void ResetPlayers();
	void ResetOnePlayer(AController* Controller);
	void CreateTeamsInfo();
	FLinearColor DetermineColorByTeamID(int32 TeamID) const;
	void SetPlayerColor(AController* Controller);

	int32 CurrentRound = 1;
	int32 RoundCountDown = 0;
	FTimerHandle GameRoundTimerHandle;
	TArray<FString> BotNames = {"Bot Yarik", "Bot Mosk", "Bot Kostya", "Bot Vanya", "Bot Ded", "Bot Bond", "Bot Kas", "Bot Sanya", "Bot Anton"};

	void LogPlayerInfo();
	void StartRespawn(AController* Controller);
	void LastManStandingGameOver();
	void GameOver();

	ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;
	void SetMatchState(ESTUMatchState State);

	void StopAllFire();
};
