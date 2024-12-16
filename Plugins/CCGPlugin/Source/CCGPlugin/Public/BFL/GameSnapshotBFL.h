// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameSnapshotBFL.generated.h"

UCLASS()
class CCGPLUGIN_API UGameSnapshotBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static void GetGameStateSnapshot(const UObject* WorldContextObject, FGameStateSnapshot& ReturnSnapshot);
	
	 /** Please add a function description */
	 UFUNCTION(BlueprintCallable)
	static void GenerateSimulationPlayList(const UObject* WorldContextObject,int32 ControllerID, FPlayStruct& PlayList);
	
	 /** Please add a function description */
	 UFUNCTION(BlueprintCallable)
	static void GenerateCardSnapshotSimulation(const UObject* WorldContextObject,ACard3D* Card, int32 ControllerID, TArray<FCardInteraction>& ReturnInteractionArray);
	
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure)
	//static void SimulateAttackCards(int32 CallingPlayerID, A3DCard_C* CallingCard, A3DCard_C* ReceivingCard, int32 PtsForDamage, int32 PtsForRemovingCardFromPlay, bool IncludeCardValue, UObject* __WorldContext, FCardInteraction_Struct& ReturnInteraction, bool& RemovedFromPlay, bool& Valid);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure)
	//static void SimulateAttackPlayer(int32 CallingPlayerID, A3DCard_C* CallingCard, int32 ReceivingPlayerID, int32 PtsForPlayerDamage, int32 PtsForRemovingPlayer, UObject* __WorldContext, FCardInteraction_Struct& ReturnInteraction, bool& RemovesPlayer, bool& Valid);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure)
	//static void SimulateAttackOwnedCard(int32 CallingPlayerID, A3DCard_C* CallingCard, A3DCard_C* ReceivingCard, int32 AttackOwnCardDefaultValue, UObject* __WorldContext, FCardInteraction_Struct& ReturnInteraction, bool& Valid);
};
