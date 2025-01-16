// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/CCGAIController.h"

#include "CCGPlugin.h"
#include "AI/CCGAIPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BFL/DeckBFL.h"
#include "GameMode/CCGMode.h"
#include "GameState/CCGState.h"
#include "Net/UnrealNetwork.h"

ACCGAIController::ACCGAIController()
: mDifficulty(EAIDifficulty::Easy)
, mRisk(EAIRisk::LowRisk)
, mAwareness(0)
, mPlaySpeed(0)
, mMaxCardsInHand(7)
, bAIReady(false)
, bShuffleDeck(true)
, bPrintAIDebugLogs(false)
, bTurnActive(false)
, bShowDebugUI(false)
, bEnableWeightedCards(false)
, mNumberOfCardsToAdd(0)
, mCardsInFirstHand(5)
, mCardPickupDelay(0.2f)
, mHealth_Low(0.1f)
, mCardsInHand_Low(0.2f)
, mBoardState_Low(0.3f)
{
	PrimaryActorTick.bCanEverTick = true;

	// for (const EAIPersonalityFocus focus : TEnumRange<EAIPersonalityFocus>())
	// {
	// 	mActivePriorityFocusList.Add(focus);
	// }
	mValidInteractionTypes.Add(ECardType::Creature);
	mValidInteractionTypes.Add(ECardType::Hero);
}

void ACCGAIController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mAICardsInHand);
	
	DOREPLIFETIME(ThisClass,mAIDeck);
}

void ACCGAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	mControlledPawn=Cast<ACCGAIPawn>(InPawn);
	UBlackboardComponent* blackboardComp = GetBlackboardComponent();
	UseBlackboard(mBlackBoardData,blackboardComp);
	RunBehaviorTree(mBehaviorTree);
	LoadAIPersonality(CCG_AIPersonality::Default);
	SetupAI();
	UpdateAIPlayerState();
	CreateAIDebugUI();
}

void ACCGAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACCGAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!mGameMode)
	{
		const UWorld* world = GetWorld();
		if (world)
		{
			mGameMode=Cast<ACCGMode>(world->GetAuthGameMode());
		}
	}
	if (!mGameState)
	{
		if (mGameMode)
		{
			mGameState=mGameMode->GetGameState<ACCGState>();
		}
	}
}

bool ACCGAIController::AddCardToPlayersHand_Implementation(FName CardName)
{
	mAICardsInHand.Add(CardName);
	UpdateAIPlayerState();
	return true;
}

bool ACCGAIController::RemoveCardFromHand_Implementation(FName Card, int32 Index, bool RemoveAll)
{
	if (!HasAuthority())
	{
		return false;
	}
	if (RemoveAll)
	{
		mAICardsInHand.Empty();
		UpdateAIState();
		return true;
	}
	if (mAICardsInHand.Contains(Card))
	{
		mAICardsInHand.Remove(Card);
		UpdateAIState();
		return true;
	}
	return false;
}

void ACCGAIController::DrawCard_Implementation(FName CardName, bool IgnoreMaxCards, int32 NumberOfCardsToDraw)
{
	if (!HasAuthority()
		||mAIDeck.Num()-mNumberOfCardsToAdd<=0)
	{
		return;
	}
	if (IgnoreMaxCards)
	{
		mNumberOfCardsToAdd+=NumberOfCardsToDraw;
		StartDrawCard(CardName);
	}
	else if (mAICardsInHand.Num()<mMaxCardsInHand)
	{
		if (NumberOfCardsToDraw+mNumberOfCardsToAdd+mAICardsInHand.Num()<=mMaxCardsInHand)
		{
			mNumberOfCardsToAdd+=NumberOfCardsToDraw;
			StartDrawCard(CardName);
		}
		else
		{
			mNumberOfCardsToAdd+=mMaxCardsInHand-(mAICardsInHand.Num()+mNumberOfCardsToAdd);
			StartDrawCard(CardName);
		}
	}
}

ACard3D* ACCGAIController::CreatePlayableCard_Implementation(FTransform SpawnTransform)
{
	return nullptr;
}

UMaterialInterface* ACCGAIController::GetProfileImg_Implementation()
{
	return IControllerInterface::GetProfileImg_Implementation();
}

void ACCGAIController::GetPlayerDeck_Implementation(TArray<FName>& Deck)
{
	IDeckInterface::GetPlayerDeck_Implementation(Deck);
}

bool ACCGAIController::RemoveCardFromDeck_Implementation(bool RemoveAll, int32 IndexToRemove)
{
	return IDeckInterface::RemoveCardFromDeck_Implementation(RemoveAll, IndexToRemove);
}

void ACCGAIController::MatchEnd_Implementation(EEndGameResults Result)
{
	IGameStateInterface::MatchEnd_Implementation(Result);
}

void ACCGAIController::MatchBegin_Implementation()
{
	IGameStateInterface::MatchBegin_Implementation();
}

void ACCGAIController::ChangeActivePlayerTurn_Implementation(bool TurnActive)
{
	IGameStateInterface::ChangeActivePlayerTurn_Implementation(TurnActive);
}

void ACCGAIController::RequestChangePlayerTurn_Implementation()
{
	IGameStateInterface::RequestChangePlayerTurn_Implementation();
}

void ACCGAIController::StartDrawCard(FName CardName)
{
	if (!mDrawCardTH.IsValid())
	{
		FTimerDelegate tempTimerDel;
		tempTimerDel.BindUFunction(this, FName("AddCardToHand"), CardName);
		GetWorld()->GetTimerManager().SetTimer(mDrawCardTH, tempTimerDel, mCardPickupDelay, true);
	}
}

void ACCGAIController::OnRep_AICardsInHand()
{
	
}

void ACCGAIController::AddCardToHand(FName CardName)
{
	if (mNumberOfCardsToAdd<=0)
	{
		GetWorld()->GetTimerManager().ClearTimer(mDrawCardTH);
		return;
	}

	--mNumberOfCardsToAdd;
	if (CardName.IsNone())
	{
		CardName=UDeckBFL::GetCardFromDeck(mAIDeck,0);
		Execute_RemoveCardFromDeck(this,false,0);
	}
	Execute_AddCardToPlayersHand(this,CardName);
	UpdateAIState();
	if (mNumberOfCardsToAdd<=0)
	{
		GetWorldTimerManager().ClearTimer(mDrawCardTH);
	}
}

bool ACCGAIController::AIPlayCard(FName CardName, ECardSet CardSet)
{
	return false;
}

bool ACCGAIController::RunCardInteraction(const FCardInteraction& AICardInteraction)
{
	return false;
}

void ACCGAIController::UpdateAIPlayerState()
{
}

bool ACCGAIController::EvaluateBoardState()
{
	return false;
}

void ACCGAIController::UpdateAIState()
{
}

EPriority ACCGAIController::GetFocusPriority(EAIPersonalityFocus Focus, float& ReturnPriorityValue)
{
	return EPriority::None;
}

void ACCGAIController::CalculatePriorityFocus()
{
}

void ACCGAIController::LoadAIPersonality(FName RowName)
{
}

void ACCGAIController::SetupAI()
{
}

void ACCGAIController::SetupAIDeck()
{
}

void ACCGAIController::CreateAIDebugUI()
{
}
