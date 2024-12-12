// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerStateInterface.generated.h"

UINTERFACE()
class UPlayerStateInterface : public UInterface
{
	GENERATED_BODY()
};

class CCGPLUGIN_API IPlayerStateInterface
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 IncreasePlayerHealth(int32 IncreaseBy);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 DecreasePlayerHealth(int32 DecreaseBy);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 IncreasePlayerMana(int32 IncreaseBy);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 DecreasePlayerMana(int32 DecreaseBy);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetManaForTurn();
};
