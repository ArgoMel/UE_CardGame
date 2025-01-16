// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "Common/CCGStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DeckBFL.generated.h"

UCLASS()
class CCGPLUGIN_API UDeckBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UDeckBFL();
	
private:
	static TObjectPtr<UDataTable> mBasicSet;
	static TObjectPtr<UDataTable> mDebugSet;
	
public:
		/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void GetChosenDeckArray(int32 Index, const TArray<UDataTable*>& TargetArray, TArray<FName>& OutRowNames);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static FCard GetCardData(FName CardName, ECardSet CardSet);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static int32 GetRandomCardFromDeck(AController* Controller, FName& Card);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static FName GetCardFromDeck(TArray<FName>& Deck, int32 Index, bool LastIndex=false);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static int32 FindCardInArray(FName SearchForCard,UPARAM(ref) TArray<FName>& CardArray, TArray<int32>& IndexArray);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void GetAllCardsInActiveSet(ECardSet CardSet, TArray<FName>& ReturnedCardSet);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void RemoveCardFromDeck(int32 IndexToRemove, TArray<FName>& Deck,bool RemoveAll=false);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void FilterWeightedCardsInDeck(TArray<FName>& CardsInDeck);
};
