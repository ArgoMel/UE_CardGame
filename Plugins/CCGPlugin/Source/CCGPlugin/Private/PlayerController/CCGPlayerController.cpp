// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController/CCGPlayerController.h"

#include "Net/UnrealNetwork.h"

ACCGPlayerController::ACCGPlayerController()
{
}

void ACCGPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mHitCard);
	DOREPLIFETIME(ThisClass,mTargetDragSelectionActor);
	DOREPLIFETIME(ThisClass,bIsDragging);
	DOREPLIFETIME(ThisClass,bIsCardSelected);
	
	DOREPLIFETIME(ThisClass,mBoardPlayer);
	
	DOREPLIFETIME(ThisClass,mPlayerDeck);
	
	DOREPLIFETIME(ThisClass,tLocation);
	
	DOREPLIFETIME(ThisClass,bPlayCardSuccess);
	
	DOREPLIFETIME(ThisClass,mTurnState);
	
	DOREPLIFETIME(ThisClass,mCardSet);
	DOREPLIFETIME(ThisClass,bTurnActive);
	
	DOREPLIFETIME(ThisClass,mCardsInHand);
	DOREPLIFETIME(ThisClass,mCardToAdd);
	DOREPLIFETIME(ThisClass,mCardSetRef);
	
	DOREPLIFETIME(ThisClass,bSkipManaCheck);
}

void ACCGPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ACCGPlayerController::OnRep_bTurnActive()
{
}

void ACCGPlayerController::OnRep_TurnState()
{
}

void ACCGPlayerController::Server_SetupDeck_Implementation()
{
}
void ACCGPlayerController::Server_SpawnAIOpponent_Implementation()
{
}

void ACCGPlayerController::Client_PostLogin_Implementation()
{
}

void ACCGPlayerController::Client_SetCountdownTimer_Implementation(int32 Time)
{
}

void ACCGPlayerController::Client_EndMatchState_Implementation(EEndGameResults Result)
{
}

void ACCGPlayerController::Client_UpdateGameUI_Implementation(bool ForceCleanUpdate)
{
}