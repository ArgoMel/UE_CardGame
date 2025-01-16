// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/CCGAIController.h"

#include "Net/UnrealNetwork.h"

ACCGAIController::ACCGAIController()
: mDifficulty()
, mRisk()
, mAwareness(0)
, mPlaySpeed(0)
, mMaxCardsInHand(0)
, bAIReady(false)
, bShuffleDeck(false)
, bPrintAIDebugLogs(false)
, bTurnActive(false)
, bShowDebugUI(false)
, bEnableWeightedCards(false)
, mNumberOfCardsToAdd(0)
, mCardsInFirstHand(0)
, mCardPickupDelay(0)
, mHealth_Low(0)
, mCardsInHand_Low(0)
, mBoardState_Low(0)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACCGAIController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mAIDeck);
	
	DOREPLIFETIME(ThisClass,mAICardsInHand);
}

void ACCGAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACCGAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACCGAIController::OnRep_AICardsInHand()
{
	
}