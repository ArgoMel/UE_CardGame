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
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool SimulateAttackCards(int32 CallingPlayerID, ACard3D* CallingCard, ACard3D* ReceivingCard, int32 PtsForDamage, int32 PtsForRemovingCardFromPlay, bool IncludeCardValue, FCardInteraction& ReturnInteraction);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool SimulateAttackPlayer(int32 CallingPlayerID, ACard3D* CallingCard, int32 ReceivingPlayerID, int32 PtsForPlayerDamage, int32 PtsForRemovingPlayer, FCardInteraction& ReturnInteraction);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool SimulateAttackOwnedCard(int32 CallingPlayerID, ACard3D* CallingCard, ACard3D* ReceivingCard, int32 AttackOwnerCardDefaultValue, FCardInteraction& ReturnInteraction);
};
