// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CCGPlayerController.generated.h"

UCLASS()
class CCGPLUGIN_API ACCGPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void Server_SetupDeck();
	
	UFUNCTION(BlueprintCallable,Client,Reliable)
	void Client_PostLogin();
	void SetCountdownTimer(int32 Time);
};
