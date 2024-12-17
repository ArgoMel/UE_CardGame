// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EvaluationRuleBFL.h"

#include "Gameplay/Card3D.h"

int32 UEvaluationRuleBFL::AdditionalIncreaseAttack(EAbilityTrigger Type,int32 Point, int32 AbilityInt)
{
	switch (Type)
	{
	case EAbilityTrigger::OnDrop:
		break;
	case EAbilityTrigger::StartOfTurn:
		break;
	case EAbilityTrigger::EndOfTurn:
		break;
	case EAbilityTrigger::TakesDamage:
		return Point-AbilityInt;
	case EAbilityTrigger::SentToGraveyard:
		break;
	case EAbilityTrigger::DeathByLifeExpectancy:
		break;
	default: ;
	}
	return Point;
}

int32 UEvaluationRuleBFL::AdditionalDamageAllCardsOnBoard(EAbilityTrigger Type, int32 Point, int32 AbilityInt)
{
	switch (Type)
	{
	case EAbilityTrigger::OnDrop:
		break;
	case EAbilityTrigger::StartOfTurn:
		return (Point+5)*AbilityInt;
	case EAbilityTrigger::EndOfTurn:
		break;
	case EAbilityTrigger::TakesDamage:
		break;
	case EAbilityTrigger::SentToGraveyard:
		break;
	case EAbilityTrigger::DeathByLifeExpectancy:
		return (Point+5)*AbilityInt;
	default: ;
	}
	return Point;
}

int32 UEvaluationRuleBFL::OwnedCardIncreaseAttack(int32 Point,FCard* TalkingCard,FCard* ReceivingCard, FCardAbility* Ability)
{
	switch (Ability->Trigger)
	{
	case EAbilityTrigger::OnDrop:
		break;
	case EAbilityTrigger::StartOfTurn:
		break;
	case EAbilityTrigger::EndOfTurn:
		break;
	case EAbilityTrigger::TakesDamage:
		{
			int32 damage=5;
			if (ReceivingCard->Health.Health-TalkingCard->Attack.Damage<=0)
			{
				damage=-99;
			}
			return Point+damage;
		}
	case EAbilityTrigger::SentToGraveyard:
		break;
	case EAbilityTrigger::DeathByLifeExpectancy:
		break;
	default: ;
	}
	return 0;
}

int32 UEvaluationRuleBFL::OwnedCardDamageAllCardsOnBoard(int32 Point,FCard* TalkingCard,FCard* ReceivingCard, FCardAbility* Ability)
{
	switch (Ability->Trigger)
	{
	case EAbilityTrigger::OnDrop:
		break;
	case EAbilityTrigger::StartOfTurn:
		return (Point+5)*Ability->AbilityInt;
	case EAbilityTrigger::EndOfTurn:
		break;
	case EAbilityTrigger::TakesDamage:
		break;
	case EAbilityTrigger::SentToGraveyard:
		break;
	case EAbilityTrigger::DeathByLifeExpectancy:
		return (Point+5)*Ability->AbilityInt;
	default: ;
	}
	return Point;
}

int32 UEvaluationRuleBFL::RuleCalculateActiveCardPoints(int32 Add, int32 PtsPerActiveCard, TArray<ACard3D*>& CardsActive, bool IncludePlayValue, int32& RuleTotal)
{
	if (IncludePlayValue)
	{
		RuleTotal=PtsPerActiveCard*CardsActive.Num();
		return Add+RuleTotal;
	}
	int32 tempCardValueTotal=0;
	for (const auto& card : CardsActive)
	{
		tempCardValueTotal+=card->GetCardData()->CardSystemData.CardValue;
	}
	RuleTotal=tempCardValueTotal+Add+PtsPerActiveCard*CardsActive.Num();
	return RuleTotal;
}

int32 UEvaluationRuleBFL::RuleCalculateCardsInHandPoints(int32 Add, int32 PtsPerCardInHand, int32 CardsInHand, int32& RuleTotal)
{
	RuleTotal=PtsPerCardInHand*CardsInHand;
	return RuleTotal+Add;
}

int32 UEvaluationRuleBFL::RuleCalculateHealthPoints(int32 Add, int32 PtsPerHealthPoint, int32 PlayerHealth, int32& RuleTotal)
{
	RuleTotal=PtsPerHealthPoint*PlayerHealth;
	return RuleTotal+Add;
}

int32 UEvaluationRuleBFL::RuleCalculateAttackPoints(int32 Add, int32 PtsForDamage, int32 PtsForRemovingCardFromPlay, int32 Attack, int32 Health, int32 CardValue, bool IncludeCardValue, int32& RuleTotal)
{
	RuleTotal=PtsForDamage;
	if (Health-Attack<=0)
	{
		RuleTotal=PtsForRemovingCardFromPlay;
	}
	const int32 damage=FMath::Max(Health-Attack,0);
	if (IncludeCardValue)
	{
		RuleTotal+=CardValue;	
	}
	RuleTotal-=damage;
	return Add+RuleTotal;
}

int32 UEvaluationRuleBFL::RuleCalculateDamageToPlayerPoints(int32 Add, int32 PtsForPlayerDamage, int32 PtsForRemovingPlayer, int32 CardAttack, int32 PlayerHealth,  int32& RuleTotal)
{
	const bool removedFromPlay=PlayerHealth-CardAttack<=0;
	RuleTotal=removedFromPlay?PtsForRemovingPlayer:PtsForPlayerDamage*CardAttack;
	return Add+RuleTotal;
}

int32 UEvaluationRuleBFL::RuleCalculateAdditionalAbilityPointOffset(ACard3D* TalkingCard, ACard3D* ReceivingCard)
{
	int32 returnPoint=0;
	for (const auto& ability : ReceivingCard->GetCardData()->Abilities)
	{
		switch (ability.Type)
		{
		case EAbilityType::DrawCard:
			break;
		case EAbilityType::IncreaseAttack:
			returnPoint=AdditionalIncreaseAttack(ability.Trigger,returnPoint,ability.AbilityInt);
			break;
		case EAbilityType::Clone:
			break;
		case EAbilityType::IncreasePlayerHealth:
			break;
		case EAbilityType::RetaliationDamage:
			returnPoint-=ability.AbilityInt;
			break;
		case EAbilityType::DamageAllCardsOnBoard:
			returnPoint=AdditionalDamageAllCardsOnBoard(ability.Trigger,returnPoint,ability.AbilityInt);
			break;
		case EAbilityType::SpawnRandomCardFromDeck:
			break;
		case EAbilityType::GiveTurnPointsToAllActiveCards:
			break;
		case EAbilityType::IncreaseTurnPoints:
			break;
		case EAbilityType::DiscardCardsInHand:
			break;
		case EAbilityType::PickupFirstCreatureInTheGraveyard_ToBoard:
			break;
		case EAbilityType::PickupFirstCreatureInTheGraveyard_ToHand:
			break;
		case EAbilityType::PossessOpponentsCard:
			break;
		case EAbilityType::SwapCardsInHand:
			break;
		default: ;
		}
	}
	return returnPoint;
}

bool UEvaluationRuleBFL::RuleCalculateOwnedCardAbilityPointOffset(ACard3D* TalkingCard, ACard3D* ReceivingCard, int32& Points)
{
	Points=0;
	for (auto& ability : ReceivingCard->GetCardData()->Abilities)
	{
		switch (ability.Type)
		{
		case EAbilityType::DrawCard:
			break;
		case EAbilityType::IncreaseAttack:
			Points=OwnedCardIncreaseAttack(Points,TalkingCard->GetCardData(),ReceivingCard->GetCardData(),&ability);
			break;
		case EAbilityType::Clone:
			break;
		case EAbilityType::IncreasePlayerHealth:
			break;
		case EAbilityType::RetaliationDamage:
			Points-=ability.AbilityInt*10;
			break;
		case EAbilityType::DamageAllCardsOnBoard:
			Points=OwnedCardDamageAllCardsOnBoard(Points,TalkingCard->GetCardData(),ReceivingCard->GetCardData(),&ability);
			break;
		case EAbilityType::SpawnRandomCardFromDeck:
			break;
		case EAbilityType::GiveTurnPointsToAllActiveCards:
			break;
		case EAbilityType::IncreaseTurnPoints:
			break;
		case EAbilityType::DiscardCardsInHand:
			break;
		case EAbilityType::PickupFirstCreatureInTheGraveyard_ToBoard:
			break;
		case EAbilityType::PickupFirstCreatureInTheGraveyard_ToHand:
			break;
		case EAbilityType::PossessOpponentsCard:
			break;
		case EAbilityType::SwapCardsInHand:
			break;
		default: ;
		}
	}
	return Points>=0;
}
