// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <CCGPlugin.h>
#include "Engine/GameInstance.h"
#include "CardGameInstance.generated.h"

UCLASS()
class CCGPLUGIN_API UCardGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UCardGameInstance();
protected:
	virtual void Init() override;

protected:
	EGameState mCurGameState;
};
