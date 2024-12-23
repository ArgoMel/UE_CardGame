// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController/CCGPlayerController.h"

#include "Net/UnrealNetwork.h"

ACCGPlayerController::ACCGPlayerController()
: bIsDragging(false)
, bIsCardSelected(false)
, bValidInteraction(false)
, mPlayerStateEnum(ECardPlayerState::PendingAction)
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
}

void ACCGPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ACCGPlayerController::RemoveCardFromHand_Implementation(FName Card, int32 Index, bool RemoveAll)
{
	return ICardsInHandInterface::RemoveCardFromHand_Implementation(Card, Index, RemoveAll);
}

void ACCGPlayerController::DrawCard_Implementation(FName CardName, bool IgnoreMaxCards, int32 NumberOfCardsToDraw)
{
	ICardsInHandInterface::DrawCard_Implementation(CardName, IgnoreMaxCards, NumberOfCardsToDraw);
}

ACard3D* ACCGPlayerController::CreatePlayableCard_Implementation(FTransform SpawnTransform)
{
	return IControllerInterface::CreatePlayableCard_Implementation(SpawnTransform);
}

bool ACCGPlayerController::AddCardToPlayersHand_Implementation(FName CardName)
{
	return IControllerInterface::AddCardToPlayersHand_Implementation(CardName);
}

void ACCGPlayerController::BeginPlayerTurn_Implementation()
{
	IControllerInterface::BeginPlayerTurn_Implementation();
}

void ACCGPlayerController::GetPlayerDeck_Implementation(TArray<FName>& Deck)
{
	IDeckInterface::GetPlayerDeck_Implementation(Deck);
}

bool ACCGPlayerController::RemoveCardFromDeck_Implementation(bool RemoveAll, int32 IndexToRemove)
{
	return IDeckInterface::RemoveCardFromDeck_Implementation(RemoveAll, IndexToRemove);
}

void ACCGPlayerController::MatchEnd_Implementation(EEndGameResults Result)
{
	IGameStateInterface::MatchEnd_Implementation(Result);
}

void ACCGPlayerController::MatchBegin_Implementation()
{
	IGameStateInterface::MatchBegin_Implementation();
}

void ACCGPlayerController::ChangeActivePlayerTurn_Implementation(bool TurnActive)
{
	IGameStateInterface::ChangeActivePlayerTurn_Implementation(TurnActive);
}

void ACCGPlayerController::OnRep_bTurnActive()
{
}

void ACCGPlayerController::OnRep_TurnState()
{
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

void ACCGPlayerController::CardInteraction()
{
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

void ACCGPlayerController::Client_CreateDisplayMessage_Implementation(const FString& Msg, FLinearColor Color, bool ToScreen,
	float ScreenDuration, bool ToMsgLogger)
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
}

void ACCGPlayerController::Debug_Server_AddCardToHand_Implementation()
{
}

void ACCGPlayerController::AddCardToHand()
{
}

void ACCGPlayerController::Client_AddCardToCardManager_Implementation(FName CardName, ECardSet FromCardSet)
{
}

void ACCGPlayerController::Client_RemoveCardFromCardManager_Implementation(UUserWidget* CardWidget, int32 CardHandIndex, bool RemoveAll)
{
}