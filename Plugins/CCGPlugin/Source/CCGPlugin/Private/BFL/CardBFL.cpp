// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/CardBFL.h"

#include "CCGPlugin.h"
#include "BFL/CardConditionBFL.h"
#include "BFL/ControllerBFL.h"
#include "BFL/DeckBFL.h"
#include "Gameplay/Card3D.h"
#include "Gameplay/CardPlacement.h"
#include "GameState/CCGState.h"
#include "Interface/PlayerStateInterface.h"

TObjectPtr<UDataTable> UCardBFL::mPreBuildDeckDT;

UCardBFL::UCardBFL()
{
	GetObjectAsset(mPreBuildDeckDT,UDataTable,"/CCGPlugin/CCGToolkit/CardGame/DEVELOPER_PrebuildDecks/PreBuildDecks_DataTable.PreBuildDecks_DataTable");
}

ACard3D* UCardBFL::SetupCard(ACard3D* CardRef, int32 OwningPlayerID, FName CardName, ECardSet CardSet, FCard CardStruct, bool SetCardDataFromStruct)
{
	IF_RET_NULL(CardRef);
	CardRef->SetOwningPlayerID(OwningPlayerID);
	CardRef->SetReplicates(true);
	CardRef->Server_SetCardData(CardName,CardSet,CardStruct,SetCardDataFromStruct);
	return CardRef;
}

bool UCardBFL::AddCardToBoardPlacement(ACard3D* AddCard, ACardPlacement* GoalPlacement, int32 OwningPlayerID)
{
	IF_RET_BOOL(AddCard);
	IF_RET_BOOL(GoalPlacement);
	GoalPlacement->Server_AddCardToBoardPlacement(AddCard);
	AddCard->SetPlacementOwner(GoalPlacement);
	const UWorld* world = AddCard->GetWorld();
	IF_RET_BOOL(world);
	ACCGState* gameState=Cast<ACCGState>(world->GetGameState());
	IF_RET_BOOL(gameState);
	gameState->AddCardToBoard(AddCard,OwningPlayerID);
	return true;
}

void UCardBFL::LoadPreBuiltDeck(FName RowName,TArray<FName>& ReturnDeck)
{
	ReturnDeck.Empty();
	TArray<FPrebuildDeckSub> cardsInDeck=mPreBuildDeckDT->FindRow<FPrebuildDeck>(RowName,nullptr,false)->CardsInDeck;
	for (auto& card : cardsInDeck)
	{
		for (int32 i=0;i<card.Quantity;++i)
		{
			ReturnDeck.Add(card.CardTableName);
		}
	}
}

void UCardBFL::SetCustomCardData(ACard3D* Card3D, bool AbilityActive, int32 TurnPoints)
{
	IF_RET_VOID(Card3D);
	Card3D->SetIsAbilityActive(AbilityActive);
	if (TurnPoints>0)
	{
		Card3D->SetTurnPoint(1);
	}
}

ACard3D* UCardBFL::GetCardReferenceFromID(UObject* WorldContextObject, int32 CardID)
{
	IF_RET_NULL(WorldContextObject);
	const UWorld* world = WorldContextObject->GetWorld();
	IF_RET_NULL(world);
	ACCGState* gameState=Cast<ACCGState>(world->GetGameState());
	IF_RET_NULL(gameState);
	return gameState->mCardReferenceArray[CardID];
}

void UCardBFL::FilterCardsWithTurnPoints(TArray<ACard3D*>& CardArray, TArray<ECardType>& ValidCardTypes, TArray<ACard3D*>& ReturnCardArray)
{
	for (auto& card : CardArray)
	{
		if (card->GetTurnPoint()>0&&
			ValidCardTypes.Contains(card->GetCardType()))
		{
			ReturnCardArray.Add(card);
		}
	}
}

bool UCardBFL::ChargeCardManaCost(UObject* WorldContextObject, int32 PlayerID, bool SkipManaCheck, FCard CardStruct, EManaAccount AccountToCharge, int32& Cost)
{
	IF_RET_BOOL(WorldContextObject);
	switch (AccountToCharge)
	{
	case EManaAccount::Placement:
		Cost=CardStruct.PlacementSetting.ManaCost;
		break;
	case EManaAccount::Attack:
		Cost=CardStruct.Attack.ManaCost;
		break;
	default:
		Cost=0;
		break;
	}
	if (SkipManaCheck)
	{
		return true;		
	}
	FPlayerStat playerStat;
	AActor* playerState=UControllerBFL::GetControllersStateStat(WorldContextObject,PlayerID,playerStat);
	IF_RET_BOOL(playerState);
	if (playerStat.Mana<Cost)
	{
		return false;
	}
	if (playerState->Implements<UPlayerStateInterface>())
	{
		IPlayerStateInterface::Execute_DecreasePlayerMana(playerState,Cost);
	}
	return true;
}

bool UCardBFL::ValidateCardPlayConditions(FName CardName, AController* CallingController,
	ECardPlayConditions& ConditionFailure)
{
	IF_RET_BOOL(CallingController);
	bool valid=true;
	ConditionFailure=ECardPlayConditions::None;
	FCard card=UDeckBFL::GetCardData(CardName,ECardSet::Empty);
	const int32 controllerID=UControllerBFL::GetControllerID(CallingController);

	for (const auto& condition:card.CardSystemData.PlayConditions)
	{
		ConditionFailure=condition;
		switch (condition)
		{
		case ECardPlayConditions::PlayAtStartOfTurn_AIOnly:
			valid=UCardConditionBFL::ConditionIfStartOfPlay(CallingController,controllerID);
			break;
		case ECardPlayConditions::PlayAtEndOfTurn_AIOnly:
			valid=UCardConditionBFL::ConditionIfEndOfPlay(CallingController,controllerID);
			break;
		case ECardPlayConditions::PlayWhenLowHealth_Self:
			valid=UCardConditionBFL::ConditionIfPlayerLowHealth(CallingController,controllerID);
			break;
		case ECardPlayConditions::PlayWhenHighHealth_Self:
			valid=UCardConditionBFL::ConditionIfPlayerHighHealth(CallingController,controllerID);
			break;
		case ECardPlayConditions::PlayWhenCreaturesAreOnTheBoard_Self:
			valid=UCardConditionBFL::ConditionIfCardTypeInPlay(CallingController,2,false,ECardType::Creature,false);
			break;
		case ECardPlayConditions::PlayWhenCreaturesAreOnTheBoard_Self_AIOnly:
			valid=UCardConditionBFL::ConditionIfCardTypeInPlay(CallingController,2,true,ECardType::Creature,false);
			break;
		case ECardPlayConditions::PlayWhenCreaturesAreOnTheBoard_Opponent:
			valid=UCardConditionBFL::ConditionIfCardTypeInPlay(CallingController,2,false,ECardType::Creature,false);
			break;
		case ECardPlayConditions::PlayWhenCreaturesAreOnTheBoard_Opponent_AIOnly:
			valid=UCardConditionBFL::ConditionIfCardTypeInPlay(CallingController,2,true,ECardType::Creature,true);
			break;
		case ECardPlayConditions::PlayWhenThereAreCardsInTheGraveyard:
			valid=UCardConditionBFL::ConditionIfPlayerHasCardsInGraveyard(CallingController,false,5);
			break;
		case ECardPlayConditions::PlayWhenThereAreCardsInTheGraveyard_AIOnly:
			valid=UCardConditionBFL::ConditionIfPlayerHasCardsInGraveyard(CallingController,true,5);
			break;
		case ECardPlayConditions::PlayWhenLowPlayableCardInHand_AIOnly:
			valid=UCardConditionBFL::ConditionIfLowPlayableCardsInHand(CallingController,controllerID,2);
			break;
		default: ;
		}
		if (!valid)
		{
			break;
		}
	}
	return valid;
}

bool UCardBFL::HasAbility(ACard3D* Card, EAbilityType Ability, EAbilityTrigger Trigger)
{
	IF_RET_BOOL(Card);
	for (const auto& ability:Card->GetCardData()->Abilities)
	{
		if (ability.Type==Ability&&
			(ability.Trigger==Trigger||ability.Trigger!=EAbilityTrigger::None))
		{
			return true;
		}
	}
	return false;
}

void UCardBFL::GetAllPreBuildDeckNames(TArray<FName>& Names)
{
	Names=mPreBuildDeckDT->GetRowNames();
}
