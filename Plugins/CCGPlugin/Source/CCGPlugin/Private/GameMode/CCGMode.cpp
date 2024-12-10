// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/CCGMode.h"

#include "BFL/ControllerBFL.h"
#include "Common/CCGConstVar.h"
#include "GameFramework/PlayerState.h"
#include "GameState/CCGState.h"
#include "PlayerController/CCGPlayerController.h"
#include "PlayerState/CCGPlayerState.h"

ACCGMode::ACCGMode()
: mCountdownTimer(4)
, mGameSeconds(1)
, bSkipStartTimer(true)
, bGameActive(false)
, mMana_Min(1)
, mMana_Max(10)
, mPlayerStartingHealth(30)
, mMaxNumOfPlayers(2)
, bSingleClientEnabled(false)
, bSpectator(false)
{
}

void ACCGMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	const UWorld* world = GetWorld();
	if (!world||
		world->GetMapName()==CCG_Level::MainMenu)
	{
		return;
	}
	if (mGameControllersArray.Num()>=mMaxNumOfPlayers||
		bSpectator)
	{
		SetBoardPlayerReference();
	}
	else
	{
		AController* controller= AddPlayerToArray(NewPlayer->PlayerState,NewPlayer);
		SetCardGamePlayerId(NewPlayer->PlayerState,controller);
		SetBoardPlayerReference();
		SetBoardPlayerReferences(controller);
		ACCGPlayerController* playerController;
		ACCGPlayerState* nullPlayerState;
		UControllerBFL::GetPlayerControllerReference(controller,playerController,nullPlayerState);
		if (playerController)
		{
			playerController->Client_PostLogin();
			playerController->Server_SetupDeck();
		}
	}
	CheckGamePreConditions();
}

void ACCGMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	RemovePlayerFromGame(Exiting);
}

void ACCGMode::BeginPlay()
{
	Super::BeginPlay();
}

void ACCGMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!mGameState)
	{
		mGameState=GetGameState<ACCGState>();
	}
}

void ACCGMode::CollectGameResults(TArray<EEndGameResults>& PlayerResults)
{
}

bool ACCGMode::CheckIsPlayerActive(int32 ControllerID)
{
	return false;
}

void ACCGMode::FinishCountdown()
{
	GetWorldTimerManager().ClearTimer(mCountdownTH);
	bGameActive=true;
	mGameState->OnGameStart();
}

int32 ACCGMode::CalculateManaForTurn(int32 PlayerTurn)
{
	return 0;
}

int32 ACCGMode::GetTurnMana(AController* Controller)
{
	return 0;
}

void ACCGMode::SetBoardPlayerReference()
{
}

void ACCGMode::GetPlayerControllers(TArray<AController*>& Players)
{
}

void ACCGMode::SetCardGamePlayerId(UObject* PlayerState, AController* Controller)
{
}

AController* ACCGMode::AddPlayerToArray(AActor* PlayerState, AController* PlayerController)
{
	return nullptr;
}

void ACCGMode::SetBoardPlayerReferences(AController*& Controller)
{
}

void ACCGMode::RemovePlayerFromGame(AController* Controller)
{
}

void ACCGMode::CreateCardGameAIOpponent()
{

}

void ACCGMode::CheckGamePreConditions_Implementation()
{
	if (mGameControllersArray.Num()!=mMaxNumOfPlayers&&
		bSingleClientEnabled)
	{
		ForceSpawnAIOpponent();
	}
	else
	{
		GetWorldTimerManager().SetTimer(mCountdownTH,this,&ThisClass::GameStartCountdown,mGameSeconds,!bSkipStartTimer);
	}
}

void ACCGMode::ForceSpawnAIOpponent_Implementation()
{
	CreateCardGameAIOpponent();
	GetWorldTimerManager().SetTimer(mCountdownTH,this,&ThisClass::GameStartCountdown,mGameSeconds,!bSkipStartTimer);
}

void ACCGMode::GameStartCountdown_Implementation()
{
	if (!bSkipStartTimer)
	{
		--mCountdownTimer;
		for (const auto& gameController:mGameControllersArray)
		{
			ACCGPlayerController* playerController=Cast<ACCGPlayerController>(gameController);
			if (playerController)
			{
				playerController->SetCountdownTimer(mCountdownTimer);
			}
		}
	}
	if (mCountdownTimer<1||
		bSkipStartTimer)
	{
		FinishCountdown();
	}
}

void ACCGMode::CheckPlayerState_Implementation()
{
	for (int32 i=1;i<=mMaxNumOfPlayers;++i)
	{
		if (!CheckIsPlayerActive(i))
		{
			EndGame();
			break;
		}
	}
}

void ACCGMode::EndGame_Implementation()
{
	bGameActive=false;
	TArray<EEndGameResults> playerResults;
	CollectGameResults(playerResults);
	mGameState->Server_NotifyEndGameState(playerResults);
}
