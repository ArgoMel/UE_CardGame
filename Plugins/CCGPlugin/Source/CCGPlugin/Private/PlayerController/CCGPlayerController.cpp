// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController/CCGPlayerController.h"

#include "CCGPlugin.h"
#include "BFL/CardsInHandBFL.h"
#include "BFL/DeckBFL.h"
#include "BFL/MiscBFL.h"
#include "GameInstance/CCGameInstance.h"
#include "Gameplay/Card3D.h"
#include "Gameplay/TargetDragSelection.h"
#include "GameState/CCGState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/CCGPlayerState.h"

ACCGPlayerController::ACCGPlayerController()
	: bCardPlayerStateDirty(false)
	, bIsDragging(false)
	, bIsCardSelected(false)
	, bValidInteraction(false)
	, mCardPlayerState(ECardPlayerState::PendingAction)
	, mCardSet(ECardSet::BasicSet)
	, mMaxCardsInHand(7)
	, mCardsInFirstHand(5)
	, mCardsToDrawPerTurn(1)
	, bTurnActive(false)
	, mCardHoldDistance(1700.)
	, bEnableInHandMovementRotation(true)
	, bIsValidClientDrop(false)
	, bPlayCardSuccess(false)
	, bIsPlayed(false)
	, bEnabledMobileCardPreview(false)
	, mError()
	, mTurnState()
	, mWeightedFilterIndex(0)
	, bShuffleDeck(true)
	, bEnableWeightedCards(true)
	, tLocation()
	, tChosenCardSet(ECardSet::BasicSet)
	, tHandIndex(0)
	, bCardIsClone(false)
	, mCardSetRef(ECardSet::BasicSet)
	, mNumberOfCardsToAdd(0)
	, mCardInHandIndex(0)
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

	FInputModeGameAndUI inputMode;
	inputMode.SetHideCursorDuringCapture(false);
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
			ClearCardPlacementState();
		}
		break;
	case ECardPlayerState::PlacingCard:
		if (bTurnActive)
		{
			FTransform transform;
			UMiscBFL::MouseDistanceInWorldSpace(this,mCardHoldDistance,transform);
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
			ClearCardPlacementState();
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
			mError=ECardError::CardNotFoundInHand;
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
	FActorSpawnParameters spawnParameters;
	spawnParameters.Owner=this;
	spawnParameters.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	spawnParameters.TransformScaleMethod=ESpawnActorScaleMethod::OverrideRootScale;
	mPlayCard_Server=world->SpawnActor<ACard3D>(ACard3D::StaticClass(),SpawnTransform,spawnParameters);
	return mPlayCard_Server;
}

bool ACCGPlayerController::AddCardToPlayersHand_Implementation(FName CardName)
{
	mCardsInHand.Add(CardName);
	Client_AddCardToCardManager(UCardsInHandBFL::GetCardInHand(mCardsInHand,0,true),ECardSet::Empty);
	Server_UpdatePlayerState();
	return true;
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
		Client_CreateDisplayMessage(CCG_MSG::GameStart,FLinearColor::Yellow,false,0.f);
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
			Client_CreateDisplayMessage(CCG_MSG::PlayerTurn,FLinearColor::Yellow,true,1.f);
		}
		else
		{
			mTurnState=EGameTurn::TurnInactive;
			bTurnActive=false;
			Client_CreateDisplayMessage(CCG_MSG::OpponentTurn,FLinearColor::Red,false,1.f);
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
	bool isMobile=false;
	gameInstance->GetCurrentPlatform(isMobile);
	return isMobile;
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

void ACCGPlayerController::ClearCardPlacementState()
{
	if (mPlayCard_Client)
	{
		Client_DestroyCard();
	}
}

bool ACCGPlayerController::CheckPlayerState(EViewState ViewState, ECardPlayerState ValidState1, ECardPlayerState ValidState2)
{
	return false;
}

void ACCGPlayerController::CardInteraction()
{
	if (bTurnActive)
	{
		if (CheckPlayerState(EViewState::Default,ECardPlayerState::PendingAction,ECardPlayerState::CardInteraction))
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

void ACCGPlayerController::SetCardLocation(ACard3D* Card, FVector HoldLocation, FRotator Rotation)
{
	
}

bool ACCGPlayerController::ValidateCardPlacement(AActor* HitActor)
{
	return false;
}

ACard3D* ACCGPlayerController::Client_CreatePlaceableCard(FName Name, ECardSet CardSet, FVector SpawnTransformLocation)
{
	return nullptr;
}

ACard3D* ACCGPlayerController::Server_CreatePlaceableCard(FTransform SpawnTransform)
{
	return nullptr;
}

ACardPlacement* ACCGPlayerController::ServerValidateCardPlacement(ACardPlacement* CardPlacement, FCard CardStruct)
{
	return nullptr;
}

ACard3D* ACCGPlayerController::SetCustomCardData(ACard3D* Card, bool ActiveAbility)
{
	return nullptr;
}

void ACCGPlayerController::PlayCard(FName CardName, ECardSet CardSet, ACardPlacement* CardPlacement, int32 CardHandIndex, FTransform SpawnTransform)
{
}

void ACCGPlayerController::DetectCardInteraction()
{
}

bool ACCGPlayerController::ValidateInteractionState()
{
	return false;
}

void ACCGPlayerController::DetectInteractionOnMove()
{
	
}

void ACCGPlayerController::RunCardInteraction(ACard3D* InteractionTalkingCard, ACard3D* InteractionReceivingCard, ABoardPlayer* InteractionReceivingPlayer)
{
}

bool ACCGPlayerController::CreateDragActorVisual(bool UseActorLocation)
{
	return false;
}

void ACCGPlayerController::SetupGameUI()
{
}

void ACCGPlayerController::SetupDeck(FString DeckName, TArray<FName>& PlayerDeck)
{
}

void ACCGPlayerController::SetTimer(int32 Time)
{
}

FString ACCGPlayerController::LoadClientDeck(TArray<FName>& Deck)
{
	return FString();
}

void ACCGPlayerController::FilterWeightedCards()
{
}

void ACCGPlayerController::ShufflePlayerDeck(const TArray<FName>& TargetArray)
{
}

void ACCGPlayerController::NotifyCardTurnActive()
{
}

void ACCGPlayerController::NotifyCardsEndTurn()
{
}

void ACCGPlayerController::CallCreateCard(FName CardName, ECardSet CardSet, int32 CardHandIndex, UUserWidget* CardWidget)
{
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
			Client_DropCard(false,mError);
			Client_DestroyCard();
		}
		break;
	default: ;
	}
}

void ACCGPlayerController::Server_SpawnAIOpponent_Implementation()
{
}

void ACCGPlayerController::Server_SetupDeck_Implementation()
{
}

void ACCGPlayerController::Server_ReturnPlayerDeck_Implementation(const FString& DeckName,const TArray<FName>& DeckArray)
{
}

void ACCGPlayerController::Server_RequestChangeTurnState_Implementation()
{
}

void ACCGPlayerController::Server_UpdatePlayerHealth_Implementation()
{
}

void ACCGPlayerController::Server_UpdatePlayerState_Implementation()
{
}

void ACCGPlayerController::Server_PlayCard_Implementation(FName CardName, ECardSet CardSet, ACardPlacement* CardPlacement, int32 CardHandIndex, FTransform SpawnTransform)
{
}

void ACCGPlayerController::Server_RunCardInteraction_Implementation(ACard3D* InteractionTalkingCard, ACard3D* InteractionReceivingCard, ABoardPlayer* InteractionReceivingPlayer)
{
}

void ACCGPlayerController::Server_SetSkipManaCheck_Implementation(bool SkipCheck)
{
}

void ACCGPlayerController::Server_ReshuffleDeck_Implementation()
{
}

void ACCGPlayerController::Server_ClearCardsInHand_Implementation()
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

void ACCGPlayerController::Client_DropCard_Implementation(bool CardPlayed, ECardError Error)
{
}

void ACCGPlayerController::Client_CreateDisplayMessage_Implementation(const FString& Msg, FLinearColor Color, bool ToScreen, float ScreenDuration, bool ToMsgLogger)
{
}

void ACCGPlayerController::Client_LogNotificationMessage_Implementation(const FString& Msg, FLinearColor Color)
{
}

void ACCGPlayerController::Client_GetPlayerDeck_Implementation()
{
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
}

void ACCGPlayerController::Client_RemoveCardFromCardManager_Implementation(UUserWidget* CardWidget, int32 CardHandIndex, bool RemoveAll)
{
}