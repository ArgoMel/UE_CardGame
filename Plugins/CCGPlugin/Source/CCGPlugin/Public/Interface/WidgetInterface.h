// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "UObject/Interface.h"
#include "WidgetInterface.generated.h"

class UWidget;

UINTERFACE()
class UWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class CCGPLUGIN_API IWidgetInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DisplayMessage(const FString& Message, FLinearColor SpecifiedColor, double Duration);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="AIBar")
	void SetValue(FEvaluationState EvaluationState);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category="AIBar")
	void SetParent(AActor* Parent);
};
