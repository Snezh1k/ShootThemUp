// Fill out your copyright notice in the Description page of Project Settings.


#include "STUGameModeBase.h"

#include "EngineUtils.h"
#include "STUBaseCharacter.h"
#include "STUGameHUD.h"
#include "STUGameInstance.h"
#include "STUPlayerController.h"
#include "STUPlayerState.h"
#include "STURespawnComponent.h"
#include "STUUtils.h"

ASTUGameModeBase::ASTUGameModeBase()
{
	DefaultPawnClass = ASTUBaseCharacter::StaticClass();
	PlayerControllerClass = ASTUPlayerController::StaticClass();
	HUDClass = ASTUGameHUD::StaticClass();
	PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
	Super::StartPlay();
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *GetWorld()->GetGameInstance<USTUGameInstance>()->TestString)
	SpawnBots();
	CreateTeamsInfo();
	CurrentRound = 1;
	StartRound();
	SetMatchState(ESTUMatchState::InProgress);
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if(InController && InController->IsA<AAIController>())
	{
		return AIPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
	const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
	const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;
	if(KillerPlayerState)
	{
		KillerPlayerState->AddKill();
	}
	if(VictimPlayerState)
	{
		VictimPlayerState->AddDeath();
	}
	StartRespawn(VictimController);
}

void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
	ResetOnePlayer(Controller);
}

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
	if(PauseSet)
	{
		StopAllFire();
		SetMatchState(ESTUMatchState::Pause);
	}
	return PauseSet;
}

bool ASTUGameModeBase::ClearPause()
{
	const auto PauseCleared = Super::ClearPause();
	if(PauseCleared)
	{
		SetMatchState(ESTUMatchState::InProgress);
	}
	return PauseCleared;
}

void ASTUGameModeBase::SpawnBots()
{
	if(!GetWorld()) return;

	for(int32 i = 0; i < GameData.PlayersNum - 1; ++i)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		const auto STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
		RestartPlayer(STUAIController);
	}
}

void ASTUGameModeBase::StartRound()
{
	RoundCountDown = GameData.RoundTime;
	GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
	//UE_LOG(LogTemp, Warning, TEXT("Time: %i / Round: %i / %i"), RoundCountDown, CurrentRound, GameData.RoundsNum);
	if(--RoundCountDown == 0)
	{
		GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);
		if(CurrentRound + 1 <= GameData.RoundsNum)
		{
			++CurrentRound;
			ResetPlayers();
			StartRound();
		}
		else
		{
			GameOver();
		}
	}
	if(LastManStandingGameMode)
	{
		if(GetWorld()->GetNumControllers() == 1)
		{
			GameOver();
		}
		else if(GetWorld()->GetNumControllers() == 2)
		{
			LastManStandingGameOver();
		}
	}
}

void ASTUGameModeBase::ResetPlayers()
{
	if(!GetWorld()) return;
	for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		ResetOnePlayer(It->Get());
	}
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
	const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
	if(!LastManStandingGameMode || (LastManStandingGameMode && PlayerState->GetDeathsNum() < 3))
	{
		if(Controller && Controller->GetPawn())
		{
			Controller->GetPawn()->Reset(); 
		}
		RestartPlayer(Controller);
		SetPlayerColor(Controller);
	}
	else if(LastManStandingGameMode && PlayerState->GetTeamID() != 1 && PlayerState->GetDeathsNum() == 3)
	{
		Controller->Destroy();
	}
}

void ASTUGameModeBase::CreateTeamsInfo()
{
	if(!GetWorld()) return;
	int32 TeamID = 1;
	for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if(!Controller) continue;
		const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
		if(!PlayerState) continue;
		PlayerState->SetTeamID(TeamID);
		PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID));
		PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : BotNames[It.GetIndex() - 1]);
		SetPlayerColor(Controller);
		if(TeamID + 1 > TeamsNum) TeamID = 1;
		else
		{
			TeamID += 1;
		}
	}
}

FLinearColor ASTUGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
	if(TeamID - 1 < GameData.TeamColors.Num())
	{
		return GameData.TeamColors[TeamID-1];
	}
	UE_LOG(LogTemp, Warning, TEXT("No color!"))
	return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
	if(!Controller) return;
	const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
	if(!Character) return;

	const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
	if(!PlayerState) return;
	Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUGameModeBase::LogPlayerInfo()
{
	if(!GetWorld()) return;
	for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if(!Controller) continue;
		const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
		if(!PlayerState) continue;
		PlayerState->LogInfo();
	}
}

void ASTUGameModeBase::StartRespawn(AController* Controller)
{
	const auto RespawnAvailable = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
	if(!RespawnAvailable) return;
	const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller);
	if(!RespawnComponent) return;
	RespawnComponent->Respawn(GameData.RespawnTime);
}

void ASTUGameModeBase::LastManStandingGameOver()
{
	if(!GetWorld()) return;
	for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if(!Controller) continue;
		const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
		if(!PlayerState) continue;
		if(PlayerState->GetDeathsNum() >= 3)
		{
			if(!Controller->IsPlayerController())
			{
				Controller->Destroy();
			}
			else
			{
				PlayerState->SetInvisible(true);
			}
			GameOver();
		}
	}
}

void ASTUGameModeBase::GameOver()
{
	//UE_LOG(LogTemp, Warning, TEXT("GAME OVER!"))
	//LogPlayerInfo();
	for(auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		if(Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(nullptr);
		}
	}
	SetMatchState(ESTUMatchState::GameOver);
}

void ASTUGameModeBase::SetMatchState(ESTUMatchState State)
{
	if(MatchState == State) return;
	MatchState = State;
	OnMatchStateChanged.Broadcast(MatchState);
}

void ASTUGameModeBase::StopAllFire()
{
	for(auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Pawn);
		if(!WeaponComponent) continue;

		WeaponComponent->StopFire();
		WeaponComponent->Zoom(false);
	}
}
