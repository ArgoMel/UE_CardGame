// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/CCGState.h"

#include "CCGPlugin.h"
#include "Common/CCGStruct.h"
#include "Net/UnrealNetwork.h"

ACCGState::ACCGState()
{
}

void ACCGState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mPlayerBoards);
	DOREPLIFETIME(ThisClass,mPlayerAndAIStates);
}

void ACCGState::GetCardPlacements(int32 Index, TArray<ACardPlacement*>& CardPlacements)
{
	const bool isValidIndex=mPlayerBoards.Num()<=Index;
	IF_RET_VOID(isValidIndex);
	CardPlacements= mPlayerBoards[Index].CardPlacements;
}

void ACCGState::OnGameStart_Implementation()
{
}

void ACCGState::Server_NotifyEndGameState(TArray<EEndGameResults>& array)
{
}
