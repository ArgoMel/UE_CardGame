// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/CCGState.h"

#include "CCGPlugin.h"
#include "Common/CCGStruct.h"
#include "Net/UnrealNetwork.h"

ACCGState::ACCGState()
{
}

void ACCGState::AddCardToBoard(ACard3D* CardReference, int32 PlayerID)
{
}

void ACCGState::RemoveCardOnBoard(ACard3D* CardReference, int32 PlayerID)
{
}

void ACCGState::RecordBattleHistory(FBattleHistory BattleHistoryStruct)
{
}

void ACCGState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mPlayerBoards);
	DOREPLIFETIME(ThisClass,mPlayerAndAIStates);
	
	DOREPLIFETIME(ThisClass,mGameTurnState);
}

AGraveyard* ACCGState::GetGraveyardReference(int32 PlayerID)
{
	return nullptr;
}

void ACCGState::GetAllPlayerCards(TArray<ACard3D*>& Cards)
{
	Cards.Empty();
	for (auto& playerBoard : mPlayerBoards)
	{
		Cards.Append(playerBoard.ActiveCards);
	}
}

void ACCGState::GetActivePlayerCards(int32 Index, TArray<ACard3D*>& Cards)
{
	const bool isValidIndex=mPlayerBoards.Num()<=Index;
	IF_RET_VOID(isValidIndex);
	Cards= mPlayerBoards[Index].ActiveCards;
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
