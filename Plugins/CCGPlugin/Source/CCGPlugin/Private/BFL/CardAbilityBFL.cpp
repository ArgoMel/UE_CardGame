// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/CardAbilityBFL.h"

#include "CCGPlugin.h"
#include "BFL/CardBFL.h"
#include "BFL/CardInteractionBFL.h"
#include "BFL/ControllerBFL.h"
#include "BFL/DeckBFL.h"
#include "BFL/PlacementBFL.h"
#include "Gameplay/Card3D.h"
#include "GameState/CCGState.h"
#include "Interface/CardsInHandInterface.h"
#include "Interface/ControllerInterface.h"
#include "Interface/DeckInterface.h"
#include "Interface/PlayerStateInterface.h"

void UCardAbilityBFL::DrawCard(ACard3D* CallingCard, int32 AbilityIndex)
{
	IF_RET_VOID(CallingCard);
	AController* controller=UControllerBFL::GetControllerReferenceFromID(CallingCard,CallingCard->GetOwningPlayerID());
	IF_RET_VOID(controller);
	if (controller->Implements<UCardsInHandInterface>())
	{
		const int32 cardsToDraw=CallingCard->mAbilities[AbilityIndex].AbilityInt;
		ICardsInHandInterface::Execute_DrawCard(controller,FName(),false,cardsToDraw);
	}
}

void UCardAbilityBFL::IncreaseAttack(ACard3D* CallingCard, int32 AbilityIndex)
{
	IF_RET_VOID(CallingCard);
	const int32 value=CallingCard->GetAttack()+CallingCard->mAbilities[AbilityIndex].AbilityInt;
	CallingCard->SetAttack(value);
	CallingCard->MultiCast_UpdateVisualStats();
}

void UCardAbilityBFL::CloneCard(ACard3D* CallingCard, int32 AbilityIndex, bool RandomPlacement)
{
	IF_RET_VOID(CallingCard);
	const UWorld* world=CallingCard->GetWorld();
	IF_RET_VOID(world);
	ACardPlacement* goalPlacement=nullptr;
	const int32 abilityInt=CallingCard->mAbilities[AbilityIndex].AbilityInt;
	const int32 playerID=CallingCard->GetOwningPlayerID();
	
	for (int32 i=0;i<abilityInt;++i)
	{
		if (RandomPlacement)
		{
			TArray<ACardPlacement*> placements;
			if (UPlacementBFL::GetPlayersAvailablePlacementsForCard(world,playerID,CallingCard->GetCardDataTableName(),placements))
			{
				const int32 randIndex=FMath::RandRange(0,placements.Num()-1);
				goalPlacement=placements[randIndex];
			}
		}
		else
		{
			goalPlacement=UPlacementBFL::GetClosestAvailablePlacement(CallingCard->GetPlacementOwner(),CallingCard->GetCardDataTableName(),ECardSet::BasicSet);
		}
		if (!goalPlacement)
		{
			break;
		}
		AController* controller=UControllerBFL::GetControllerReferenceFromID(CallingCard,CallingCard->GetOwningPlayerID());
		IF_RET_VOID(controller);
		if (controller->Implements<UControllerInterface>())
		{
			break;
		}
		FTransform newTransform=CallingCard->GetTransform();
		newTransform.AddToTranslation(FVector(0.f,0.0f,600.0f));
		ACard3D* card=IControllerInterface::Execute_CreatePlayableCard(controller,newTransform);
		IF_RET_VOID(card);
		card=UCardBFL::SetupCard(card,playerID,CallingCard->GetCardDataTableName(),CallingCard->GetCardSet(),FCard(),false);
		UCardBFL::SetCustomCardData(card,false,0);
		UCardBFL::AddCardToBoardPlacement(card,goalPlacement,playerID);
	}
}

void UCardAbilityBFL::IncreasePlayerHealth(ACard3D* CallingCard, int32 AbilityIndex)
{
	IF_RET_VOID(CallingCard);
	AActor* playerState=UControllerBFL::GetControllerPlayerState(CallingCard,CallingCard->GetOwningPlayerID());
	IF_RET_VOID(playerState);
	if (playerState->Implements<UPlayerStateInterface>())
	{
		IPlayerStateInterface::Execute_IncreasePlayerHealth(playerState,CallingCard->mAbilities[AbilityIndex].AbilityInt);
	}
}

void UCardAbilityBFL::DealRetaliationDamage(ACard3D* CallingCard, int32 AbilityIndex)
{
	IF_RET_VOID(CallingCard);
	if (CallingCard->mDamageDealingCard)
	{
		EInteractionConditions calling;
		EInteractionConditions talking;
		UCardInteractionBFL::DealDamageToCard(true,CallingCard,CallingCard->mDamageDealingCard,CallingCard->mAbilities[AbilityIndex].AbilityInt,calling,talking);
		CallingCard->mDamageDealingCard=nullptr;
	}
}

void UCardAbilityBFL::DamageAllCardsOnBoard(ACard3D* CallingCard, int32 AbilityIndex)
{
	IF_RET_VOID(CallingCard);
	const UWorld* world=CallingCard->GetWorld();
	IF_RET_VOID(world);
	ACCGState* gameState=Cast<ACCGState>(world->GetGameState());
	IF_RET_VOID(gameState);
	TArray<ACard3D*> cards;
	gameState->GetAllPlayerCards(cards);
	EInteractionConditions calling;
	EInteractionConditions talking;

	for (const auto& card : cards)
	{
		if (card==CallingCard||
			card->IsInGraveyard()||
			!card->IsPlaced())
		{
			continue;
		}
		UCardInteractionBFL::DealDamageToCard(true,CallingCard,card,CallingCard->mAbilities[AbilityIndex].AbilityInt,calling,talking);
	}
}

void UCardAbilityBFL::SpawnRandomCardFromDeck(ACard3D* CallingCard, int32 AbilityIndex)
{
	IF_RET_VOID(CallingCard);
	AController* controller=UControllerBFL::GetControllerReferenceFromID(CallingCard,CallingCard->GetOwningPlayerID());
	IF_RET_VOID(controller);
	const int32 loopCount=CallingCard->mAbilities[AbilityIndex].AbilityInt;
	
	for (int32 i=0;i<loopCount;++i)
	{
		FName cardName;
		const int32 cardIndex=UDeckBFL::GetRandomCardFromDeck(controller,cardName);
		ACardPlacement* goalPlacement=UPlacementBFL::GetClosestAvailablePlacement(CallingCard->GetPlacementOwner(),cardName,ECardSet::BasicSet);
		if (!goalPlacement||
			cardName.IsNone()||
			!controller->Implements<UControllerInterface>()||
			!controller->Implements<UDeckInterface>())
		{
			break;
		}
		FTransform newTransform=CallingCard->GetTransform();
		newTransform.AddToTranslation(FVector(0.f,0.0f,600.0f));
		ACard3D* card=IControllerInterface::Execute_CreatePlayableCard(controller,newTransform);
		IF_RET_VOID(card);
		UCardBFL::SetupCard(card,CallingCard->GetOwningPlayerID(),cardName,ECardSet::Empty,FCard(),false);
		UCardBFL::AddCardToBoardPlacement(card,goalPlacement,CallingCard->GetOwningPlayerID());
		IDeckInterface::Execute_RemoveCardFromDeck(controller,false,cardIndex);
	}
}

void UCardAbilityBFL::GiveTurnPointsToAllActiveCards(ACard3D* CallingCard, int32 AbilityIndex)
{
	IF_RET_VOID(CallingCard);
	const UWorld* world=CallingCard->GetWorld();
	IF_RET_VOID(world);
	ACCGState* gameState=Cast<ACCGState>(world->GetGameState());
	IF_RET_VOID(gameState);
	TArray<ACard3D*> cards;
	gameState->GetActivePlayerCards(CallingCard->GetOwningPlayerID(),cards);
	
	for (const auto& card : cards)
	{
		card->SetTurnPoint(card->GetTurnPoint()+CallingCard->mAbilities[AbilityIndex].AbilityInt);
		card->MultiCast_UpdateCardVisual(true);
	}
}

void UCardAbilityBFL::IncreaseTurnPoints(ACard3D* CallingCard, int32 AbilityIndex)
{
	IF_RET_VOID(CallingCard);
	CallingCard->SetTurnPoint(CallingCard->GetTurnPoint()+CallingCard->mAbilities[AbilityIndex].AbilityInt);
}

void UCardAbilityBFL::DiscardRandomCardFromHand(ACard3D* CallingCard, int32 AbilityIndex)
{
	IF_RET_VOID(CallingCard);
	AController* controller=UControllerBFL::GetControllerReferenceFromID(CallingCard,CallingCard->GetOwningPlayerID());
	IF_RET_VOID(controller);
	TArray<int32> IDs;
	UControllerBFL::GetControllerIDs(controller,IDs);
	IDs.Remove(CallingCard->GetOwningPlayerID());
	const int32 randOpponentID=IDs[FMath::RandRange(0,IDs.Num()-1)];
	FCardAbility ability=CallingCard->mAbilities[AbilityIndex];
	int32 loopCount=1;
	
	if (ability.AffectingPlayer==ECardPlayers::SelfAndOpponent)
	{
		loopCount=2;
	}
	for (int32 i=1;i<=loopCount;++i)
	{
		for (int32 j=0;j<ability.AbilityInt;++j)
		{
		
		}
	}
}
