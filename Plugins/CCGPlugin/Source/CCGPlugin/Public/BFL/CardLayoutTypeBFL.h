// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Slate/WidgetTransform.h"
#include "CardLayoutTypeBFL.generated.h"

UCLASS()
class CCGPLUGIN_API UCardLayoutTypeBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static FWidgetTransform Fanned(int32 CardLoopIndex,int32 CardsInHand,FCardLayout Cal);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static FWidgetTransform Horizontal(int32 CardLoopIndex,int32 CardsInHand,FCardLayout Cal);
};
