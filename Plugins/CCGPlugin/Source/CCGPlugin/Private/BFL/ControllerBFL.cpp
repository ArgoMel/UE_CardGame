// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/ControllerBFL.h"

#include "AI/CCGAIController.h"
#include "AI/CCGAIPawn.h"
#include "PlayerController/CCGPlayerController.h"
#include "PlayerState/CCGPlayerState.h"
#include "GameMode/CCGMode.h"

int32 UControllerBFL::GetControllerID(AController* Controller)
{
	return 0;
}

bool UControllerBFL::GetPlayerControllerReference(AController* Controller, ACCGPlayerController*& CardGamePlayerController, ACCGPlayerState*& CardGamePlayerState)
{
	CardGamePlayerController=Cast<ACCGPlayerController>(Controller);
	if (!CardGamePlayerController)
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
	if (!CardGameAIController)
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

int32 UControllerBFL::GetControllersStateProfile(const UObject* WorldContextObject,int32 ControllerID)
{
	AController* controller= GetControllerReferenceFromID(WorldContextObject,ControllerID);
	ACCGPlayerController* playerController;
	ACCGPlayerState* playerState;
	if (GetPlayerControllerReference(controller,playerController,playerState))
	{
		return 0;
	}
	ACCGAIController* AIController;
	ACCGAIPawn* AIPawn;
	if (GetAiControllerReference(controller,AIController,AIPawn))
	{
		return 0;
	}
	return 0;
}

void UControllerBFL::GetControllerIDForBothPlayers(AController* Controller, TArray<int32>& Players)
{
}

AActor* UControllerBFL::GetControllerPlayerState(int32 ControllerID)
{
	return nullptr;
}

bool UControllerBFL::GetReplicatedPlayerState(int32 PlayerID, int32& Temp)
{
	return false;
}

int32 UControllerBFL::GetOpponentID(int32 PlayerID)
{
	return 0;
}
