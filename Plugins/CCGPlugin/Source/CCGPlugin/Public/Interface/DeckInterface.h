// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DeckInterface.generated.h"

UINTERFACE()
class UDeckInterface : public UInterface
{
	GENERATED_BODY()
};

class CCGPLUGIN_API IDeckInterface
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetPlayerDeck(TArray<FName>& Deck);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool RemoveCardFromDeck(bool RemoveAll, int32 IndexToRemove);
};
