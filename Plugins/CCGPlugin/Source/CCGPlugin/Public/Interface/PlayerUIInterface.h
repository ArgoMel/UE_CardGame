// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "UObject/Interface.h"
#include "PlayerUIInterface.generated.h"

UINTERFACE()
class UPlayerUIInterface : public UInterface
{
	GENERATED_BODY()
};

class CCGPLUGIN_API IPlayerUIInterface
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateUIPlayerStats(bool ForceCleanVisualUpdate);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateUITurnState(bool TurnActive, EGameTurn TurnState);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateUIGameTurnTime();

};
