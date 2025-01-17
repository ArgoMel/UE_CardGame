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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Common")
	ACard3D* CreatePlayableCard(FTransform SpawnTransform);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Common")
	UMaterialInterface* GetProfileImg();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Common")
	int32 GetCurrentPlayerIndex() const;
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player")
	void BeginPlayerTurn();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player")
	void EndPlayerTurn();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player")
	void UpdateUI();
	/** CardWidget 에서 사용 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player")
	UUserWidget* GetPlayerUI();
	/** CardWidget 에서 사용 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player")
	void CallCreateCard(FName CardName,ECardSet CardSet,int32 CardHandIndex,UUserWidget* CardWidget);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="Player|Save/Load")
	void SetGameModeOption(FCardGameOption Option);
};
