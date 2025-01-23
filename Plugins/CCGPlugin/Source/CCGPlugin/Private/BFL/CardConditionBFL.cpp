// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/CardConditionBFL.h"

#include "CCGPlugin.h"
#include "AI/CCGAIController.h"
#include "AI/CCGAIPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BFL/ControllerBFL.h"
#include "Gameplay/Card3D.h"
#include "Gameplay/CardPlacement.h"
#include "Gameplay/Graveyard.h"
#include "GameState/CCGState.h"

bool UCardConditionBFL::ConditionIfCardTypeInPlay(AController* Controller, int32 MinCreaturesOnBoard, bool bOpponent, ECardType CardType, bool AIOnly)
{
	IF_RET_BOOL(Controller);
	if (AIOnly&&
		!Cast<ACCGAIPawn>(Controller->GetPawn()))
	{
		return true;
	}
	TArray<int32> controllerIDs;
	int32 playerID=UControllerBFL::GetControllerIDs(Controller,controllerIDs);
	controllerIDs.Remove(playerID);
	if (bOpponent)
	{
		playerID=controllerIDs[FMath::RandRange(0,controllerIDs.Num()-1)];
	}

	const UWorld* world=Controller->GetWorld();
	IF_RET_BOOL(world);
	ACCGState* gameState=Cast<ACCGState>(world->GetGameState());
	IF_RET_BOOL(gameState);
	TArray<ACard3D*> playerCards;
	gameState->GetActivePlayerCards(playerID,playerCards);
	int32 creaturesOnBoard=0;
	for (auto& playerCard : playerCards)
	{
		if (playerCard->GetCardData()->Type==CardType)
		{
			++creaturesOnBoard;
		}
	}
	return creaturesOnBoard>=MinCreaturesOnBoard;
}

bool UCardConditionBFL::ConditionIfPlayerLowHealth(UObject* WorldContextObject, int32 PlayerID)
{
	IF_RET_BOOL(WorldContextObject);
	FPlayerStat playerStat;
	UControllerBFL::GetControllersStateStat(WorldContextObject,PlayerID,playerStat);
	return playerStat.Health/playerStat.HealthMax<=0.35f;
}

bool UCardConditionBFL::ConditionIfPlayerHighHealth(UObject* WorldContextObject, int32 PlayerID)
{
	IF_RET_BOOL(WorldContextObject);
	FPlayerStat playerStat;
	UControllerBFL::GetControllersStateStat(WorldContextObject,PlayerID,playerStat);
	return playerStat.Health/playerStat.HealthMax>=0.70f;
}

bool UCardConditionBFL::ConditionIfStartOfPlay(UObject* WorldContextObject, int32 PlayerID)
{
	IF_RET_BOOL(WorldContextObject);
	AController* controller=UControllerBFL::GetControllerReferenceFromID(WorldContextObject,PlayerID);
	IF_RET_BOOL(controller);
	ACCGAIController* AIController=Cast<ACCGAIController>(controller);
	if (!AIController)
	{
		return true;
	}

	const UBlackboardComponent* BB= AIController->GetBlackboardComponent();
	return BB->GetValueAsInt(CCG_BB::Loop)<=2;
}

bool UCardConditionBFL::ConditionIfEndOfPlay(UObject* WorldContextObject, int32 PlayerID)
{
	IF_RET_BOOL(WorldContextObject);
	AController* controller=UControllerBFL::GetControllerReferenceFromID(WorldContextObject,PlayerID);
	IF_RET_BOOL(controller);
	ACCGAIController* AIController=Cast<ACCGAIController>(controller);
	if (!AIController)
	{
		return true;
	}

	const UBlackboardComponent* BB= AIController->GetBlackboardComponent();
	return BB->GetValueAsBool(CCG_BB::FinalLoop);
}

bool UCardConditionBFL::ConditionIfPlacedOnOwnedPlacement(int32 PlayerID, ACard3D* ReceivingCard, bool Opponent)
{
	if (!ReceivingCard)
	{
		return true;
	}
	const int32 playerIndex=ReceivingCard->GetPlacementOwner()->GetPlayerIndex();
	if (Opponent)
	{
		return playerIndex!=PlayerID||playerIndex==0;
	}
	return playerIndex==PlayerID||playerIndex==0;
}

bool UCardConditionBFL::ConditionIfReceivingCardType(ACard3D* ReceivingCard, ECardType ReceivingCardType)
{
	IF_RET_BOOL(ReceivingCard);
	return ReceivingCard->GetCardData()->Type==ReceivingCardType;
}

bool UCardConditionBFL::ConditionIsNotOwningPlayer(int32 PlayerID, ACard3D* ReceivingCard)
{
	if (!ReceivingCard)
	{
		return true;
	}
	return ReceivingCard->GetOwningPlayerID()!=PlayerID;
}

bool UCardConditionBFL::ConditionIfPlayerHasCardsInGraveyard(AController* Controller, bool AIOnly, int32 MinCardsInGraveyard)
{
	IF_RET_BOOL(Controller);
	if (AIOnly&&
	!Cast<ACCGAIPawn>(Controller->GetPawn()))
	{
		return true;
	}
	const int32 controllerID=UControllerBFL::GetControllerID(Controller);
	const UWorld* world=Controller->GetWorld();
	IF_RET_BOOL(world);
	ACCGState* gameState=Cast<ACCGState>(world->GetGameState());
	IF_RET_BOOL(gameState);
	const AGraveyard* graveyard=gameState->GetGraveyardReference(controllerID);
	IF_RET_BOOL(graveyard);
	return graveyard->mGraveyardList.Num()>=MinCardsInGraveyard;
}

bool UCardConditionBFL::ConditionIfLowPlayableCardsInHand(UObject* WorldContextObject, int32 PlayerID, int32 LowValue)
{
	IF_RET_BOOL(WorldContextObject);
	ACCGAIController* AIController=Cast<ACCGAIController>(UControllerBFL::GetControllerReferenceFromID(WorldContextObject,PlayerID));
	IF_RET_BOOL(AIController);
	return AIController->GetAIPersonality()->PlayableDecks.Num()<=LowValue;
}
