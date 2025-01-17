// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/CCGAIController.h"

#include "CCGPlugin.h"
#include "AI/CCGAIPawn.h"
#include "AI/EvaluationRuleBFL.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BFL/CardBFL.h"
#include "BFL/CardInteractionBFL.h"
#include "BFL/CardsInHandBFL.h"
#include "BFL/CCGBFL.h"
#include "BFL/ControllerBFL.h"
#include "BFL/DeckBFL.h"
#include "BFL/MathBFL.h"
#include "BFL/PlacementBFL.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/CCGMode.h"
#include "Gameplay/Card3D.h"
#include "GameState/CCGState.h"
#include "Interface/WidgetInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ACCGAIController::ACCGAIController()
: mMaxCardsInHand(7)
, bShuffleDeck(true)
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
	LoadAIPersonality(CCG_Default::DefaultAIPersonality);
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
	UWorld* world=GetWorld();
	IF_RET_NULL(world);
	IF_RET_NULL(mCard3DClass);
	FActorSpawnParameters spawnParameters;
	spawnParameters.Owner=this;
	spawnParameters.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	spawnParameters.TransformScaleMethod=ESpawnActorScaleMethod::OverrideRootScale;
	return world->SpawnActor<ACard3D>(mCard3DClass,SpawnTransform,spawnParameters);
}

UMaterialInterface* ACCGAIController::GetProfileImg_Implementation()
{
	//서브시스템에서 데이터에셋에서 받아오기
	return IControllerInterface::GetProfileImg_Implementation();
}

int32 ACCGAIController::GetCurrentPlayerIndex_Implementation() const
{
	IF_RET(CCG_PlayerIndex::InvalidIndex,mBoardPlayer);
	return mBoardPlayer->GetPlayerIndex();
}

void ACCGAIController::GetPlayerDeck_Implementation(TArray<FName>& Deck)
{
	Deck=mAIDeck;
}

bool ACCGAIController::RemoveCardFromDeck_Implementation(bool RemoveAll, int32 IndexToRemove)
{
	UDeckBFL::RemoveCardFromDeck(IndexToRemove,mAIDeck,RemoveAll);
	UpdateAIPlayerState();
	return true;
}

void ACCGAIController::MatchEnd_Implementation(EEndGameResults Result)
{
	bTurnActive=false;
	GetBlackboardComponent()->SetValueAsBool(CCG_BB::TurnActive,false);
}

void ACCGAIController::MatchBegin_Implementation()
{
	if (!HasAuthority())
	{
		return;
	}
	Execute_DrawCard(this,FName(),false,mCardsInFirstHand);
	UpdateAIState();
}

void ACCGAIController::ChangeActivePlayerTurn_Implementation(bool TurnActive)
{
	if (!HasAuthority())
	{
		return;
	}
	bTurnActive=TurnActive;
	GetBlackboardComponent()->SetValueAsBool(CCG_BB::TurnActive,bTurnActive);
	if (bTurnActive)
	{
		mControlledPawn->IncreaseTurn();
		Execute_DrawCard(this,FName(),false,1);
		UpdateAIPlayerState();
	}
}

void ACCGAIController::RequestChangePlayerTurn_Implementation()
{
	IF_RET_VOID(mGameState);
	if (mGameState->RequestChangeTurnState(this))
	{
		mGameState->Server_RequestChangeTurnState(this);
	}
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

FAIState ACCGAIController::GetAISate(int32 PlayerIndex) const
{
	FAIState aiState;
	TArray<ACard3D*> activeCards;
	mGameState->GetActivePlayerCards(PlayerIndex,activeCards);
	const FAITotalScore activeCardScore=UEvaluationRuleBFL::RuleCalculateActiveCardPoints(0,mAIPointAllocation.Evaluation.PerActiveCard,activeCards,mAIPointAllocation.Evaluation.bIncludeActiveCardValue);
		
	FPlayerStat playerStat;
	UControllerBFL::GetControllersStateStat(this,PlayerIndex,playerStat);
	const FAITotalScore handScore=UEvaluationRuleBFL::RuleCalculateCardsInHandPoints(activeCardScore.Total,mAIPointAllocation.Evaluation.PerCardInHand,playerStat.CardsInHand);
		
	const FAITotalScore healthScore=UEvaluationRuleBFL::RuleCalculateHealthPoints(handScore.Total,mAIPointAllocation.Evaluation.PerHealthPoint,playerStat.Health);

	aiState.CardOnBoardState=activeCardScore.RuleTotal;
	aiState.CardInHandState=handScore.RuleTotal;
	aiState.HealthState=healthScore.RuleTotal;
	aiState.Total=healthScore.Total;
	return aiState;
}

void ACCGAIController::OnRep_AICardsInHand()
{
	UpdateAIState();
	UpdateAIPlayerState();
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
	if (CardName.IsNone())
	{
		return false;
	}
	const FCard card=UDeckBFL::GetCardData(CardName,CardSet);
	IF_RET_BOOL(mControlledPawn);
	int32 id=0;
	switch (card.PlacementSetting.Owner)
	{
	case ECardPlayers::Self:
		id=mControlledPawn->GetPlayerId();
		break;
	case ECardPlayers::Opponent:
		id=mGameState->GetRandomOpponentIndex(this);
		break;
	case ECardPlayers::SelfAndOpponent:
		id=mGameState->GetRandomOpponentIndex();
		break;
	default: ;
	}
	TArray<ACardPlacement*> placements;
	if (!UPlacementBFL::GetPlayersAvailablePlacementsForCard(this,id,CardName,placements))
	{
		return false;
	}
	if (mControlledPawn->Implements<UPlayerStateInterface>())
	{
		IPlayerStateInterface::Execute_DecreasePlayerMana(mControlledPawn,card.PlacementSetting.ManaCost);
	}
	Execute_RemoveCardFromHand(this,CardName,0,false);
	FVector pawnPos=mControlledPawn->GetActorLocation();
	pawnPos.Z=100.f;
	ACard3D* card3D=Execute_CreatePlayableCard(this,FTransform(pawnPos));
	card3D=UCardBFL::SetupCard(card3D,mControlledPawn->GetPlayerId(),CardName,CardSet);
	return UCardBFL::AddCardToBoardPlacement(card3D,placements[FMath::Rand()%placements.Num()],mControlledPawn->GetPlayerId());
}

bool ACCGAIController::RunCardInteraction(const FCardInteraction& AICardInteraction)
{
	bool successDamage=false;
	ACard3D* talkingCard=Cast<ACard3D>(AICardInteraction.TalkingCard);
	IF_RET_BOOL(talkingCard);
	int32 cost=0;
	
	if (!UCardBFL::ChargeCardManaCost(this,mControlledPawn->GetPlayerId(),false,*talkingCard->GetCardData(),EManaAccount::Attack,cost))
	{
		return successDamage;
	}
	
	switch (AICardInteraction.InteractionType)
	{
	case EPlayType::AttackCard:
		{
			ACard3D* receivingCard=Cast<ACard3D>(AICardInteraction.ReceivingActor);
			IF_RET_BOOL(receivingCard);
			EInteractionConditions callingCardCondition;
			EInteractionConditions talkingCardCondition;
			successDamage=UCardInteractionBFL::DealDamageToCard(false,talkingCard,receivingCard,0,callingCardCondition,talkingCardCondition);
		}
		break;
	case EPlayType::AttackPlayer:
		{
			ABoardPlayer* receivingPlayer=Cast<ABoardPlayer>(AICardInteraction.ReceivingActor);
			IF_RET_BOOL(receivingPlayer);
			EInteractionConditions receivingPlayerCondition;
			successDamage=UCardInteractionBFL::DealDamageToPlayer(false,talkingCard,receivingPlayer,0,receivingPlayerCondition);
		}
		break;
	default: ;
	}
	if (successDamage)
	{
		UpdateAIState();
	}
	return successDamage;
}

void ACCGAIController::UpdateAIPlayerState() const
{
	mControlledPawn->Multicast_ForceUIUpdate(mAICardsInHand.Num(),mAIDeck.Num());
}

bool ACCGAIController::EvaluateBoardState()
{
	mPriorityFocusList.Empty();
	FAIState curEvaluationStateOpponent;
	IF_RET_BOOL(mGameState);
	for (int32 i=0;i<mGameState->mPlayerAndAIStates.Num(); ++i)
	{
		if (mGameState->GetCurPlayerTurn()==this)
		{
			continue;
		}
		const FAIState evaluationStateOpponent=GetAISate(i);
		if (curEvaluationStateOpponent.Total<evaluationStateOpponent.Total)
		{
			curEvaluationStateOpponent=evaluationStateOpponent;
		}
	}
	IF_RET_BOOL(mControlledPawn);
	const FAIState evaluationStateSelf=GetAISate(mControlledPawn->GetPlayerId());
	
	mEvaluationStateTotal.BoardStateCompValue=UMathBFL::PercentageOfInt32(evaluationStateSelf.CardOnBoardState,curEvaluationStateOpponent.CardOnBoardState);
	mEvaluationStateTotal.InHandStateCompValue=UMathBFL::PercentageOfInt32(evaluationStateSelf.CardInHandState,curEvaluationStateOpponent.CardInHandState);
	mEvaluationStateTotal.HealthCompValue=UMathBFL::PercentageOfInt32(evaluationStateSelf.HealthState,curEvaluationStateOpponent.HealthState);
	mEvaluationStateTotal.Total=UMathBFL::PercentageOfInt32(evaluationStateSelf.Total,curEvaluationStateOpponent.Total);

	CalculatePriorityFocus();
	if (mAIBarWidget
		&&mAIBarWidget->Implements<UWidgetInterface>())
	{
		IWidgetInterface::Execute_SetValue(mAIBarWidget,mEvaluationStateTotal);
	}
	return true;
}

void ACCGAIController::UpdateAIState()
{
	if (!HasAuthority())
	{
		return;
	}
	TArray<FName> playableCardsInHand;
	UCardsInHandBFL::FindPlayableCardsInHand(this,playableCardsInHand);
	UCardsInHandBFL::SortCardsByFocus(playableCardsInHand,mPriorityFocusList,true,mPlayableCardsInHandList);
	IF_RET_VOID(mGameState);
	IF_RET_VOID(mControlledPawn);
	TArray<ACard3D*> playerCards;
	mGameState->GetActivePlayerCards(mControlledPawn->GetPlayerId(),playerCards);
	UCardBFL::FilterCardsWithTurnPoints(playerCards,mValidInteractionTypes,mCardsOnBoardWithTurnPoints);
	EvaluateBoardState();
	UpdateAIPlayerState();
}

EPriority ACCGAIController::GetFocusPriority(EAIPersonalityFocus Focus, float& ReturnPriorityValue)
{
	IF_RET(EPriority::None,mControlledPawn);
	IF_RET(EPriority::None,mGameMode);
	
	float lowValue=1.f;
	ReturnPriorityValue=1.f;
	switch (Focus)
	{
	case EAIPersonalityFocus::IncreasePlayerHealth:
		lowValue=mHealth_Low;
		ReturnPriorityValue=static_cast<float>(mControlledPawn->mAIStat.Health)/mGameMode->mCardGameOption.PlayerStartingHealth;
		break;
	case EAIPersonalityFocus::DamageCards:
		{
			lowValue=mBoardState_Low;
			TArray<ACard3D*> opponentCards;
			mGameState->GetActivePlayerCards(mGameState->GetRandomOpponentIndex(this),opponentCards);
			const int32 total=mGameState->GetTotalCardPlacementPos(mGameState->GetRandomOpponentIndex(this));
			ReturnPriorityValue=1.f-(opponentCards.Num()/total);
		}
		break;
	case EAIPersonalityFocus::DamageOpponentPlayer:
		lowValue=0.5f;
		ReturnPriorityValue=mEvaluationStateTotal.Total;
		break;
	case EAIPersonalityFocus::PlayingCards:
		{
			lowValue=mBoardState_Low;
			TArray<ACard3D*> playerCards;
			mGameState->GetActivePlayerCards(mControlledPawn->GetPlayerId(),playerCards);
			const int32 total=mGameState->GetTotalCardPlacementPos(mControlledPawn->GetPlayerId());
			ReturnPriorityValue=1.f-(playerCards.Num()/total);
		}
		break;
	case EAIPersonalityFocus::PickingUpCards:
		lowValue=mCardsInHand_Low;
		ReturnPriorityValue=mControlledPawn->mAIStat.CardsInHand/static_cast<float>(mMaxCardsInHand);
		if(mPlayableCardsInHandList.Num()<=2)
		{
			ReturnPriorityValue/=1.3f;
		}
		break;
	case EAIPersonalityFocus::OpponentHindrance:
		lowValue=mBoardState_Low;
		ReturnPriorityValue=mEvaluationStateTotal.Total;
		break;
	default: ;
	}
	if (mPriorityFocusList.Contains(Focus))
	{
		ReturnPriorityValue/=2.f;
	}
	ReturnPriorityValue=FMath::Clamp(ReturnPriorityValue,0.f,1.f);

	if (ReturnPriorityValue<=lowValue)
	{
		return EPriority::High;
	}
	if (ReturnPriorityValue<=lowValue*2.f)
	{
		return EPriority::Medium;
	}
	return EPriority::Low;
}

void ACCGAIController::CalculatePriorityFocus()
{
	TArray<EAIPersonalityFocus> tempAIFocusPriority;
	TArray<EPriority> tempPriorityListArray;
	TArray<float> priorityValueArray;

	for (const EAIPersonalityFocus focus : TEnumRange<EAIPersonalityFocus>())
	{
		float priorityValue;
		EPriority priority;
		if (focus!=EAIPersonalityFocus::IncreasePlayerHealth)
		{
			int32 i;
			for (i=0;i< tempAIFocusPriority.Num();++i)
			{
				priority=GetFocusPriority(focus,priorityValue);
				if ((tempPriorityListArray[i]==priority
					&&priorityValue<=priorityValueArray[i])
					||tempPriorityListArray[i]>priority)
				{
					tempAIFocusPriority.Insert(focus,i);
					tempPriorityListArray.Insert(priority,i);
					priorityValueArray.Insert(priorityValue,i);
					break;
				}
			}
			if (i==tempAIFocusPriority.Num()-1)
			{
				tempAIFocusPriority.Add(focus);
				priority=GetFocusPriority(focus,priorityValue);
				tempPriorityListArray.Add(priority);
				priorityValueArray.Add(priorityValue);
			}
		}
		else
		{
			tempAIFocusPriority.Add(focus);
			priority=GetFocusPriority(focus,priorityValue);
			tempPriorityListArray.Add(priority);
			priorityValueArray.Add(priorityValue);
		}
	}
	mPriorityFocusList=tempAIFocusPriority;
}

void ACCGAIController::LoadAIPersonality(FName RowName)
{
	mAIPersonality=*mAIPersonalityData->FindRow<FAIPersonality>(RowName,TEXT("CardGameAIController: Failed to get AI 'Personality' data table row"));
	IF_RET_VOID(mControlledPawn);
	mControlledPawn->SetAIName(mAIPersonality.AIName.ToString());
	mAIPointAllocation=*mAIPointAllocationData->FindRow<FPointAllocation>(mAIPersonality.AIPointAllocation,nullptr);
	GetBlackboardComponent()->SetValueAsEnum(CCG_BB::Difficulty,static_cast<uint8>(mAIPersonality.Difficulty));
	GetBlackboardComponent()->SetValueAsFloat(CCG_BB::PlaySpeed,mAIPersonality.PlaySpeed);
	GetBlackboardComponent()->SetValueAsEnum(CCG_BB::TurnState,static_cast<uint8>(ETurnState::TurnActive));
}

void ACCGAIController::SetupAI()
{
	const FName selectedDeck=mAIPersonality.PlayableDecks[FMath::Rand()%mAIPersonality.PlayableDecks.Num()];
	UCardBFL::LoadPreBuiltDeck(selectedDeck,mAIDeck);
	
	if (bShuffleDeck)
	{
		UCCGBFL::ShuffleArray(mAIDeck);
	}
	if (bEnableWeightedCards)
	{
		UDeckBFL::FilterWeightedCardsInDeck(mAIDeck);
	}
}

void ACCGAIController::CreateAIDebugUI()
{
	if (bShowDebugUI)
	{
		if (UCCGBFL::CanExecuteCosmeticEvents(this))
		{
			mAIBarWidget=CreateWidget(UGameplayStatics::GetPlayerController(this,0),mAIBarClass);
			if (mAIBarWidget->Implements<UWidgetInterface>())
			{
				IWidgetInterface::Execute_SetParent(mAIBarWidget,this);
			}
			mAIBarWidget->AddToViewport(CCG_ZOrder::AIBar);
		}
	}
	UpdateAIState();
}
