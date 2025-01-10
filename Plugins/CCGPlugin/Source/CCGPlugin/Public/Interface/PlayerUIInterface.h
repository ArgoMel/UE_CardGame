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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Common")
	void UpdateUIPlayerStats(int32 PlayerIndex,bool ForceCleanVisualUpdate=false);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Common")
	void AddCardWidget(FName CardName,ECardSet FromCardSet);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Common")
	void RemoveCardWidget(UUserWidget* CardWidget,int32 HandIndex,bool RemoveAll=false);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Common")
	void SetCardViewState(EViewState CardViewState ,bool Force=false);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player")
	void UpdateUITurnState(bool TurnActive, EGameTurn TurnState);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player")
	EViewState GetViewState();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player")
	void LogNotificationMessage(const FString& Message, FLinearColor SpecifiedColor);
};
