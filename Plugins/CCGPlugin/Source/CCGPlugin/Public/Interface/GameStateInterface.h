// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "UObject/Interface.h"
#include "GameStateInterface.generated.h"

UINTERFACE()
class UGameStateInterface : public UInterface
{
	GENERATED_BODY()
};

class CCGPLUGIN_API IGameStateInterface
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Common")
	void MatchEnd(EEndGameResults Result);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Common")
	void MatchBegin();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Common")
	void ChangeActivePlayerTurn(bool TurnActive);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="AI")
	void RequestChangePlayerTurn();


};
