// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/CardInteractionBFL.h"

#include "CCGPlugin.h"
#include "BFL/CardConditionBFL.h"
#include "BFL/ControllerBFL.h"
#include "BFL/MiscBFL.h"
#include "Gameplay/BoardPlayer.h"
#include "Gameplay/Card3D.h"
#include "GameState/CCGState.h"
#include "Interface/CardInteractionInterface.h"
#include "Interface/PlayerStateInterface.h"

bool UCardInteractionBFL::DealDamageToCard(bool SimpleDamage, ACard3D* TalkingCard, ACard3D* ReceivingCard, int32 DamageOverride, EInteractionConditions& CallingCardCondition, EInteractionConditions& TalkingCardCondition)
{
	IF_RET_BOOL(TalkingCard);
	IF_RET_BOOL(ReceivingCard);
	
	if (!SimpleDamage)
	{
		if (!ValidateCardInteraction(TalkingCard, ReceivingCard, CallingCardCondition,TalkingCardCondition))
		{
			return false;
		}
		if (!ChargeInteractionPoints(TalkingCard,1))
		{
			return false;
		}
		TalkingCard->MultiCast_Attacking(ReceivingCard);
	}
	
	if (ReceivingCard->Implements<UCardInteractionInterface>())
	{
		if (DamageOverride<=0)
		{
			DamageOverride=UMiscBFL::ReadInteractingCardDataAttack(TalkingCard);
		}
		ICardInteractionInterface::Execute_TakesDamage(ReceivingCard,TalkingCard,0,DamageOverride);
	}

	const UWorld* world=TalkingCard->GetWorld();
	IF_RET_BOOL(world);
	ACCGState* gameState=Cast<ACCGState>(world->GetGameState());
	IF_RET_BOOL(gameState);
	FBattleHistory battleHistory;
	battleHistory.OwningControllerID=TalkingCard->GetOwningPlayerID();
	battleHistory.InteractionType=EPlayType::AttackCard;
	battleHistory.TalkingCard=*TalkingCard->GetCardData();
	battleHistory.ReceivingCard=*ReceivingCard->GetCardData();
	battleHistory.ReceivingPlayerID=0;
	gameState->RecordBattleHistory(battleHistory);
	return true;
}

bool UCardInteractionBFL::DealDamageToPlayer(bool SimpleDamage, ACard3D* TalkingCard, ABoardPlayer* BoardPlayer, int32 DamageOverride, EInteractionConditions& InteractionConditionFailure)
{
	IF_RET_BOOL(TalkingCard);
	IF_RET_BOOL(BoardPlayer);
	
	if (!SimpleDamage)
	{
		if (!ValidatePlayerInteraction(TalkingCard, BoardPlayer, InteractionConditionFailure))
		{
			return false;
		}
		if (!ChargeInteractionPoints(TalkingCard,1))
		{
			return false;
		}
		TalkingCard->MultiCast_Attacking(BoardPlayer);
	}

	AActor* playerState= UControllerBFL::GetControllerPlayerState(BoardPlayer,BoardPlayer->mPlayerIndex);
	IF_RET_BOOL(playerState);
	if (playerState->Implements<UPlayerStateInterface>())
	{
		if (DamageOverride<=0)
		{
			DamageOverride=UMiscBFL::ReadInteractingCardDataAttack(TalkingCard);
		}
		IPlayerStateInterface::Execute_DecreasePlayerHealth(playerState,DamageOverride);
	}

	const UWorld* world=TalkingCard->GetWorld();
	IF_RET_BOOL(world);
	ACCGState* gameState=Cast<ACCGState>(world->GetGameState());
	IF_RET_BOOL(gameState);
	FBattleHistory battleHistory;
	battleHistory.OwningControllerID=TalkingCard->GetOwningPlayerID();
	battleHistory.InteractionType=EPlayType::AttackPlayer;
	battleHistory.TalkingCard=*TalkingCard->GetCardData();
	battleHistory.ReceivingPlayerID=BoardPlayer->mPlayerIndex;
	gameState->RecordBattleHistory(battleHistory);
	return true;
}

bool UCardInteractionBFL::ChargeInteractionPoints(ACard3D* CardToCharge, int32 Cost)
{
	IF_RET_BOOL(CardToCharge);
	if (CardToCharge->GetTurnPoint()<1)
	{
		return false;
	}
	CardToCharge->SetTurnPoint(CardToCharge->GetTurnPoint()-Cost);
	return true;
}

bool UCardInteractionBFL::IsSameOwningPlayer(ACard3D* TalkingCard, ACard3D* ReceivingCard)
{
	return TalkingCard->GetOwningPlayerID() == ReceivingCard->GetOwningPlayerID();
}

bool UCardInteractionBFL::ValidateCardInteraction(ACard3D* TalkingCard, ACard3D* ReceivingCard, EInteractionConditions& CallingCardConditionFailure, EInteractionConditions& TalkingCardConditionFailure)
{
	IF_RET_BOOL(TalkingCard);
	IF_RET_BOOL(ReceivingCard);
	
	return TalkingCard->GetTurnPoint()>0
	&&TalkingCard->GetAttack()>0
	&&!TalkingCard->IsInGraveyard()
	&&TalkingCard->GetCardData()->Attack.CanAttackCards
	&&ReceivingCard->GetHealth()>0
	&&!ReceivingCard->IsInGraveyard()
	&&ValidateCardInteractionConditions(TalkingCard,ReceivingCard,false,CallingCardConditionFailure)
	&&ValidateCardInteractionConditions(TalkingCard,ReceivingCard,true,TalkingCardConditionFailure);
}

bool UCardInteractionBFL::ValidatePlayerInteraction(ACard3D* TalkingCard, ABoardPlayer* ReceivingBoardPlayer, EInteractionConditions& ConditionFailure)
{
	IF_RET_BOOL(TalkingCard);
	IF_RET_BOOL(ReceivingBoardPlayer);
	FPlayerStat playerStat;
	UControllerBFL::GetControllersStateStat(TalkingCard,ReceivingBoardPlayer->mPlayerIndex,playerStat);
	return TalkingCard->GetTurnPoint()>0
	&&TalkingCard->GetAttack()>0
	&&!TalkingCard->IsInGraveyard()
	&&TalkingCard->GetCardData()->Attack.CanAttackPlayer
	&&playerStat.Health>=0
	&&ValidateCardInteractionConditions(TalkingCard,nullptr,false,ConditionFailure);
}

bool UCardInteractionBFL::ValidateCardInteractionConditions(ACard3D* TalkingCard, ACard3D* ReceivingCard, bool ReceivingCardConditions, EInteractionConditions& ConditionFailure)
{
	bool valid=true;
	ConditionFailure=EInteractionConditions::None;
	IF_RET_BOOL(TalkingCard);
	if (!ReceivingCard)
	{
		ReceivingCardConditions=false;
	}
	TArray<EInteractionConditions> conditions=ReceivingCardConditions?ReceivingCard->GetCardData()->CardSystemData.InteractionConditions:TalkingCard->GetCardData()->CardSystemData.InteractionConditions;
	
	for (const auto& condition : conditions)
	{
		ConditionFailure=condition;
		switch (condition) 
		{
		case EInteractionConditions::CardCanOnlyReceiveInteractionOnOwnedPlacement:
			valid=UCardConditionBFL::ConditionIfPlacedOnOwnedPlacement(TalkingCard->GetOwningPlayerID(),ReceivingCard,false);
			break;
		case EInteractionConditions::CardCanOnlyReceiveInteractionOnAnOpponentsPlacement:
			valid=UCardConditionBFL::ConditionIfPlacedOnOwnedPlacement(TalkingCard->GetOwningPlayerID(),ReceivingCard,true);
			break;
		case EInteractionConditions::CanOnlyAttackCreatures:
			valid=UCardConditionBFL::ConditionIfReceivingCardType(ReceivingCard,ECardType::Creature);
			break;
		case EInteractionConditions::CannotAttackIfPlayerOwnerCard:
			valid=UCardConditionBFL::ConditionIsNotOwningPlayer(TalkingCard->GetOwningPlayerID(),ReceivingCard);
			break;
		case EInteractionConditions::CanReceiveSelfInteraction_AIOnly:
			break;
		case EInteractionConditions::CanReceiveSelfInteractionIfDoesNotRemoveCardFromPlay_AIOnly:
			break;
		default: ;
		}
		if (!valid)
		{
			break;
		}
	}
	if (valid)
	{
		ConditionFailure=EInteractionConditions::None;
	}
	return valid;
}
