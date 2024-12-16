// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CardsInHandBFL.generated.h"

enum class EAIPersonalityFocus : uint8;

UCLASS()
class CCGPLUGIN_API UCardsInHandBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static int32 CountCardsInHand(TArray<FName>& CardsInHand);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static FName GetCardInHand(TArray<FName>& CardsInHand, int32 Index, bool LastIndex=false);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool HasCardInHand(TArray<FName>& CardsInHand, FName Card, int32 Index);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static void FindPlayableCardsInHand(AController* Controller,TArray<FName>& PlayableCardsInHand);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static void SortCardsByFocus(const TArray<FName>& CardsArray, TArray<EAIPersonalityFocus>& FocusArray, bool IncludeOrderByValue, TArray<FName>& ReturnCardArray);
};
