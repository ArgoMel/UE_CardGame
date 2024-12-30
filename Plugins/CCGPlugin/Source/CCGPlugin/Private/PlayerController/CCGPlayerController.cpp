// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController/CCGPlayerController.h"

#include "CCGPlugin.h"
#include "BFL/CardBFL.h"
#include "BFL/CardInteractionBFL.h"
#include "BFL/CardsInHandBFL.h"
#include "BFL/CCGBFL.h"
#include "BFL/DeckBFL.h"
#include "BFL/MiscBFL.h"
#include "BFL/SaveGameBFL.h"
#include "Blueprint/UserWidget.h"
#include "GameInstance/CCGameInstance.h"
#include "GameMode/CCGMode.h"
#include "Gameplay/Card3D.h"
#include "Gameplay/CardPlacement.h"
#include "Gameplay/TargetDragSelection.h"
#include "GameState/CCGState.h"
#include "Interface/CardWidgetInterface.h"
#include "Interface/PlayerUIInterface.h"
#include "Interface/TimerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/CCGPlayerState.h"

ACCGPlayerController::ACCGPlayerController()
	: bCardPlayerStateDirty(false)
	, bIsDragging(false)
	, bIsCardSelected(false)
	, mCardPlayerState(ECardPlayerState::PendingAction)
	, mCardSet(ECardSet::BasicSet)
	, mMaxCardsInHand(7)
	, mCardsInFirstHand(5)
	, mCardsToDrawPerTurn(1)
	, bTurnActive(false)
	, mCardInHandIndex(0)
	, mCardHoldDistance(1700.)
	, bEnableInHandMovementRotation(true)
	, bIsValidClientDrop(false)
	, bPlayCardSuccess(false)
	, bEnabledMobileCardPreview(false)
	, mTurnState()
	, mPlayerNum(0)
	, bShuffleDeck(true)
	, bEnableWeightedCards(true)
	, tLocation()
	, tChosenCardSet(ECardSet::BasicSet)
	, tHandIndex(0)
	, bCardIsClone(false)
	, mCardSetRef(ECardSet::BasicSet)
	, mNumberOfCardsToAdd(0)
	, mCardPickupDelay(0.2)
	, bSkipManaCheck(false)
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = true;
	bEnableTouchOverEvents = true;
	DefaultClickTraceChannel = ECC_WorldDynamic;
	bOnlyRelevantToOwner = false;
	bAlwaysRelevant = true;
	bReplicates = true;
}

void ACCGPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass,mHitCard);
	DOREPLIFETIME(ThisClass,mTargetDragSelectionActor);
	DOREPLIFETIME(ThisClass,bIsDragging);
	DOREPLIFETIME(ThisClass,bIsCardSelected);

	DOREPLIFETIME(ThisClass,mCardSet);
	DOREPLIFETIME(ThisClass,mBoardPlayer);
	DOREPLIFETIME(ThisClass,bTurnActive);

	DOREPLIFETIME(ThisClass,bPlayCardSuccess);

	DOREPLIFETIME(ThisClass,mTurnState);
	DOREPLIFETIME(ThisClass,mPlayerNum);

	DOREPLIFETIME(ThisClass,mPlayerDeck);

	DOREPLIFETIME(ThisClass,tLocation);

	DOREPLIFETIME(ThisClass,mCardsInHand);
	DOREPLIFETIME(ThisClass,mCardToAdd);
	DOREPLIFETIME(ThisClass,mCardSetRef);

	DOREPLIFETIME(ThisClass,bSkipManaCheck);
}

void ACCGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	OnInputTouchBegin.AddDynamic(this,&ThisClass::TouchBegin);
	OnInputTouchEnd.AddDynamic(this,&ThisClass::TouchEnd);
	OnClicked.AddDynamic(this,&ThisClass::ClickBegin);
	OnReleased.AddDynamic(this,&ThisClass::ClickEnd);

	const FInputModeGameAndUI inputMode;
	SetInputMode(inputMode);
}

void ACCGPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!mGameState)
	{
		const UWorld* world=GetWorld();
		if (world)
		{
			mGameState=Cast<ACCGState>(world->GetGameState());
		}
	}
	if (!mPlayerState)
	{
		mPlayerState=GetPlayerState<ACCGPlayerState>();
	}

	switch (mCardPlayerState)
	{
	case ECardPlayerState::PendingAction:
		break;
	case ECardPlayerState::CardInteraction:
		if (bTurnActive)
		{
			DetectInteractionOnMove();
			bCardPlayerStateDirty=true;
		}
		else if (bCardPlayerStateDirty)
		{
			bCardPlayerStateDirty=false;
			ClearCardInteractionState();
			if (mPlayCard_Client)
			{
				Client_DestroyCard();
			}
		}
		break;
	case ECardPlayerState::PlacingCard:
		if (bTurnActive)
		{
			const FTransform transform=UMiscBFL::MouseDistanceInWorldSpace(this,mCardHoldDistance);
			FRotator rotator;
			UMiscBFL::GetWorldRotationForPlayer(GetWorld(),FRotator::ZeroRotator,rotator);
			SetCardLocation(mPlayCard_Client,transform.GetLocation(),rotator);
			FHitResult hit;
			UMiscBFL::MouseToWorldLocation(this,hit);
			ValidateCardPlacement(hit.GetActor());
			bCardPlayerStateDirty=true;
		}
		else if (bCardPlayerStateDirty)
		{
			bCardPlayerStateDirty=false;
			ClearCardInteractionState();
			//ClearCardPlacementState
			if (mPlayCard_Client)
			{
				Client_DestroyCard();
			}
		}
		break;
	default: ;
	}
}

bool ACCGPlayerController::RemoveCardFromHand_Implementation(FName Card, int32 Index, bool RemoveAll)
{
	if (!HasAuthority())
	{
		return false;
	}
	if (RemoveAll)
	{
		mCardsInHand.Empty();
		Client_RemoveCardFromCardManager(nullptr,0,true);
		IF_RET_BOOL(mPlayerState);
		mPlayerState->Multicast_ForceUpdateUI();
	}
	else
	{
		if (!UCardsInHandBFL::HasCardInHand(mCardsInHand,Card,Index))
		{
			return false;
		}
		mCardsInHand.RemoveAt(Index);
		Client_RemoveCardFromCardManager(nullptr,Index,false);
	}
	Server_UpdatePlayerState();
	return true;
}

void ACCGPlayerController::DrawCard_Implementation(FName CardName, bool IgnoreMaxCards, int32 NumberOfCardsToDraw)
{
	if(!HasAuthority())
	{
		return;
	}
	mCardToAdd=CardName;
	const int32 cardCount=UDeckBFL::CountCardsInDeck(mPlayerDeck);
	if (cardCount-mNumberOfCardsToAdd<=0)
	{
		return;	
	}
	if (IgnoreMaxCards)
	{
		mNumberOfCardsToAdd+=NumberOfCardsToDraw;
	}
	else 
	{
		if (cardCount>=mMaxCardsInHand)
		{
			return;
		}
		if (NumberOfCardsToDraw+mNumberOfCardsToAdd+cardCount>mMaxCardsInHand)
		{
			mNumberOfCardsToAdd+=mMaxCardsInHand-(cardCount+mNumberOfCardsToAdd);
		}
		else
		{
			mNumberOfCardsToAdd+=NumberOfCardsToDraw;
		}
	}
	if (!mAddCardTH.IsValid())
	{
		GetWorldTimerManager().SetTimer(mAddCardTH,this,&ACCGPlayerController::AddCardToHand,mCardPickupDelay,true);
	}
}

ACard3D* ACCGPlayerController::CreatePlayableCard_Implementation(FTransform SpawnTransform)
{
	if (!HasAuthority())
	{
		return nullptr;
	}
	UWorld* world=GetWorld();
	IF_RET_NULL(world);
	IF_RET_NULL(mCard3DClass);
	FActorSpawnParameters spawnParameters;
	spawnParameters.Owner=this;
	spawnParameters.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	spawnParameters.TransformScaleMethod=ESpawnActorScaleMethod::OverrideRootScale;
	mPlayCard_Server=world->SpawnActor<ACard3D>(mCard3DClass,SpawnTransform,spawnParameters);
	return mPlayCard_Server;
}

bool ACCGPlayerController::AddCardToPlayersHand_Implementation(FName CardName)
{
	mCardsInHand.Add(CardName);
	Client_AddCardToCardManager(UCardsInHandBFL::GetCardInHand(mCardsInHand,0,true),ECardSet::Empty);
	Server_UpdatePlayerState();
	return true;
}

int32 ACCGPlayerController::CurPlayerNum_Implementation()
{
	return mPlayerNum;
}

void ACCGPlayerController::GetPlayerDeck_Implementation(TArray<FName>& Deck)
{
	Deck=mPlayerDeck;
}

bool ACCGPlayerController::RemoveCardFromDeck_Implementation(bool RemoveAll, int32 IndexToRemove)
{
	UDeckBFL::RemoveCardFromDeck(IndexToRemove,mPlayerDeck,RemoveAll);
	Server_UpdatePlayerState();
	return true;
}

void ACCGPlayerController::MatchEnd_Implementation(EEndGameResults Result)
{
	if (bTurnActive)
	{
		bTurnActive=false;
	}
	FTimerDelegate tempTimerDel;
	FTimerHandle tempTimer;
	tempTimerDel.BindUFunction(this, FName("Client_EndMatchState"), Result);
	GetWorld()->GetTimerManager().SetTimer(tempTimer, tempTimerDel, 0.5f, false);
}

void ACCGPlayerController::MatchBegin_Implementation()
{
	if (HasAuthority())
	{
		Execute_DrawCard(this,FName(),false,mCardsInFirstHand);
		Server_UpdatePlayerHealth();
		Client_CreateDisplayMessage(CCG_MSG::GameStart,FLinearColor::Yellow,false,0.f,true);
	}
}

void ACCGPlayerController::ChangeActivePlayerTurn_Implementation(bool TurnActive)
{
	if (HasAuthority())
	{
		if (TurnActive)
		{
			mTurnState=EGameTurn::TurnActive;
			bTurnActive=true;
			IF_RET_VOID(mPlayerState)
			if (mPlayerState->Implements<UPlayerStateInterface>())
			{
				IPlayerStateInterface::Execute_GetManaForTurn(this);
			}
			Execute_DrawCard(this,FName(),false,mCardsToDrawPerTurn);
			++mPlayerState->mPlayerStat.PlayerTurn;
			Client_CreateDisplayMessage(CCG_MSG::PlayerTurn,FLinearColor::Yellow,true,1.f,true);
		}
		else
		{
			mTurnState=EGameTurn::TurnInactive;
			bTurnActive=false;
			Client_CreateDisplayMessage(CCG_MSG::OpponentTurn,FLinearColor::Red,false,1.f,true);
		}
		Client_UpdateGameUI(false);
	}
}

void ACCGPlayerController::OnRep_bTurnActive()
{
	Client_UpdateGameUI(false);
}

void ACCGPlayerController::OnRep_TurnState()
{
	Client_UpdateGameUI(false);
}

void ACCGPlayerController::TouchBegin(ETouchIndex::Type FingerIndex, AActor* TouchedActor)
{
	CardInteraction();
}

void ACCGPlayerController::TouchEnd(ETouchIndex::Type FingerIndex, AActor* TouchedActor)
{
	DragCanceled();
}

void ACCGPlayerController::ClickBegin(AActor* TouchedActor, FKey ButtonPressed)
{
	CardInteraction();
}

void ACCGPlayerController::ClickEnd(AActor* TouchedActor, FKey ButtonPressed)
{
	DragCanceled();
}

bool ACCGPlayerController::IsPlatformMobile() const
{
	const UCCGameInstance* gameInstance = Cast<UCCGameInstance>(GetGameInstance());
	IF_RET_BOOL(gameInstance);
	return gameInstance->IsMobilePlatform();
}

void ACCGPlayerController::DisableCardSelection(ACard3D* ReceivingCard) const
{
	if (ReceivingCard)
	{
		ReceivingCard->Deselected();
	}
}

void ACCGPlayerController::ClearCardInteractionState()
{
	if (IsPlatformMobile()
		&&mEnabledMobilePreview)
	{
		mEnabledMobilePreview->DisableMobileCardPreview();
	}
	if (mTargetDragSelectionActor)
	{
		mTargetDragSelectionActor->Destroy();
	}
	DisableCardSelection(mReceivingCard);
	DisableCardSelection(mTalkingCard);
	mTargetDragSelectionActor=nullptr;
	mReceivingCard=nullptr;
	mTalkingCard=nullptr;
	mHitCard=nullptr;
	mReceivingPlayer=nullptr;
	bIsDragging=false;
	bIsCardSelected=false;
}

bool ACCGPlayerController::ValidatePlacement(ACardPlacement* PlacementTarget, FCard Card) const
{
	IF_RET_BOOL(PlacementTarget);
	return PlacementTarget->mValidCardTypes.Contains(Card.Type)&&!PlacementTarget->IsFull();
}

bool ACCGPlayerController::ValidatePlacementOwner(int32 PlayerIndex, int32 PlacementPlayerIndex, FCard Card) const
{
	if (Card.PlacementSetting.Owner==ECardPlayers::SelfAndOpponent)
	{
		return true;
	}
	if (Card.PlacementSetting.Owner==ECardPlayers::Self
		&&PlayerIndex==PlacementPlayerIndex)
	{
		return true;
	}
	return Card.PlacementSetting.Owner==ECardPlayers::Opponent
		&&PlayerIndex!=PlacementPlayerIndex;
}

void ACCGPlayerController::CardInteraction()
{
	if (bTurnActive)
	{
		IF_RET_VOID(mPlayerGameUI);
		if (mPlayerGameUI->Implements<UPlayerUIInterface>()
			&&IPlayerUIInterface::Execute_GetViewState(mPlayerGameUI)!=EViewState::Default
			&&(mCardPlayerState==ECardPlayerState::PendingAction||mCardPlayerState==ECardPlayerState::CardInteraction))
		{
			DetectCardInteraction();
		}
	}
	else
	{
		//Spawn Particle when interacting with the board when not the players turn
		if (mWarningParticle)
		{
			FHitResult hitResult;
			UMiscBFL::MouseToWorldLocation(this,hitResult);
			UGameplayStatics::SpawnEmitterAtLocation(this,mWarningParticle,hitResult.Location);
		}
	}
}

void ACCGPlayerController::SetCardLocation(ACard3D* Card, FVector HoldLocation, FRotator Rotation) const
{
	IF_RET_VOID(Card);
	const FVector cardLoc=Card->GetActorLocation();
	const UWorld* world = GetWorld();
	IF_RET_VOID(world);
	const float deltaTime=world->GetDeltaSeconds();
	const FVector interpVector=UKismetMathLibrary::VInterpTo(cardLoc,HoldLocation,deltaTime,10.f);
	Card->SetActorLocation(interpVector);
	if (bEnableInHandMovementRotation)
	{
		if (Rotation.Yaw>0.f)
		{
			Rotation.Pitch=(cardLoc.X-HoldLocation.X)/5.f;
			Rotation.Roll=(cardLoc.Y-HoldLocation.Y)/-5.f;
		}
		else
		{
			Rotation.Pitch=(cardLoc.X-HoldLocation.X)/-5.f;
			Rotation.Roll=(cardLoc.Y-HoldLocation.Y)/5.f;
		}
		const FRotator interpRot=UKismetMathLibrary::RInterpTo(Card->GetActorRotation(),Rotation,deltaTime,5.f);
		Card->SetActorRotation(interpRot);
	}
}

bool ACCGPlayerController::ValidateCardPlacement(AActor* HitActor)
{
	mCardPlacement=nullptr;
	mTargetCardPlacement=nullptr;
	
	IF_RET_BOOL(HitActor);
	ACardPlacement* cardPlacement=Cast<ACardPlacement>(HitActor);
	if (cardPlacement)
	{
		if (cardPlacement==mTargetCardPlacement)
		{
			return false;
		}
		mCardPlacement=cardPlacement;
	}
	else if (ACard3D* card3D=Cast<ACard3D>(HitActor))
	{
		if (card3D->GetPlacementOwner()==mTargetCardPlacement)
		{
			return false;
		}
		mCardOnBoard=card3D;
		mCardPlacement=cardPlacement;
	}
	else
	{
		return false;
	}
	IF_RET_BOOL(mPlayerState);
	const int32 playerID=mPlayerState->GetCardGamePlayerId();
	const FCard card=UDeckBFL::GetCardData(tCreateCardName,tChosenCardSet);
	if (!ValidatePlacement(mCardPlacement,card)
		||!ValidatePlacementOwner(playerID,mCardPlacement->GetPlayerIndex(),card))
	{
		return false;
	}
	mTargetCardPlacement=cardPlacement;
	return true;
}

ACard3D* ACCGPlayerController::Client_CreatePlaceableCard(FName Name, ECardSet CardSet, FVector SpawnTransformLocation)
{
	UWorld* world=GetWorld();
	IF_RET_NULL(world);
	IF_RET_NULL(mCard3DClass);
	FRotator cardRot;
	UMiscBFL::GetWorldRotationForPlayer(world,FRotator::ZeroRotator,cardRot);
	FActorSpawnParameters spawnParameters;
	spawnParameters.Owner=this;
	spawnParameters.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	spawnParameters.TransformScaleMethod=ESpawnActorScaleMethod::OverrideRootScale;
	mPlayCard_Client=world->SpawnActor<ACard3D>(mCard3DClass,SpawnTransformLocation,cardRot,spawnParameters);
	if (mPlayCard_Client)
	{
		mPlayCard_Client->SetClientSideData(Name,CardSet);
	}
	return mPlayCard_Client;
}

ACard3D* ACCGPlayerController::Server_CreatePlaceableCard(FTransform SpawnTransform)
{
	if (!HasAuthority())
	{
		return nullptr;
	}
	UWorld* world=GetWorld();
	IF_RET_NULL(world);
	IF_RET_NULL(mCard3DClass);
	FActorSpawnParameters spawnParameters;
	spawnParameters.Owner=this;
	spawnParameters.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	spawnParameters.TransformScaleMethod=ESpawnActorScaleMethod::OverrideRootScale;
	mPlayCard_Server=world->SpawnActor<ACard3D>(mCard3DClass,SpawnTransform,spawnParameters);
	return mPlayCard_Server;
}

ACardPlacement* ACCGPlayerController::ServerValidateCardPlacement(ACardPlacement* CardPlacement, FCard CardStruct) const
{
	if (!HasAuthority())
	{
		return nullptr;
	}
	IF_RET_NULL(CardPlacement);
	IF_RET_NULL(mPlayerState);
	const int32 playerID=mPlayerState->GetCardGamePlayerId();
	if (!ValidatePlacementOwner(playerID,CardPlacement->GetPlayerIndex(),CardStruct))
	{
		return nullptr;
	}
	if (!ValidatePlacement(CardPlacement,CardStruct))
	{
		return nullptr;
	}
	return CardPlacement;
}

ACard3D* ACCGPlayerController::SetCustomCardData(ACard3D* Card, bool ActiveAbility) const
{
	IF_RET_NULL(Card);
	Card->SetIsAbilityActive(ActiveAbility);
	return Card;
}

void ACCGPlayerController::PlayCard(FName CardName, ECardSet CardSet, ACardPlacement* CardPlacement, int32 CardHandIndex, FTransform SpawnTransform)
{
	if (!HasAuthority())
	{
		return;
	}
	IF_RET_VOID(mPlayerState);
	const FCard card=UDeckBFL::GetCardData(CardName,CardSet);
	bool cardPlayed=false;
	if (UCardsInHandBFL::HasCardInHand(mCardsInHand,CardName,CardHandIndex))
	{
		if (ServerValidateCardPlacement(CardPlacement,card))
		{
			ECardPlayConditions condition;
			if (UCardBFL::ValidateCardPlayConditions(CardName,this,condition))
			{
				int32 tempInt=0;
				if (UCardBFL::ChargeCardManaCost(this,mPlayerState->GetCardGamePlayerId(),bSkipManaCheck,card,EManaAccount::Placement,tempInt))
				{
					if (Execute_RemoveCardFromHand(this,CardName,CardHandIndex,false))
					{
						ACard3D* card3d=Server_CreatePlaceableCard(SpawnTransform);
						card3d=UCardBFL::SetupCard(card3d,mPlayerState->GetCardGamePlayerId(),CardName,CardSet,FCard(),false);
						UCardBFL::AddCardToBoardPlacement(card3d,CardPlacement,mPlayerState->GetCardGamePlayerId());
						cardPlayed=true;
					}
				}
				else
				{
					Client_LogNotificationMessage(CCG_MSG::NotEnoughMana,FLinearColor::Red);
				}
			}
			else
			{
				Client_LogNotificationMessage(UEnum::GetDisplayValueAsText(condition).ToString(),FLinearColor::Red);
			}
		}
		else
		{
			Client_LogNotificationMessage(CCG_MSG::InvalidPlacement,FLinearColor::Red);
		}
	}
	Client_DestroyCard();
	Client_DropCard(cardPlayed);
	Server_UpdatePlayerState();
}

void ACCGPlayerController::DetectCardInteraction()
{
	IF_RET_VOID(mPlayerState);
	FHitResult result;
	UMiscBFL::MouseToWorldLocation(this,result);
	const ACard3D* card3d=Cast<ACard3D>(result.GetActor());
	if (!card3d)
	{
		ClearCardInteractionState();
		mCardPlayerState=ECardPlayerState::PendingAction;
		return;
	}
	bool checkTalkingCard=true;
	bool checkMobile=true;
	if (card3d->GetOwningPlayerID()==mPlayerState->GetCardGamePlayerId())
	{
		if (bIsCardSelected)
		{
			if (!mTalkingCard)
			{
				checkMobile=false;
				checkTalkingCard=false;
				mTalkingCard=mHitCard;
				mTalkingCard->Selected(mPlayerState->GetCardGamePlayerId());
			}
		}
		else
		{
			checkMobile=false;
			checkTalkingCard=false;
			bIsCardSelected=true;
			mCardPlayerState=ECardPlayerState::CardInteraction;
			mTalkingCard=mHitCard;
			mTalkingCard->Selected(mPlayerState->GetCardGamePlayerId());
		}
	}
	if (checkTalkingCard&&mTalkingCard)
	{
		if (mTalkingCard!=mHitCard)
		{
			checkMobile=false;
			IF_RET_VOID(mHitCard);
			if (mTalkingCard->IsPreviewEnabled())
			{
				mHitCard->DisableMobileCardPreview();
				ClearCardInteractionState();
				mCardPlayerState=ECardPlayerState::PendingAction;
			}
			else if (mTalkingCard->CanAttackCard())
			{
				mReceivingCard=mHitCard;
				mReceivingCard->Selected(mPlayerState->GetCardGamePlayerId());
			}
			else
			{
				ClearCardInteractionState();
				mCardPlayerState=ECardPlayerState::PendingAction;
			}
		}
	}
	if (checkMobile&&IsPlatformMobile())
	{
		IF_RET_VOID(mHitCard);
		if (mHitCard->IsPreviewEnabled())
		{
			mHitCard->DisableMobileCardPreview();
			ClearCardInteractionState();
			mCardPlayerState=ECardPlayerState::PendingAction;
		}
		else
		{
			mHitCard->EnableMobileCardPreview();
			mEnabledMobilePreview=mHitCard;
		}
	}
	else
	{
		ClearCardInteractionState();
		mCardPlayerState=ECardPlayerState::PendingAction;
	}
}

bool ACCGPlayerController::ValidateInteractionState() const
{
	if (!bIsCardSelected)
	{
		return false;
	}
	const bool validateInteraction=mReceivingPlayer||(mTalkingCard&&mReceivingCard&&mTalkingCard!=mReceivingCard);
	if (!validateInteraction)
	{
		return false;
	}
	return true;
}

void ACCGPlayerController::DetectInteractionOnMove()
{
	//EnableDragSelection
	if (!bIsDragging)
	{
		ClearCardInteractionState();
		DetectCardInteraction();
		bIsDragging=true;
	}

	FHitResult result;
	UMiscBFL::MouseToWorldLocation(this,result);
	IF_RET_VOID(mReceivingCard);
	if (Cast<ACard3D>(result.GetActor()))
	{
		IF_RET_VOID(mTalkingCard);
		IF_RET_VOID(mPlayerState);
		if (mTalkingCard->CanAttackCard()
			&&mHitCard!=mReceivingCard)
		{
			mReceivingCard->Deselected();
			mReceivingCard=nullptr;
			if (mHitCard!=mTalkingCard)
			{
				mReceivingCard=mHitCard;
				mReceivingPlayer=nullptr;
				mReceivingCard->Selected(mPlayerState->GetCardGamePlayerId());
			}
		}
	}
	else if (ABoardPlayer* boardPlayer=Cast<ABoardPlayer>(result.GetActor()))
	{
		IF_RET_VOID(mTalkingCard);
		if (mTalkingCard->CanAttackPlayer())
		{
			mReceivingPlayer=boardPlayer;
		}
		mReceivingCard->Deselected();
		mReceivingCard=nullptr;
	}
	else
	{
		mReceivingCard->Deselected();
		mReceivingCard=nullptr;
		mReceivingPlayer=nullptr;
	}
	if (CreateDragActorVisual(true))
	{
		IF_RET_VOID(mTargetDragSelectionActor);
		mTargetDragSelectionActor->SetSelectionProperties(result.Location,mReceivingCard||mReceivingPlayer);
	}
}

void ACCGPlayerController::RunCardInteraction(ACard3D* InteractionTalkingCard, ACard3D* InteractionReceivingCard, ABoardPlayer* InteractionReceivingPlayer)
{
	if (!HasAuthority())
	{
		return;
	}
	IF_RET_VOID(InteractionTalkingCard);
	IF_RET_VOID(mPlayerState);
	int32 cost=0;
	if (!UCardBFL::ChargeCardManaCost(this,mPlayerState->GetCardGamePlayerId(),bSkipManaCheck,*InteractionTalkingCard->GetCardData(),EManaAccount::Attack,cost))
	{
		Client_LogNotificationMessage(CCG_MSG::NotEnoughMana,FLinearColor::Red);
		return;
	}
	
	if (InteractionReceivingPlayer)
	{
		EInteractionConditions condition;
		if (!UCardInteractionBFL::DealDamageToPlayer(false,InteractionTalkingCard,InteractionReceivingPlayer,0,condition))
		{
			Client_LogNotificationMessage(UEnum::GetDisplayValueAsText(condition).ToString(),FLinearColor::Yellow);
			return;
		}
	}
	else
	{
		EInteractionConditions callingCondition;
		EInteractionConditions talkingCondition;
		if (!UCardInteractionBFL::DealDamageToCard(false,InteractionTalkingCard,InteractionReceivingCard,0,callingCondition,talkingCondition))
		{
			if (callingCondition!=EInteractionConditions::None)
			{
				Client_LogNotificationMessage(UEnum::GetDisplayValueAsText(callingCondition).ToString(),FLinearColor::Yellow);
			}
			if (talkingCondition!=EInteractionConditions::None)
			{
				Client_LogNotificationMessage(UEnum::GetDisplayValueAsText(talkingCondition).ToString(),FLinearColor::Yellow);
			}
		}
	}
}

bool ACCGPlayerController::CreateDragActorVisual(bool UseActorLocation)
{
	if (mTargetDragSelectionActor
		||!mTalkingCard)
	{
		return mTalkingCard!=nullptr;
	}
	IF_RET_BOOL(mDragSelectionClass);
	UWorld* world=GetWorld();
	IF_RET_BOOL(world);
	FVector spawnLoc;
	if (UseActorLocation)
	{
		spawnLoc=mTalkingCard->GetActorLocation();
	}
	else
	{
		FHitResult hitResult;
		UMiscBFL::MouseToWorldLocation(this,hitResult);
		spawnLoc=hitResult.Location;
	}
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	mTargetDragSelectionActor=world->SpawnActor<ATargetDragSelection>(mDragSelectionClass,spawnLoc,FRotator::ZeroRotator,spawnParams);
	return mTalkingCard!=nullptr;
}

void ACCGPlayerController::SetupGameUI()
{
	if (!IsLocalPlayerController())
	{
		return;
	}
	IF_RET_VOID(mPlayerGameUIClass);
	mPlayerGameUI=CreateWidget(this,mPlayerGameUIClass);
	mPlayerGameUI->AddToViewport(CCG_ZOrder::PlayerGameUI);
	
	IF_RET_VOID(mOpponentUIClass);
	mOpponentUI=CreateWidget(this,mOpponentUIClass);
	mOpponentUI->AddToViewport(CCG_ZOrder::OpponentUI);
}

void ACCGPlayerController::SetupDeck(FString DeckName, TArray<FName>& PlayerDeck)
{
	if (!HasAuthority())
	{
		return;
	}
	if (bShuffleDeck)
	{
		ShufflePlayerDeck(PlayerDeck);
	}
	if (bEnableWeightedCards)
	{
		UDeckBFL::FilterWeightedCardsInDeck(PlayerDeck);
	}
	IF_RET_VOID(mPlayerState);
	mPlayerState->SetMaxCardsInDeck(PlayerDeck.Num());
	mPlayerDeck=PlayerDeck;
}

void ACCGPlayerController::SetTimer(int32 Time)
{
	if (!mCountdownTimerWidget)
	{
		mCountdownTimerWidget=CreateWidget(this,mCountdownWidgetClass);
		mCountdownTimerWidget->AddToViewport(CCG_ZOrder::Countdown);
	}
	if (mCountdownTimerWidget->Implements<UTimerInterface>())
	{
		ITimerInterface::Execute_SetTime(mCountdownTimerWidget,Time);
	}
	if (Time<1)
	{
		mCountdownTimerWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		mCountdownTimerWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

FString ACCGPlayerController::LoadClientDeck(TArray<FName>& Deck) const
{
	const UCCGameInstance* gameInstance= Cast<UCCGameInstance>(GetGameInstance());
	IF_RET(FString,gameInstance);
	FString deckName=gameInstance->GetSelectedCardSet();
	bool tempBool=false;
	if (USaveGameBFL::LoadCustomDeck(deckName,Deck,tempBool))
	{
		return deckName;
	}
	TArray<FName> deckNames;
	UCardBFL::GetAllPreBuildDeckNames(deckNames);
	deckName=deckNames[FMath::Rand()%deckNames.Num()].ToString();
	USaveGameBFL::LoadCustomDeck(deckName,Deck,tempBool);
	return deckName;
}

void ACCGPlayerController::ShufflePlayerDeck(TArray<FName>& TargetArray) const
{
	UCCGBFL::ShuffleArray(TargetArray);
}

void ACCGPlayerController::NotifyCardTurnActive() const
{
	IF_RET_VOID(mGameState);
	IF_RET_VOID(mPlayerState);
	mGameState->BeginPlayerTurn(mPlayerState->GetCardGamePlayerId());
}

void ACCGPlayerController::NotifyCardsEndTurn() const
{
	IF_RET_VOID(mGameState);
	IF_RET_VOID(mPlayerState);
	mGameState->EndOfPlayerTurn(mPlayerState->GetCardGamePlayerId());
}

void ACCGPlayerController::CallCreateCard(FName CardName, ECardSet CardSet, int32 CardHandIndex, UUserWidget* CardWidget)
{
	IF_RET_VOID(CardWidget);
	if (!bTurnActive)
	{
		return;
	}
	const FInputModeGameOnly inputMode;
	SetInputMode(inputMode);
	mCardWidget=CardWidget;
	tCreateCardName=CardName;
	tChosenCardSet=CardSet;
	tHandIndex=CardHandIndex;
	mCardPlayerState=ECardPlayerState::PlacingCard;

	FVector spawnLoc;
	const UCCGameInstance* gameInstance=Cast<UCCGameInstance>(GetGameInstance());
	IF_RET_VOID(gameInstance);
	const bool isMobile= gameInstance->IsMobilePlatform();
	if (isMobile)
	{
		spawnLoc=UMiscBFL::MouseDistanceInWorldSpace(this,mCardHoldDistance).GetLocation();
	}
	else
	{
		IF_RET_VOID(mCardWidget->Implements<UCardWidgetInterface>());
		const FVector2D mousePos=ICardWidgetInterface::Execute_GetInitialMousePos(mCardWidget);
		spawnLoc=UMiscBFL::ScreenPositionInWorldSpace(this,mousePos,1500.f).GetLocation();
	}
	Client_CreatePlaceableCard(CardName,CardSet,spawnLoc);
}

void ACCGPlayerController::DragCanceled()
{
	if (!bTurnActive)
	{
		return;
	}
	switch (mCardPlayerState)
	{
	case ECardPlayerState::PendingAction:
		break;
	case ECardPlayerState::CardInteraction:
		if (ValidateInteractionState())
		{
			Server_RunCardInteraction(mTalkingCard,mReceivingCard,mReceivingPlayer);
			ClearCardInteractionState();
			mCardPlayerState = ECardPlayerState::PendingAction;
		}
		else if (bIsDragging)
		{
			ClearCardInteractionState();
			mCardPlayerState = ECardPlayerState::PendingAction;
		}
		break;
	case ECardPlayerState::PlacingCard:
		IF_RET_VOID(mPlayCard_Client);
		if (bIsValidClientDrop)
		{
			Server_PlayCard(tCreateCardName,tChosenCardSet,mTargetCardPlacement,tHandIndex,mPlayCard_Client->GetActorTransform());
			mCardPlayerState = ECardPlayerState::PendingAction;
		}
		else
		{
			Client_DropCard(false);
			Client_DestroyCard();
		}
		break;
	default: ;
	}
}

void ACCGPlayerController::Server_SpawnAIOpponent_Implementation()
{
	const UWorld* world=GetWorld();
	IF_RET_VOID(world);
	ACCGMode* gameMode=Cast<ACCGMode>(world->GetAuthGameMode());
	IF_RET_VOID(gameMode);
	gameMode->ForceSpawnAIOpponent();
}

void ACCGPlayerController::Server_SetupDeck_Implementation()
{
	Client_GetPlayerDeck();
}

void ACCGPlayerController::Server_ReturnPlayerDeck_Implementation(const FString& DeckName,const TArray<FName>& DeckArray)
{
	TArray<FName> newDeck=DeckArray;
	SetupDeck(DeckName,newDeck);
	Server_UpdatePlayerState();
	Client_CreateDisplayMessage(CCG_MSG::DeckCreated,FLinearColor::Green,false,0.f,true);
}

void ACCGPlayerController::Server_RequestChangeTurnState_Implementation()
{
	IF_RET_VOID(mGameState);
	if (mGameState->RequestChangeTurnState(this))
	{
		mGameState->Server_RequestChangeTurnState(this);
	}
}

void ACCGPlayerController::Server_UpdatePlayerHealth_Implementation()
{
	IF_RET_VOID(mPlayerState);
	IF_RET_VOID(mBoardPlayer);
	mBoardPlayer->Server_UpdateHealth(mPlayerState->mPlayerStat.Health);
}

void ACCGPlayerController::Server_UpdatePlayerState_Implementation()
{
	IF_RET_VOID(mGameState);
	IF_RET_VOID(mPlayerState);
	TArray<ACard3D*> cards;
	mGameState->GetActivePlayerCards(mPlayerState->GetCardGamePlayerId(),cards);
	mPlayerState->UpdatePlayersCardStates(mCardsInHand.Num(),mPlayerDeck.Num(),cards.Num());
}

void ACCGPlayerController::Server_PlayCard_Implementation(FName CardName, ECardSet CardSet, ACardPlacement* CardPlacement, int32 CardHandIndex, FTransform SpawnTransform)
{
	PlayCard(CardName,CardSet,CardPlacement,CardHandIndex,SpawnTransform);
}

void ACCGPlayerController::Server_RunCardInteraction_Implementation(ACard3D* InteractionTalkingCard, ACard3D* InteractionReceivingCard, ABoardPlayer* InteractionReceivingPlayer)
{
	RunCardInteraction(InteractionTalkingCard,InteractionReceivingCard,InteractionReceivingPlayer);
}

void ACCGPlayerController::Server_SetSkipManaCheck_Implementation(bool SkipCheck)
{
	bSkipManaCheck=SkipCheck;
}

void ACCGPlayerController::Server_ReshuffleDeck_Implementation()
{
	Client_GetPlayerDeck();
}

void ACCGPlayerController::Server_ClearCardsInHand_Implementation()
{
	mCardsInHand.Empty();
	Server_UpdatePlayerState();
}

void ACCGPlayerController::Client_PostLogin_Implementation()
{
	const UCCGameInstance* gameInstance=Cast<UCCGameInstance>(GetGameInstance());
	IF_RET_VOID(gameInstance);
	if (gameInstance->IsSameGameState(ECardGameState::Playing))
	{
		SetupGameUI();
		Client_UpdateGameUI(false);
	}
}

void ACCGPlayerController::Client_SetCountdownTimer_Implementation(int32 Time)
{
	SetTimer(Time);
}

void ACCGPlayerController::Client_EndMatchState_Implementation(EEndGameResults Result)
{
	IF_RET_VOID(mPlayerGameUI);
	IF_RET_VOID(mOpponentUI);
	if (mPlayerGameUI->Implements<UPlayerUIInterface>())
	{
		IPlayerUIInterface::Execute_SetCardViewState(mPlayerGameUI,EViewState::HideHand,true);
	}
	if (mOpponentUI->Implements<UPlayerUIInterface>())
	{
		IPlayerUIInterface::Execute_SetCardViewState(mOpponentUI,EViewState::HideHand,true);
	}

	FLinearColor color=FLinearColor::White;
	switch (Result)
	{
	case EEndGameResults::Victory:
		color=FLinearColor::Green;
		break;
	case EEndGameResults::Defeat:
		color=FLinearColor::Red;
		break;
	case EEndGameResults::Draw:
		color=FLinearColor::Yellow;
		break;
	default: ;
	}
	FTimerDelegate tempTimerDel;
	FTimerHandle tempTimer;
	tempTimerDel.BindUFunction(this, FName("Client_CreateDisplayMessage"), UEnum::GetDisplayValueAsText(Result).ToString(),color,true,3.f,false);
	GetWorld()->GetTimerManager().SetTimer(tempTimer, tempTimerDel, 1.f, false);
}

void ACCGPlayerController::Client_UpdateGameUI_Implementation(bool ForceCleanUpdate)
{
	IF_RET_VOID(mPlayerGameUI);
	IF_RET_VOID(mOpponentUI);
	Server_UpdatePlayerHealth();
	if (mOpponentUI->Implements<UPlayerUIInterface>())
	{
		IPlayerUIInterface::Execute_UpdateUIPlayerStats(mOpponentUI,ForceCleanUpdate);
	}
	if (mPlayerGameUI->Implements<UPlayerUIInterface>())
	{
		IPlayerUIInterface::Execute_UpdateUIPlayerStats(mPlayerGameUI,false);
		IPlayerUIInterface::Execute_UpdateUITurnState(mPlayerGameUI,bTurnActive,mTurnState);
	}
}

void ACCGPlayerController::Client_DropCard_Implementation(bool CardPlayed)
{
	IF_RET_VOID(mPlayerGameUI);
	if (CardPlayed)
	{
		if (mPlayerGameUI->Implements<UPlayerUIInterface>())
		{
			IPlayerUIInterface::Execute_SetCardViewState(mPlayerGameUI,EViewState::Default,false);
		}
		Client_UpdateGameUI(false);
	}
	else
	{
		IF_RET_VOID(mCardWidget);
		if (mCardWidget->Implements<UCardWidgetInterface>())
		{
			ICardWidgetInterface::Execute_SetDefaults(mCardWidget);
		}
		mCardWidget=nullptr;
		if (mPlayerGameUI->Implements<UPlayerUIInterface>())
		{
			IPlayerUIInterface::Execute_SetCardViewState(mPlayerGameUI,EViewState::Default,false);
		}
	}
	const FInputModeGameAndUI inputMode;
	SetInputMode(inputMode);
}

void ACCGPlayerController::Client_CreateDisplayMessage_Implementation(const FString& Msg, FLinearColor Color, bool ToScreen, float ScreenDuration, bool ToMsgLogger)
{
	if (ToScreen)
	{
		UMiscBFL::CreateScreenDisplayMessage(this,Msg,Color,ScreenDuration);
	}
	if (ToMsgLogger)
	{
		UMiscBFL::AddMsgToLog(this,Msg,Color,ScreenDuration);
	}
}

void ACCGPlayerController::Client_LogNotificationMessage_Implementation(const FString& Msg, FLinearColor Color)
{
	IF_RET_VOID(mPlayerGameUI);
	if (mPlayerGameUI->Implements<UPlayerUIInterface>())
	{
		IPlayerUIInterface::Execute_LogNotificationMessage(mPlayerGameUI,Msg,Color);
	}
}

void ACCGPlayerController::Client_GetPlayerDeck_Implementation()
{
	TArray<FName> deck;
	const FString deckName=LoadClientDeck(deck);
	Server_ReturnPlayerDeck(deckName,deck);
}

void ACCGPlayerController::Client_DestroyCard_Implementation()
{
	IF_RET_VOID(mPlayCard_Client);
	mPlayCard_Client->Destroy();
	mPlayCard_Client=nullptr;
	mCardPlayerState = ECardPlayerState::PendingAction;
}

void ACCGPlayerController::Debug_Server_AddCardToHand_Implementation(FName CardName)
{
	mCardToAdd=CardName;
	
	if (mCardToAdd.IsNone())
	{
		Execute_AddCardToPlayersHand(this, UDeckBFL::GetCardFromDeck(mPlayerDeck,0,false));
		Execute_RemoveCardFromDeck(this,false,0);
	}
	else
	{
		Execute_AddCardToPlayersHand(this, mCardToAdd);
	}
	Server_UpdatePlayerState();
	if (mNumberOfCardsToAdd<=0)
	{
		GetWorldTimerManager().ClearTimer(mAddCardTH);
	}
}

void ACCGPlayerController::AddCardToHand()
{
	if (mNumberOfCardsToAdd>=0)
	{
		--mNumberOfCardsToAdd;
		if (mCardToAdd.IsNone())
		{
			Execute_AddCardToPlayersHand(this, UDeckBFL::GetCardFromDeck(mPlayerDeck,0,false));
			Execute_RemoveCardFromDeck(this,false,0);
		}
		else
		{
			Execute_AddCardToPlayersHand(this, mCardToAdd);
		}
		Server_UpdatePlayerState();
	}
	if (mNumberOfCardsToAdd<=0)
	{
		GetWorldTimerManager().ClearTimer(mAddCardTH);
	}
}

void ACCGPlayerController::Client_AddCardToCardManager_Implementation(FName CardName, ECardSet FromCardSet)
{
	IF_RET_VOID(mPlayerGameUI);
	if (mPlayerGameUI->Implements<UPlayerUIInterface>())
	{
		IPlayerUIInterface::Execute_AddCardWidget(mPlayerGameUI,CardName,FromCardSet);
	}
}

void ACCGPlayerController::Client_RemoveCardFromCardManager_Implementation(UUserWidget* CardWidget, int32 CardHandIndex, bool RemoveAll)
{
	IF_RET_VOID(mPlayerGameUI);
	if (mPlayerGameUI->Implements<UPlayerUIInterface>())
	{
		IPlayerUIInterface::Execute_RemoveCardWidget(mPlayerGameUI,CardWidget,CardHandIndex,RemoveAll);
	}
}