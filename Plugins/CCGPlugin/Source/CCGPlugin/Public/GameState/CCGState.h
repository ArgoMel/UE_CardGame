// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "GameFramework/GameStateBase.h"
#include "CCGState.generated.h"

UCLASS()
class CCGPLUGIN_API ACCGState : public AGameStateBase
{
	GENERATED_BODY()
public:
	void OnGameStart();
	void Server_NotifyEndGameState(TArray<EEndGameResults>& array);
};
