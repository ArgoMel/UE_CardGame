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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCardViewState(EViewState CardViewState ,bool Force=false);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player")
	EViewState GetViewState();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player")
	void LogNotificationMessage(const FString& Message, FLinearColor SpecifiedColor);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player")
	void AddCardWidget(FName CardName,ECardSet FromCardSet);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player")
	void RemoveCardWidget(UUserWidget* CardWidget,int32 HandIndex,bool RemoveAll=false);
};
