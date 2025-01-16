// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CardsInHandInterface.generated.h"

UINTERFACE()
class UCardsInHandInterface : public UInterface
{
	GENERATED_BODY()
};

class CCGPLUGIN_API ICardsInHandInterface
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool AddCardToPlayersHand(FName CardName);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool RemoveCardFromHand(FName Card, int32 Index, bool RemoveAll);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DrawCard(FName CardName, bool IgnoreMaxCards, int32 NumberOfCardsToDraw);
};
