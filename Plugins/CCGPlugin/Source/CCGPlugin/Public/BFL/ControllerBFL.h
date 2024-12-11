// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ControllerBFL.generated.h"

class ACCGAIPawn;
class ACCGAIController;
class ACCGPlayerState;
class ACCGPlayerController;

UCLASS()
class CCGPLUGIN_API UControllerBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
	/** Please add a function description */
	UFUNCTION(BlueprintPure,DisplayName="GetControllerID(ServerOnly)")
	static int32 GetControllerID(AController* Controller);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure,DisplayName="Get Player Controller Reference(Server Only)")
	static bool GetPlayerControllerReference(AController* Controller,ACCGPlayerController*& CardGamePlayerController, ACCGPlayerState*& CardGamePlayerState);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure,DisplayName="GetAiControllerReference(ServerOnly)")
	static bool GetAiControllerReference(AController* Controller, ACCGAIController*& CardGameAIController, ACCGAIPawn*& CardGameAIPawn);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure,DisplayName="GetControllerReferenceFromID(ServerOnly)")
	static AController* GetControllerReferenceFromID(const UObject* WorldContextObject,int32 ControllerID);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure,DisplayName="Get Controllers State Profile(Server Only)")
	static AActor* GetControllersStateProfile(const UObject* WorldContextObject,int32 ControllerID,FPlayerStat& PlayerStat,TArray<FName>& Deck,TArray<FName>& CardsInHand);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure,DisplayName="GetControllerIDForBothPlayers(ServerOnly)")
	static int32 GetControllerIDs(const UObject* WorldContextObject,AController* Controller,TArray<int32>& Players);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure,DisplayName="GetControllerPlayerState(ServerOnly)")
	static AActor* GetControllerPlayerState(const UObject* WorldContextObject,int32 ControllerID);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static bool GetReplicatedPlayerState(const UWorld* World,int32 PlayerID,FString& PlayerName,FPlayerStat& PlayerStat);
};
