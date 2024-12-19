// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CardInteractionBFL.generated.h"

class ABoardPlayer;
class ACard3D;

UCLASS()
class CCGPLUGIN_API UCardInteractionBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Deal Damage to Card")
	static bool DealDamageToCard(bool SimpleDamage, ACard3D* TalkingCard, ACard3D* ReceivingCard, int32 DamageOverride, EInteractionConditions& CallingCardCondition, EInteractionConditions& TalkingCardCondition);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Deal Damage to Player")
	static bool DealDamageToPlayer(bool SimpleDamage, ACard3D* TalkingCard, ABoardPlayer* BoardPlayer, int32 DamageOverride, EInteractionConditions& InteractionConditionFailure);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static bool ChargeInteractionPoints(ACard3D* CardToCharge, int32 Cost);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool IsSameOwningPlayer(ACard3D* TalkingCard, ACard3D* ReceivingCard);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool ValidateCardInteraction(ACard3D* TalkingCard, ACard3D* ReceivingCard, EInteractionConditions& CallingCardConditionFailure, EInteractionConditions& TalkingCardConditionFailure);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool ValidatePlayerInteraction(ACard3D* TalkingCard, ABoardPlayer* ReceivingBoardPlayer,EInteractionConditions& ConditionFailure);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool ValidateCardInteractionConditions(ACard3D* TalkingCard, ACard3D * ReceivingCard, bool ReceivingCardConditions, EInteractionConditions& ConditionFailure);
};
