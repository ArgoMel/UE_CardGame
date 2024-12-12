// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CardInteractionInterface.generated.h"

class ACard3D;

UINTERFACE()
class UCardInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class CCGPLUGIN_API ICardInteractionInterface
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TakesDamage(ACard3D* DealingCard, int32 DamageAttack, int32 DamageHealth);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnActivePlayerTurn();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEndActivePlayerTurn();
};
