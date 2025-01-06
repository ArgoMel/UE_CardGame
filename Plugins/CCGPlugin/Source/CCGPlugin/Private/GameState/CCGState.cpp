// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/CCGState.h"

#include "CCGPlugin.h"
#include "BFL/CCGBFL.h"
#include "BFL/ControllerBFL.h"
#include "BFL/GameSnapshotBFL.h"
#include "Common/CCGStruct.h"
#include "GameInstance/CCGameInstance.h"
#include "GameMode/CCGMode.h"
#include "Gameplay/Card3D.h"
#include "Gameplay/CardPlacement.h"
#include "Gameplay/Graveyard.h"
#include "Interface/CardInteractionInterface.h"
#include "Interface/GameStateInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/CCGPlayerController.h"
#include "SaveGame/RecordGameStateSG.h"

ACCGState::ACCGState()
: mGameSeconds(1.f)
, bGameActive(false)
, bEnableBattleHistory(true)
, mCountdownTimer(4)
, mGameTime_Seconds(0)
, mGameTime_Minutes(0)
, mGameTurnState(EGameTurn::Waiting)
, mTurnDuration_Seconds(59)
, mTurnDuration_Minutes(2)
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
	CompilePlacementsPerPlayer();
	GetGraveyardReferencesPerPlayer();
}

void ACCGState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority()
		&&!mCardGameMode)
	{
		const UWorld* world = GetWorld();
		if (world)
		{
			mCardGameMode=Cast<ACCGMode>(world->GetAuthGameMode());
		}
	}
}

void ACCGState::ResetTurn()
{
	IF_RET_VOID(!mPlayerTurns.IsEmpty());
	for (const auto& controller:mCardGameMode->mGameControllersArray)
	{
		if (!controller->Implements<UGameStateInterface>())
		{
			continue;
		}
		const bool isStart=controller==mPlayerTurns[0];
		IGameStateInterface::Execute_ChangeActivePlayerTurn(controller,isStart);
		const int32 ID=UControllerBFL::GetControllerID(controller);
		if (isStart)
		{
			BeginPlayerTurn(ID);
		}
		else
		{
			EndOfPlayerTurn(ID);
		}
	}
	ResetTurnTimer();
}

void ACCGState::GameTimer()
{
	if (mGameTime_Seconds>=59)
	{
		++mGameTime_Minutes;
		mGameTime_Seconds=0;
	}
	else
	{
		++mGameTime_Seconds;
		if (mGameTime_Minutes>=30)
		{
			IF_RET_VOID(mCardGameMode);
			const int32 playerCount=mCardGameMode->mGameControllersArray.Num();
			TArray<EEndGameResults> results;
			results.Init(EEndGameResults::Draw, playerCount);
			Server_NotifyEndGameState(results);
		}
	}
}

void ACCGState::TurnTimer()
{
	if (mTurnTime_Seconds<=0)
	{
		--mTurnTime_Minutes;
		mTurnTime_Seconds=59;
	}
	else
	{
		--mTurnTime_Seconds;
	}
	if (mTurnTime_Minutes==0
		&&mTurnTime_Seconds==0)
	{
		Server_ForceChangeTurnState();
	}
}

void ACCGState::ResetTurnTimer()
{
	mTurnTime_Seconds=mTurnDuration_Seconds;
	mTurnTime_Minutes=mTurnDuration_Minutes;
}

bool ACCGState::CheckPlayerTurnState(EGameTurn GameTurnState) const
{
	return mGameTurnState==GameTurnState;
}

bool ACCGState::RequestChangeTurnState(AController* Controller)
{
	IF_RET_BOOL(!mPlayerTurns.IsEmpty());
	return Controller==mPlayerTurns[0];
}

void ACCGState::AddCardToBoard(ACard3D* CardReference, int32 PlayerID)
{
	if (HasAuthority())
	{
		mPlayerBoards[PlayerID].ActiveCards.AddUnique(CardReference);
		mCardReferenceArray.Add(CardReference);
		CardReference->SetCardID(PlayerID);
	}
}

void ACCGState::RemoveCardOnBoard(ACard3D* CardReference, int32 PlayerID)
{
	if (HasAuthority())
	{
		mPlayerBoards[PlayerID].ActiveCards.Remove(CardReference);
	}
}

void ACCGState::CompilePlacementsPerPlayer()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(this,ACardPlacement::StaticClass(),actors);
	for (AActor* actor:actors)
	{
		ACardPlacement* placement=Cast<ACardPlacement>(actor);
		if (placement)
		{
			const int32 index=placement->GetPlayerIndex();
			mPlayerBoards[index].CardPlacements.Add(placement);
			mPlayerBoards[index].TotalCardPlacementPositions+=placement->GetMaxCardsInPlacement();
		}
	}
}

void ACCGState::GetGraveyardReferencesPerPlayer()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(this,AGraveyard::StaticClass(),actors);
	for (AActor* actor:actors)
	{
		AGraveyard* graveyard=Cast<AGraveyard>(actor);
		if (graveyard)
		{
			const int32 index=graveyard->GetPlayerIndex();
			mPlayerBoards[index].Graveyards=graveyard;
		}
	}
}

AGraveyard* ACCGState::GetGraveyardReference(int32 PlayerID)
{
	const bool isValidIndex=mPlayerBoards.Num()<=PlayerID;
	IF_RET_NULL(isValidIndex);
	return mPlayerBoards[PlayerID].Graveyards;
}

int32 ACCGState::GetTotalCardPlacementPos(int32 PlayerID)
{
	const bool isValidIndex=mPlayerBoards.Num()<=PlayerID;
	IF_RET_BOOL(isValidIndex);
	return mPlayerBoards[PlayerID].TotalCardPlacementPositions;
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

void ACCGState::RotatePlayerTurn()
{
	IF_RET_VOID(!mPlayerTurns.IsEmpty());
	mPlayerTurns.Add(mPlayerTurns[0]);
	mPlayerTurns.RemoveAt(0);
}

void ACCGState::EndOfPlayerTurn(int32 PlayerID)
{
	if (HasAuthority())
	{
		TArray<ACard3D*> cards;
		GetActivePlayerCards(PlayerID, cards);
		for (const auto& card : cards)
		{
			if (card->Implements<UCardInteractionInterface>())
			{
				ICardInteractionInterface::Execute_OnEndActivePlayerTurn(card);
			}
		}
	}
}

void ACCGState::BeginPlayerTurn(int32 PlayerID)
{
	if (HasAuthority())
	{
		TArray<ACard3D*> cards;
		GetActivePlayerCards(PlayerID, cards);
		for (const auto& card : cards)
		{
			if (card->Implements<UCardInteractionInterface>())
			{
				ICardInteractionInterface::Execute_OnActivePlayerTurn(card);
			}
		}
	}
}

void ACCGState::RecordGameStateSnapshot()
{
	FGameStateSnapshot gameStateSnapshot;
	UGameSnapshotBFL::GetGameStateSnapshot(this,gameStateSnapshot);
	mGameSnapshot.Add(gameStateSnapshot);
	URecordGameStateSG* saveGame=Cast<URecordGameStateSG>(UGameplayStatics::CreateSaveGameObject(URecordGameStateSG::StaticClass()));
	if (saveGame)
	{
		saveGame->mSnapshot=mGameSnapshot;
		UGameplayStatics::SaveGameToSlot(saveGame,CCG_SaveSlotName::HappySnaps,0);
	}
}

void ACCGState::RecordBattleHistory(FBattleHistory BattleHistoryStruct)
{
	if (!bEnableBattleHistory)
	{
		return;
	}
	mBattleHistory.Add(BattleHistoryStruct);
}

void ACCGState::Server_OnGameStart_Implementation()
{
	bGameActive=true;
	IF_RET_VOID(mCardGameMode);
	mPlayerAndAIStates=mCardGameMode->mPlayerAndAIStates;
	GetWorldTimerManager().SetTimer(mGameTH,this,&ACCGState::GameTimer,mCardGameMode->GetGameSecond(),true);
	GetWorldTimerManager().SetTimer(mTurnTH,this,&ACCGState::TurnTimer,mCardGameMode->GetGameSecond(),true);

	for (auto& controller:mCardGameMode->mGameControllersArray)
	{
		mPlayerTurns.AddUnique(controller);
	}
	UCCGBFL::ShuffleArray(mPlayerTurns,FMath::Rand()%10000);
	for (const auto& playerTurn:mPlayerTurns)
	{
		if (playerTurn->Implements<UGameStateInterface>())
		{
			IGameStateInterface::Execute_MatchBegin(playerTurn);
		}
	}
	ResetTurn();
}

void ACCGState::Server_RequestChangeTurnState_Implementation(AController* Controller)
{
	if (RequestChangeTurnState(Controller))
	{
		Server_ForceChangeTurnState();
	}
}

void ACCGState::Server_ForceChangeTurnState_Implementation()
{
	if (bGameActive)
	{
		RotatePlayerTurn();
		ResetTurn();
	}
}

void ACCGState::Server_NotifyEndGameState_Implementation(const TArray<EEndGameResults>& array)
{
	bGameActive=false;
	GetWorldTimerManager().ClearTimer(mTurnTH);
	GetWorldTimerManager().ClearTimer(mGameTH);
	IF_RET_VOID(mCardGameMode);
	int32 index=0;
	for (const auto& controller:mCardGameMode->mGameControllersArray)
	{
		if (controller->Implements<UGameStateInterface>())
		{
			IGameStateInterface::Execute_MatchEnd(controller,array[index]);
		}
		++index;
	}
	UCCGameInstance* gameInstance= Cast<UCCGameInstance>(GetGameInstance());
	IF_RET_VOID(gameInstance);
	gameInstance->ShowMainMenuEvent();
}
