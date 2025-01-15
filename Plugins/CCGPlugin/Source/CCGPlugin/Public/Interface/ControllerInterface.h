// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "UObject/Interface.h"
#include "ControllerInterface.generated.h"

class ACard3D;

UINTERFACE()
class UControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class CCGPLUGIN_API IControllerInterface
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ACard3D* CreatePlayableCard(FTransform SpawnTransform);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool AddCardToPlayersHand(FName CardName);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginPlayerTurn();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndPlayerTurn();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateUI();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Server")
	int32 CurPlayerNum();
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Save/Load")
	void SetGameModeOption(FCardGameOption Option);

	/** CardWidget 에서 사용 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player")
	UUserWidget* GetPlayerUI();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player")
	UMaterialInterface* GetProfileImg();
	/** CardWidget 에서 사용 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player")
	void CallCreateCard(FName CardName,ECardSet CardSet,int32 CardHandIndex,UUserWidget* CardWidget);
};
