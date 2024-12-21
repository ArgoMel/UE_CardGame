// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/CardAbilityBFL.h"

#include "CCGPlugin.h"
#include "BFL/CardBFL.h"
#include "BFL/CardInteractionBFL.h"
#include "BFL/CCGBFL.h"
#include "BFL/ControllerBFL.h"
#include "BFL/DeckBFL.h"
#include "BFL/PlacementBFL.h"
#include "Gameplay/Card3D.h"
#include "Gameplay/CardPlacement.h"
#include "Gameplay/Graveyard.h"
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
	AController* owningController=UControllerBFL::GetControllerReferenceFromID(CallingCard,CallingCard->GetOwningPlayerID());
	IF_RET_VOID(owningController);
	TArray<int32> IDs;
	const int32 playerID=UControllerBFL::GetControllerIDs(owningController,IDs);
	IDs.Remove(CallingCard->GetOwningPlayerID());
	const int32 randOpponentID=IDs[FMath::RandRange(0,IDs.Num()-1)];
	const FCardAbility ability=CallingCard->mAbilities[AbilityIndex];
	int32 loopCount=1;
	
	if (ability.AffectingPlayer==ECardPlayers::SelfAndOpponent)
	{
		loopCount=2;
	}
	for (int32 i=1;i<=loopCount;++i)
	{
		for (int32 j=0;j<ability.AbilityInt;++j)
		{
			int32 ID=0;
			switch (ability.AffectingPlayer)
			{
			case ECardPlayers::Self:
				ID=playerID;
				break;
			case ECardPlayers::Opponent:
				ID=randOpponentID;
				break;
			case ECardPlayers::SelfAndOpponent:
				ID=i;
				break;
			default: ;
			}
			AController* controller=UControllerBFL::GetControllerReferenceFromID(CallingCard,ID);
			IF_RET_VOID(controller);
			TArray<FName> cardsInHand;
			UControllerBFL::GetControllersStateCardsInHand(CallingCard,ID,cardsInHand);
			IF_RET_VOID(cardsInHand.IsEmpty());
			const int32 randCardIndex=FMath::RandRange(0,cardsInHand.Num()-1);
			if (cardsInHand[randCardIndex].IsNone()||
				!controller->Implements<UCardsInHandInterface>()||
				!controller->Implements<UControllerInterface>())
			{
				continue;
			}
			if (!ICardsInHandInterface::Execute_RemoveCardFromHand(controller,cardsInHand[randCardIndex],randCardIndex,false))
			{
				continue;
			}
			
			const UWorld* world=controller->GetWorld();
			IF_RET_VOID(world);
			ACCGState* gameState=Cast<ACCGState>(world->GetGameState());
			IF_RET_VOID(gameState);
			AGraveyard* graveyard=gameState->GetGraveyardReference(ID);
			IF_RET_VOID(graveyard);
			FTransform newTransform=graveyard->GetTransform();
			newTransform.AddToTranslation(FVector(0.f,0.0f,500.0f));
			ACard3D* card=IControllerInterface::Execute_CreatePlayableCard(controller,newTransform);
			IF_RET_VOID(card);
			card=UCardBFL::SetupCard(card,ID,cardsInHand[randCardIndex],ECardSet::Empty,FCard(),false);
			graveyard->Server_AddToGraveyard(card);
			card->ForceMoveCardDirectlyToGraveyard(graveyard);
		}
	}
}

void UCardAbilityBFL::PickupCardFromGraveyard(ACard3D* CallingCard, int32 AbilityIndex, bool AddDirectlyToBoard, ECardType CardType)
{
	IF_RET_VOID(CallingCard);
	AController* owningController=UControllerBFL::GetControllerReferenceFromID(CallingCard,CallingCard->GetOwningPlayerID());
	IF_RET_VOID(owningController);
	TArray<int32> IDs;
	const int32 playerID=UControllerBFL::GetControllerIDs(owningController,IDs);
	IDs.Remove(CallingCard->GetOwningPlayerID());
	const int32 randOpponentID=IDs[FMath::RandRange(0,IDs.Num()-1)];
	const FCardAbility ability=CallingCard->mAbilities[AbilityIndex];
	int32 playerIndex=0;
	
	switch (ability.AffectingPlayer)
	{
	case ECardPlayers::Self:
		playerIndex=playerID;
		break;
	case ECardPlayers::Opponent:
		playerIndex=randOpponentID;
		break;
	default: ;
	}

	const UWorld* world=owningController->GetWorld();
	IF_RET_VOID(world);
	ACCGState* gameState=Cast<ACCGState>(world->GetGameState());
	IF_RET_VOID(gameState);
	AGraveyard* graveyard=gameState->GetGraveyardReference(playerIndex);
	IF_RET_VOID(graveyard);
	FName cardName;
	int32 index;

	for (index=graveyard->mGraveyardList.Num()-1;index>=0;--index)
	{
		const FCard card=UDeckBFL::GetCardData(graveyard->mGraveyardList[index],ECardSet::Empty);
		if (CardType!=ECardType::None
			&&card.Type!=CardType)
		{
			continue;
		}
		cardName=graveyard->mGraveyardList[index];
		break;
	}
	if (cardName.IsNone())
	{
		return;	
	}

	if (AddDirectlyToBoard)
	{
		TArray<ACardPlacement*> placements;
		gameState->GetCardPlacements(CallingCard->GetOwningPlayerID(),placements);
		IF_RET_VOID(!placements.IsEmpty());
		ACardPlacement* goalPlacement;
		if (UPlacementBFL::GetIsValidPlacement(placements[0],cardName,ECardSet::Empty))
		{
			goalPlacement=placements[0];
		}
		else
		{
			goalPlacement=UPlacementBFL::GetClosestAvailablePlacement(placements[0],cardName,ECardSet::Empty);
		}
		IF_RET_VOID(goalPlacement);

		//Create and add the card back to the board
		AController* controller=UControllerBFL::GetControllerReferenceFromID(CallingCard,playerIndex);
		IF_RET_VOID(controller);
		IF_RET_VOID(controller->Implements<UControllerInterface>());
		ACard3D* card3D=IControllerInterface::Execute_CreatePlayableCard(controller,FTransform(graveyard->GetCardInGraveyardLoc()));
		IF_RET_VOID(card3D);
		const FCard card=UDeckBFL::GetCardData(cardName,ECardSet::Empty);
		FCard graveyardCard=graveyard->mGraveyardStructList[index];
		graveyardCard.Health=card.Health;
		card3D=UCardBFL::SetupCard(card3D,CallingCard->GetOwningPlayerID(),cardName,ECardSet::Empty,graveyardCard,true);
		UCardBFL::SetCustomCardData(card3D,true,0);
		
		if (!UCardBFL::AddCardToBoardPlacement(card3D,goalPlacement,CallingCard->GetOwningPlayerID()))
		{
			return;
		}
	}
	else
	{
		if (owningController->Implements<UCardsInHandInterface>())
		{
			ICardsInHandInterface::Execute_DrawCard(owningController,cardName,false,1);
		}
	}

	graveyard->Server_RemoveCardFromGraveyard(cardName,index);
}

void UCardAbilityBFL::ChangePlayedCardOwner(ACard3D* CallingCard, int32 AbilityIndex)
{
	IF_RET_VOID(CallingCard);
	AController* owningController=UControllerBFL::GetControllerReferenceFromID(CallingCard,CallingCard->GetOwningPlayerID());
	IF_RET_VOID(owningController);
	TArray<int32> IDs;
	const int32 playerID=UControllerBFL::GetControllerIDs(owningController,IDs);
	IDs.Remove(CallingCard->GetOwningPlayerID());
	const int32 randOpponentID=IDs[FMath::RandRange(0,IDs.Num()-1)];
	const FCardAbility ability=CallingCard->mAbilities[AbilityIndex];
	int32 loopCount=1;
	
	if (ability.AffectingPlayer==ECardPlayers::SelfAndOpponent)
	{
		loopCount=2;
	}
	for (int32 i=1;i<=loopCount;++i)
	{
		for (int32 j=0;j<ability.AbilityInt;++j)
		{
			int32 ID=0;
			switch (ability.AffectingPlayer)
			{
			case ECardPlayers::Self:
				ID=playerID;
				break;
			case ECardPlayers::Opponent:
				ID=randOpponentID;
				break;
			case ECardPlayers::SelfAndOpponent:
				ID=i;
				break;
			default: ;
			}

			const UWorld* world=CallingCard->GetWorld();
			IF_RET_VOID(world);
			ACCGState* gameState=Cast<ACCGState>(world->GetGameState());
			IF_RET_VOID(gameState);
			TArray<ACard3D*> opponentCards;
			gameState->GetActivePlayerCards(randOpponentID,opponentCards);
			UCCGBFL::ShuffleArray(opponentCards,FMath::Rand()%1000);
			TArray<ACard3D*> ignoreCards;
			ACard3D* card3D=nullptr;
			for (auto& opponentCard : opponentCards)
			{
				const int32 playerIndex=opponentCard->GetPlacementOwner()->GetPlayerIndex();
				if (playerIndex!=ID
					&&opponentCard->GetOwningPlayerID()!=ID
					&&!ignoreCards.Contains(opponentCard))
				{
					card3D=opponentCard;
					break;
				}
				if (playerIndex==0)
				{
					card3D=opponentCard;
					break;
				}
			}
			if (!card3D)
			{
				return;	
			}

			ACardPlacement* goalPlacement;
			TArray<ACardPlacement*> placements;
			gameState->GetCardPlacements(ID,placements);
			IF_RET_VOID(!placements.IsEmpty());
			if (UPlacementBFL::GetIsValidPlacement(placements[0],card3D->GetCardDataTableName(),ECardSet::Empty))
			{
				goalPlacement=placements[0];
			}
			else
			{
				goalPlacement=UPlacementBFL::GetClosestAvailablePlacement(placements[0],card3D->GetCardDataTableName(),ECardSet::Empty);
			}
			IF_RET_VOID(goalPlacement);

			ignoreCards.Add(card3D);
			gameState->RemoveCardOnBoard(card3D,card3D->GetOwningPlayerID());
			ACardPlacement* oldPlacement=card3D->GetPlacementOwner();
			IF_RET_VOID(oldPlacement);
			oldPlacement->Server_RemoveCardFromPlacement_Implementation(card3D);
			card3D->SetOwningPlayerID(ID);
			UCardBFL::SetCustomCardData(card3D,true,1);
			UCardBFL::AddCardToBoardPlacement(card3D,goalPlacement,ID);
		}
	}
}

void UCardAbilityBFL::ChangeInHandCardOwner(ACard3D* CallingCard, int32 AbilityIndex)
{
	IF_RET_VOID(CallingCard);
	const int32 playerID=CallingCard->GetOwningPlayerID();
	AController* playerController=UControllerBFL::GetControllerReferenceFromID(CallingCard,playerID);
	IF_RET_VOID(playerController);
	TArray<int32> IDs;
	UControllerBFL::GetControllerIDs(playerController,IDs);
	IDs.Remove(playerID);
	const int32 randOpponentID=IDs[FMath::RandRange(0,IDs.Num()-1)];
	TArray<FName> playerCardsInHand;
	UControllerBFL::GetControllersStateCardsInHand(CallingCard,playerID,playerCardsInHand);
	TArray<FName> opponentCardsInHand;
	UControllerBFL::GetControllersStateCardsInHand(CallingCard,randOpponentID,opponentCardsInHand);

	if (playerController->Implements<UCardsInHandInterface>())
	{
		ICardsInHandInterface::Execute_RemoveCardFromHand(playerController,FName(),0,true);
	}
	if (playerController->Implements<UControllerInterface>())
	{
		for (const auto& card : opponentCardsInHand)
		{
			IControllerInterface::Execute_AddCardToPlayersHand(playerController,card);
		}
	}
	
	AController* opponentController=UControllerBFL::GetControllerReferenceFromID(CallingCard,randOpponentID);
	IF_RET_VOID(opponentController);
	if (opponentController->Implements<UCardsInHandInterface>())
	{
		ICardsInHandInterface::Execute_RemoveCardFromHand(opponentController,FName(),0,true);
	}
	if (opponentController->Implements<UControllerInterface>())
	{
		for (const auto& card : playerCardsInHand)
		{
			IControllerInterface::Execute_AddCardToPlayersHand(opponentController,card);
		}
	}
}
