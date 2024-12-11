// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/CCGState.h"

#include "Net/UnrealNetwork.h"

ACCGState::ACCGState()
{
}

void ACCGState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mPlayerAndAIStates);
}

void ACCGState::OnGameStart_Implementation()
{
}

void ACCGState::Server_NotifyEndGameState(TArray<EEndGameResults>& array)
{
}
