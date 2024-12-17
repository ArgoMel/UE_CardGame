// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EvaluationRuleBFL.generated.h"

class ACard3D;

UCLASS()
class CCGPLUGIN_API UEvaluationRuleBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
private:
	static int32 AdditionalIncreaseAttack(EAbilityTrigger Type,int32 Point,int32 AbilityInt);
	static int32 AdditionalDamageAllCardsOnBoard(EAbilityTrigger Type,int32 Point,int32 AbilityInt);

	static int32 OwnedCardIncreaseAttack(int32 Point,FCard* TalkingCard,FCard* ReceivingCard, FCardAbility* Ability);
	static int32 OwnedCardDamageAllCardsOnBoard(int32 Point,FCard* TalkingCard,FCard* ReceivingCard, FCardAbility* Ability);
	
public:
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static int32 RuleCalculateActiveCardPoints(int32 Add, int32 PtsPerActiveCard, TArray<ACard3D*>& CardsActive, bool IncludePlayValue, int32& RuleTotal);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static int32 RuleCalculateCardsInHandPoints(int32 Add, int32 PtsPerCardInHand, int32 CardsInHand, int32& RuleTotal);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static int32 RuleCalculateHealthPoints(int32 Add, int32 PtsPerHealthPoint, int32 PlayerHealth, int32& RuleTotal);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static int32 RuleCalculateAttackPoints(int32 Add, int32 PtsForDamage, int32 PtsForRemovingCardFromPlay, int32 Attack, int32 Health, int32 CardValue, bool IncludeCardValue, int32& RuleTotal);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static int32 RuleCalculateDamageToPlayerPoints(int32 Add, int32 PtsForPlayerDamage, int32 PtsForRemovingPlayer, int32 CardAttack, int32 PlayerHealth, int32& RuleTotal);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static int32 RuleCalculateAdditionalAbilityPointOffset(ACard3D* TalkingCard, ACard3D* ReceivingCard);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static bool RuleCalculateOwnedCardAbilityPointOffset(ACard3D* TalkingCard, ACard3D* ReceivingCard, int32& Points);
};
