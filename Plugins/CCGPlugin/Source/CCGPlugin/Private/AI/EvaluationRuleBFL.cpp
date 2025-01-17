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

FAITotalScore UEvaluationRuleBFL::RuleCalculateActiveCardPoints(int32 Add, int32 PtsPerActiveCard, TArray<ACard3D*>& CardsActive, bool IncludePlayValue)
{
	FAITotalScore total;
	if (IncludePlayValue)
	{
		total.RuleTotal=PtsPerActiveCard*CardsActive.Num();
		total.Total= Add+total.RuleTotal;
		return total;
	}
	int32 tempCardValueTotal=0;
	for (const auto& card : CardsActive)
	{
		tempCardValueTotal+=card->GetCardData()->CardSystemData.CardValue;
	}
	total.RuleTotal=tempCardValueTotal+Add+PtsPerActiveCard*CardsActive.Num();
	total.Total= total.RuleTotal;
	return total;
}

FAITotalScore UEvaluationRuleBFL::RuleCalculateCardsInHandPoints(int32 Add, int32 PtsPerCardInHand, int32 CardsInHand)
{
	FAITotalScore total;
	total.RuleTotal=PtsPerCardInHand*CardsInHand;
	total.Total= total.RuleTotal+Add;
	return total;
}

FAITotalScore UEvaluationRuleBFL::RuleCalculateHealthPoints(int32 Add, int32 PtsPerHealthPoint, int32 PlayerHealth)
{
	FAITotalScore total;
	total.RuleTotal=PtsPerHealthPoint*PlayerHealth;
	total.Total= total.RuleTotal+Add;
	return total;
}

FAITotalScore UEvaluationRuleBFL::RuleCalculateAttackPoints(int32 Add, int32 PtsForDamage, int32 PtsForRemovingCardFromPlay, int32 Attack, int32 Health, int32 CardValue, bool IncludeCardValue)
{
	FAITotalScore total;
	total.RuleTotal=PtsForDamage;
	if (Health-Attack<=0)
	{
		total.RuleTotal=PtsForRemovingCardFromPlay;
	}
	const int32 damage=FMath::Max(Health-Attack,0);
	if (IncludeCardValue)
	{
		total.RuleTotal+=CardValue;	
	}
	total.RuleTotal-=damage;
	total.Total= Add+total.RuleTotal;
	return total;
}

FAITotalScore UEvaluationRuleBFL::RuleCalculateDamageToPlayerPoints(int32 Add, int32 PtsForPlayerDamage, int32 PtsForRemovingPlayer, int32 CardAttack, int32 PlayerHealth)
{
	FAITotalScore total;
	const bool removedFromPlay=PlayerHealth-CardAttack<=0;
	total.RuleTotal=removedFromPlay?PtsForRemovingPlayer:PtsForPlayerDamage*CardAttack;
	total.Total= Add+total.RuleTotal;
	return total;
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
