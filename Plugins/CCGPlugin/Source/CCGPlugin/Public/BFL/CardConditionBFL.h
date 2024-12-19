// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CardConditionBFL.generated.h"

class ACard3D;

UCLASS()
class CCGPLUGIN_API UCardConditionBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool ConditionIfCardTypeInPlay(AController* Controller, int32 MinCreaturesOnBoard, bool bOpponent, ECardType CardType, bool AIOnly=false);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool ConditionIfPlayerLowHealth(UObject* WorldContextObject,int32 PlayerID);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool ConditionIfPlayerHighHealth(UObject* WorldContextObject,int32 PlayerID);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure,DisplayName="ConditionIfStartOfPlay(AIOnly)")
	static bool ConditionIfStartOfPlay(UObject* WorldContextObject,int32 PlayerID);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure,DisplayName="ConditionIfEndOfPlay(AIOnly)")
	static bool ConditionIfEndOfPlay(UObject* WorldContextObject,int32 PlayerID);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool ConditionIfPlacedOnOwnedPlacement(int32 PlayerID, ACard3D* ReceivingCard, bool Opponent=false);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool ConditionIfReceivingCardType(ACard3D* ReceivingCard, ECardType ReceivingCardType);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool ConditionIsNotOwningPlayer(int32 PlayerID, ACard3D* ReceivingCard);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool ConditionIfPlayerHasCardsInGraveyard(AController* Controller, bool AIOnly, int32 MinCardsInGraveyard);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure,DisplayName="ConditionIfLowPlayableCardsInHand(AIOnly)")
	static bool ConditionIfLowPlayableCardsInHand(UObject* WorldContextObject,int32 PlayerID, int32 LowValue);
};
