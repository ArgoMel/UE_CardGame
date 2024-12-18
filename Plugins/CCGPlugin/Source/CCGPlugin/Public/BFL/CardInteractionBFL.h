// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CardInteractionBFL.generated.h"

class ACard3D;

UCLASS()
class CCGPLUGIN_API UCardInteractionBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	// UFUNCTION(BlueprintCallable,DisplayName="Deal Damage to Card")
	// static void DealDamageToCard(bool SimpleDamage, ACard3D* TalkingCard, ACard3D* ReceivingCard, int32 DamageOverride, UObject* __WorldContext, bool& Success, EInteractionConditions& CallingCardCondition, TEnumAsByte<InteractionConditions_Enum>& TalkingCardCondition, ACard3D* InteractionTalkingCard, A3DCard_C* InteractionReceivingCard, TEnumAsByte<InteractionConditions_Enum> CallingCardConditionFailure, TEnumAsByte<InteractionConditions_Enum> TalkingCardConditionFailure);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable)
	// static void Deal Damage to Player(bool SimpleDamage, A3DCard_C* TalkingCard, ABoardPlayer_C* BoardPlayer, int32 DamageOverride, UObject* __WorldContext, bool& Success, TEnumAsByte<InteractionConditions_Enum>& InteractionConditionFailure, A3DCard_C* InteractionTalkingCard, ABoardPlayer_C* InteractionReceivingPlayer, TEnumAsByte<InteractionConditions_Enum> ConditionFailure);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable)
	// static void ChargeInteractionPoints(A3DCard_C* CardToCharge, int32 Cost, UObject* __WorldContext, bool& Success);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure)
	// static bool IsSameOwningPlayer(A3DCard_C* TalkingCard, A3DCard_C* ReceivingCard, UObject* __WorldContext);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure)
	// static bool ValidateCardInteraction(A3DCard_C* TalkingCard, A3DCard_C* ReceivingCard, UObject* __WorldContext, TEnumAsByte<InteractionConditions_Enum>& CallingCardConditionFailure, TEnumAsByte<InteractionConditions_Enum>& TalkingCardConditionFailure);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure)
	// static bool ValidatePlayerInteraction(A3DCard_C* TalkingCard, ABoardPlayer_C* ReceivingBoardPlayer, UObject* __WorldContext, TEnumAsByte<InteractionConditions_Enum>& ConditionFailure);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool ValidateCardInteractionConditions(ACard3D* TalkingCard, ACard3D * ReceivingCard, bool ReceivingCardConditions, EInteractionConditions& ConditionFailure);
};
