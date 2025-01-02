// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CardBFL.generated.h"

class ACard3D;

UCLASS()
class CCGPLUGIN_API UCardBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UCardBFL();

private:
	static TObjectPtr<UDataTable> mPreBuildDeckDT;
	
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static ACard3D* SetupCard(ACard3D* CardRef, int32 OwningPlayerID, FName CardName, ECardSet CardSet, FCard CardStruct, bool SetCardDataFromStruct);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static bool AddCardToBoardPlacement(ACard3D* AddCard, ACardPlacement* GoalPlacement, int32 OwningPlayerID);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static void LoadPreBuiltDeck(FName RowName,TArray<FName>& ReturnDeck);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void SetCustomCardData(ACard3D* Card3D, bool AbilityActive, int32 TurnPoints);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static ACard3D* GetCardReferenceFromID(UObject* WorldContextObject,int32 CardID);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static void FilterCardsWithTurnPoints(TArray<ACard3D*>& CardArray, TArray<ECardType>& ValidCardTypes, TArray<ACard3D*>& ReturnCardArray);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static bool ChargeCardManaCost(UObject* WorldContextObject,int32 PlayerID, bool SkipManaCheck, FCard CardStruct, EManaAccount AccountToCharge, int32& Cost);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool ValidateCardPlayConditions(FName CardName, AController* CallingController, ECardPlayConditions& ConditionFailure);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool HasAbility(ACard3D* Card, EAbilityType Ability, EAbilityTrigger Trigger);

	UFUNCTION(BlueprintPure)
	static void GetAllPreBuildDeckNames(TArray<FName>& Names);
	
	UFUNCTION(BlueprintPure)
	static void GetPreBuildDeck(FName Name,FPrebuildDeck& Deck);
};
