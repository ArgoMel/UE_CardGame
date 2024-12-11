// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/ControllerBFL.h"

#include "AI/CCGAIController.h"
#include "AI/CCGAIPawn.h"
#include "PlayerController/CCGPlayerController.h"
#include "PlayerState/CCGPlayerState.h"
#include "GameMode/CCGMode.h"
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

bool UControllerBFL::GetPlayerControllerReference(AController* Controller, ACCGPlayerController*& CardGamePlayerController, ACCGPlayerState*& CardGamePlayerState)
{
	CardGamePlayerController=Cast<ACCGPlayerController>(Controller);
	if (!CardGamePlayerController||
		!Controller)
	{
		return false;
	}
	CardGamePlayerState=Cast<ACCGPlayerState>(Controller->PlayerState);
	if (!CardGamePlayerState)
	{
		return false;
	}
	return true;
}

bool UControllerBFL::GetAiControllerReference(AController* Controller, ACCGAIController*& CardGameAIController, ACCGAIPawn*& CardGameAIPawn)
{
	CardGameAIController=Cast<ACCGAIController>(Controller);
	if (!Controller||
		!CardGameAIController)
	{
		return false;
	}
	CardGameAIPawn=Cast<ACCGAIPawn>(Controller->GetPawn());
	if (!CardGameAIPawn)
	{
		return false;
	}
	return true;
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
	if (!WorldContextObject)
	{
		return nullptr;
	}
	AController* controller= GetControllerReferenceFromID(WorldContextObject,ControllerID);
	if (!controller)
	{
		return nullptr;
	}
	ACCGPlayerController* playerController;
	ACCGPlayerState* playerState;
	if (GetPlayerControllerReference(controller,playerController,playerState))
	{
		PlayerStat=playerState->mPlayerStat;
		Deck=playerController->GetPlayerDeck();
		CardsInHand=playerController->GetCardsInHand();
		return playerState;
	}
	ACCGAIController* AIController;
	ACCGAIPawn* AIPawn;
	if (GetAiControllerReference(controller,AIController,AIPawn))
	{
		PlayerStat=AIPawn->mAIStat;
		Deck=AIController->GetAIDeck();
		CardsInHand=AIController->GetAICardsInHand();
		return AIPawn;
	}
	return nullptr;
}

int32 UControllerBFL::GetControllerIDs(const UObject* WorldContextObject,AController* Controller, TArray<int32>& Players)
{
	if (!WorldContextObject)
	{
		return -1;
	}
	const UWorld* world= WorldContextObject->GetWorld();
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
		Players.Init(1,gameMode->GetMaxNumOfPlayers());
		return 1;
	}
	for (int32 i=0;i<gameMode->GetMaxNumOfPlayers();++i)
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
	AController* controller= GetControllerReferenceFromID(WorldContextObject,ControllerID);
	if (!controller)
	{
		return nullptr;
	}
	ACCGPlayerController* playerController;
	ACCGPlayerState* playerState;
	if (GetPlayerControllerReference(controller,playerController,playerState))
	{
		return playerState;
	}
	ACCGAIController* AIController;
	ACCGAIPawn* AIPawn;
	if (GetAiControllerReference(controller,AIController,AIPawn))
	{
		return AIPawn;
	}
	return nullptr;
}

bool UControllerBFL::GetReplicatedPlayerState(const UWorld* World,int32 PlayerID,FString& PlayerName,FPlayerStat& PlayerStat)
{
	PlayerName=TEXT("No_Opponent");
	if (!World)
	{
		return false;
	}
	ACCGState* gameState=World->GetGameState<ACCGState>();
	if (!gameState)
	{
		return false;
	}
	if (!gameState->mPlayerAndAIStates[PlayerID-1])
	{
		return false;
	}
	const ACCGPlayerState* playerState=Cast<ACCGPlayerState>(gameState->mPlayerAndAIStates[PlayerID-1]);
	if (playerState)
	{
		PlayerName=playerState->GetPlayerName();
		PlayerStat=playerState->mPlayerStat;
		return true;
	}
	const ACCGAIPawn* AIPawn=Cast<ACCGAIPawn>(gameState->mPlayerAndAIStates[PlayerID-1]);
	if (AIPawn)
	{
		PlayerName=AIPawn->GetAIName();
		PlayerStat=AIPawn->mAIStat;
		return true;
	}
	return false;
}
