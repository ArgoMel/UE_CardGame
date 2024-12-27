// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TimerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UTimerInterface : public UInterface
{
	GENERATED_BODY()
};

class CCGPLUGIN_API ITimerInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetTime(int32 Time);
};
