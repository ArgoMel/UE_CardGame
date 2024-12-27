// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CardWidgetInterface.generated.h"

UINTERFACE()
class UCardWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class CCGPLUGIN_API ICardWidgetInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetDefaults();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector2D GetInitialMousePos();
};
