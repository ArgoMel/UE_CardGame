// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/CCGState.h"

#include "CCGPlugin.h"
#include "Common/CCGStruct.h"
#include "Net/UnrealNetwork.h"

ACCGState::ACCGState()
: mPlatform()
, mGameSeconds(0)
, bGameActive(false)
, bEnableBattleHistory(false)
, mCountdownTimer(0)
, mGameTime_Seconds(0)
, mGameTime_Minutes(0)
, mGameTurnState()
, mTurnDuration_Seconds(0)
, mTurnDuration_Minutes(0)
, mTurnTime_Seconds(0)
, mTurnTime_Minutes(0)
{
}

void ACCGState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mGameTime_Seconds);
	DOREPLIFETIME(ThisClass,mGameTime_Minutes);
	DOREPLIFETIME(ThisClass,bGameActive);

	DOREPLIFETIME(ThisClass,mGameTurnState);
	DOREPLIFETIME(ThisClass,mTurnTime_Seconds);
	DOREPLIFETIME(ThisClass,mTurnTime_Minutes);
	
	DOREPLIFETIME(ThisClass,mPlayerBoards);
	DOREPLIFETIME(ThisClass,mPlayerAndAIStates);
}

void ACCGState::BeginPlay()
{
	Super::BeginPlay();
}

void ACCGState::GameTimer()
{
}

void ACCGState::TurnTimer()
{
}

void ACCGState::ResetTurnTimer()
{
}

bool ACCGState::CheckPlayerTurnState(EGameTurn GameTurnState)
{
	return false;
}

bool ACCGState::RequestChangeTurnState(AController* Controller)
{
	return false;
}

void ACCGState::AddCardToBoard(ACard3D* CardReference, int32 PlayerID)
{
}

void ACCGState::RemoveCardOnBoard(ACard3D* CardReference, int32 PlayerID)
{
}

void ACCGState::GetBoardState(int32 PlayerID, TArray<ACard3D*>& PlayerCards, TArray<ACard3D*>& OpponentCards)
{
}

void ACCGState::CompilePlacementsPerPlayer()
{
}

int32 ACCGState::GetCardPlacementReferences(int32 PlayerID, TArray<ACardPlacement*>& PlacementArray)
{
	return 0;
}

void ACCGState::RotatePlayerTurn()
{
}

void ACCGState::EndOfPlayerTurn(int32 PlayerID)
{
}

void ACCGState::BeginPlayerTurn(int32 PlayerID)
{
}

void ACCGState::RecordGameStateSnapshot()
{
}

void ACCGState::RecordBattleHistory(FBattleHistory BattleHistoryStruct)
{
}

void ACCGState::GetGraveyardReferencesPerPlayer()
{
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

void ACCGState::Server_OnGameStart_Implementation()
{
}

void ACCGState::Server_RequestChangeTurnState_Implementation(AController* Controller)
{
}

void ACCGState::Server_ForceChangeTurnState_Implementation()
{
}

void ACCGState::Server_NotifyEndGameState_Implementation(const TArray<EEndGameResults>& array)
{
}
