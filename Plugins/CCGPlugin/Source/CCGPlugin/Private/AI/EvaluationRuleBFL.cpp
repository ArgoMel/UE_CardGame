// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EvaluationRuleBFL.h"

int32 UEvaluationRuleBFL::RuleCalculateActiveCardPoints(int32 Add, int32 PtsPerActiveCard,
	TArray<ACard3D*>& CardsActive, bool IncludePlayValue, int32& RuleTotal)
{
	return 0;
}

int32 UEvaluationRuleBFL::RuleCalculateCardsInHandPoints(int32 Add, int32 PtsPerCardInHand, int32 CardsInHand, int32& RuleTotal)
{
	return 0;
}

int32 UEvaluationRuleBFL::RuleCalculateHealthPoints(int32 Add, int32 PtsPerHealthPoint, int32 PlayerHealth, int32& RuleTotal)
{
	return 0;
}

bool UEvaluationRuleBFL::RuleCalculateAttackPoints(int32 Add, int32 PtsForDamage, int32 PtsForRemovingCardFromPlay, int32 Attack, int32 Health, int32 CardValue, bool IncludeCardValue, int32& Total, int32& RuleTotal)
{
	return false;
}

bool UEvaluationRuleBFL::RuleCalculateDamageToPlayerPoints(int32 Add, int32 PtsForPlayerDamage, int32 PtsForRemovingPlayer, int32 CardAttack, int32 PlayerHealth, int32& Total, int32& RuleTotal)
{
	return false;
}

int32 UEvaluationRuleBFL::RuleCalculateAdditionalAbilityPointOffset(ACard3D* TalkingCard, ACard3D* ReceivingCard)
{
	return 0;
}

bool UEvaluationRuleBFL::RuleCalculateOwnedCardAbilityPointOffset(ACard3D* TalkingCard, ACard3D* ReceivingCard, int32& Points)
{
	return false;
}
