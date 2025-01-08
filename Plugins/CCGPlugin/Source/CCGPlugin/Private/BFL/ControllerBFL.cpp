// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/ControllerBFL.h"

#include "CCGPlugin.h"
#include "AI/CCGAIController.h"
#include "AI/CCGAIPawn.h"
#include "PlayerController/CCGPlayerController.h"
#include "PlayerState/CCGPlayerState.h"
#include "GameMode/CCGMode.h"
#include "Gameplay/BoardPlayer.h"
#include "GameState/CCGState.h"

int32 UControllerBFL::GetControllerID(AController* Controller)
{
	if (!Controller)
	{
		return -1;
	}
	const ACCGPlayerController* playerController=Cast<ACCGPlayerController>(Controller);
	if (playerController)
	{
		const ACCGPlayerState* playerState=Controller->GetPlayerState<ACCGPlayerState>();
		if (playerState)
		{
			return playerState->GetCardGamePlayerId();
		}
	}
	const ACCGAIController* AIController=Cast<ACCGAIController>(Controller);
	if (AIController)
	{
		const ACCGAIPawn* AIPawn=AIController->GetPawn<ACCGAIPawn>();
		if (AIPawn)
		{
			return AIPawn->GetPlayerId();
		}
	}
	return -1;
}

AController* UControllerBFL::GetControllerReferenceFromID(const UObject* WorldContextObject,int32 ControllerID)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	const UWorld* world= WorldContextObject->GetWorld();
	if (!world)
	{
		return nullptr;
	}
	ACCGMode* gameMode= Cast<ACCGMode>(world->GetAuthGameMode());
	if (!gameMode)
	{
		return nullptr;
	}
	return gameMode->mGameControllersArray[ControllerID-1];
}

AActor* UControllerBFL::GetControllersStateProfile(const UObject* WorldContextObject,int32 ControllerID,FPlayerStat& PlayerStat,TArray<FName>& Deck,TArray<FName>& CardsInHand)
{
	IF_RET_NULL(WorldContextObject);
	AController* controller= GetControllerReferenceFromID(WorldContextObject,ControllerID);
	IF_RET_NULL(controller);
	ACCGPlayerController* playerController=Cast<ACCGPlayerController>(controller);
	ACCGPlayerState* playerState=controller->GetPlayerState<ACCGPlayerState>();
	if (playerController&&playerState)
	{
		PlayerStat=playerState->mPlayerStat;
		Deck=playerController->GetPlayerDeck();
		CardsInHand=playerController->GetCardsInHand();
		return playerState;
	}
	ACCGAIController* AIController=Cast<ACCGAIController>(controller);
	ACCGAIPawn* AIPawn=controller->GetPawn<ACCGAIPawn>();
	if (AIController&&AIPawn)
	{
		PlayerStat=AIPawn->mAIStat;
		Deck=AIController->GetAIDeck();
		CardsInHand=AIController->GetAICardsInHand();
		return AIPawn;
	}
	return nullptr;
}

AActor* UControllerBFL::GetControllersStateStat(const UObject* WorldContextObject, int32 ControllerID, FPlayerStat& PlayerStat)
{
	IF_RET_NULL(WorldContextObject);
	const AController* controller= GetControllerReferenceFromID(WorldContextObject,ControllerID);
	IF_RET_NULL(controller);
	ACCGPlayerState* playerState=controller->GetPlayerState<ACCGPlayerState>();
	if (playerState)
	{
		PlayerStat=playerState->mPlayerStat;
		return playerState;
	}
	ACCGAIPawn* AIPawn=controller->GetPawn<ACCGAIPawn>();
	if (AIPawn)
	{
		PlayerStat=AIPawn->mAIStat;
		return AIPawn;
	}
	return nullptr;
}

AActor* UControllerBFL::GetControllersStateDeck(const UObject* WorldContextObject, int32 ControllerID, TArray<FName>& Deck)
{
	IF_RET_NULL(WorldContextObject);
	AController* controller= GetControllerReferenceFromID(WorldContextObject,ControllerID);
	IF_RET_NULL(controller);
	ACCGPlayerController* playerController=Cast<ACCGPlayerController>(controller);
	if (playerController)
	{
		Deck=playerController->GetPlayerDeck();
		return playerController->PlayerState;
	}
	ACCGAIController* AIController=Cast<ACCGAIController>(controller);
	if (AIController)
	{
		Deck=AIController->GetAIDeck();
		return AIController->GetPawn();
	}
	return nullptr;
}

AActor* UControllerBFL::GetControllersStateCardsInHand(const UObject* WorldContextObject, int32 ControllerID, TArray<FName>& CardsInHand)
{
	IF_RET_NULL(WorldContextObject);
	AController* controller= GetControllerReferenceFromID(WorldContextObject,ControllerID);
	IF_RET_NULL(controller);
	ACCGPlayerController* playerController=Cast<ACCGPlayerController>(controller);
	if (playerController)
	{
		CardsInHand=playerController->GetCardsInHand();
		return playerController->PlayerState;
	}
	ACCGAIController* AIController=Cast<ACCGAIController>(controller);
	if (AIController)
	{
		CardsInHand=AIController->GetAICardsInHand();
		return AIController->GetPawn();
	}
	return nullptr;
}

int32 UControllerBFL::GetControllerIDs(AController* Controller, TArray<int32>& Players)
{
	if (!Controller)
	{
		return -1;
	}
	const UWorld* world= Controller->GetWorld();
	if (!world)
	{
		return -1;
	}
	const ACCGMode* gameMode= Cast<ACCGMode>(world->GetAuthGameMode());
	if (!gameMode)
	{
		return -1;
	}
	if (gameMode->mGameControllersArray.Num()<=1)
	{
		Players.Init(1,gameMode->mCardGameOption.MaxNumOfPlayers);
		return 1;
	}
	for (int32 i=0;i<gameMode->mCardGameOption.MaxNumOfPlayers;++i)
	{
		Players.Add(i+1);
	}
	return GetControllerID(Controller);
}

AActor* UControllerBFL::GetControllerPlayerState(const UObject* WorldContextObject,int32 ControllerID)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	const AController* controller= GetControllerReferenceFromID(WorldContextObject,ControllerID);
	if (!controller)
	{
		return nullptr;
	}
	ACCGPlayerState* playerState=controller->GetPlayerState<ACCGPlayerState>();
	if (playerState)
	{
		return playerState;
	}
	return controller->GetPawn<ACCGAIPawn>();
}

AActor* UControllerBFL::GetReplicatedPlayerState(const UObject* WorldContextObject,int32 PlayerID,FString& PlayerName,FPlayerStat& PlayerStat)
{
	PlayerName=TEXT("No_Opponent");
	IF_RET_NULL(WorldContextObject);
	const UWorld* world= WorldContextObject->GetWorld();
	IF_RET_NULL(world);
	ACCGState* gameState=world->GetGameState<ACCGState>();
	IF_RET_NULL(gameState);
	if (!gameState->mPlayerAndAIStates[PlayerID-1])
	{
		return nullptr;
	}
	const ACCGPlayerState* playerState=Cast<ACCGPlayerState>(gameState->mPlayerAndAIStates[PlayerID-1]);
	if (playerState)
	{
		PlayerName=playerState->GetPlayerName();
		PlayerStat=playerState->mPlayerStat;
		return gameState->mPlayerAndAIStates[PlayerID-1];
	}
	const ACCGAIPawn* AIPawn=Cast<ACCGAIPawn>(gameState->mPlayerAndAIStates[PlayerID-1]);
	if (AIPawn)
	{
		PlayerName=AIPawn->GetAIName();
		PlayerStat=AIPawn->mAIStat;
		return gameState->mPlayerAndAIStates[PlayerID-1];
	}
	return nullptr;
}

int32 UControllerBFL::GetOpponentIndex(const UObject* WorldContextObject)
{
	IF_RET(-1,WorldContextObject);
	const UWorld* world= WorldContextObject->GetWorld();
	IF_RET(-1,world);
	const ACCGPlayerController* playerController=world->GetFirstPlayerController<ACCGPlayerController>();
	IF_RET(-1,playerController);
	return playerController->GetOpponentIndex();
}

int32 UControllerBFL::GetPlayerIndex(const UObject* WorldContextObject)
{
	IF_RET(CCG_PlayerIndex::InvalidIndex,WorldContextObject);
	const UWorld* world= WorldContextObject->GetWorld();
	IF_RET(CCG_PlayerIndex::InvalidIndex,world);
	const ACCGPlayerController* playerController=world->GetFirstPlayerController<ACCGPlayerController>();
	IF_RET(CCG_PlayerIndex::InvalidIndex,playerController);
	const ABoardPlayer* boardPlayer= playerController->GetBoardPlayer();
	IF_RET(CCG_PlayerIndex::InvalidIndex,boardPlayer);
	return boardPlayer->mPlayerIndex;
}
