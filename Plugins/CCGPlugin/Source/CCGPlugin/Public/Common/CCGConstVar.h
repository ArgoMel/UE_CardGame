// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CCGConstVar.generated.h"

namespace CCG_ZOrder
{
	constexpr int32 MainMenu=5;
	constexpr int32 DisplayMessage=5;
}

namespace CCG_Level
{
	const FName MainMenu(TEXT("MainMenu")); 
}

namespace CCG_MatchType
{
	const FString Default(TEXT("Default")); 
}

namespace CCG_SaveSlotName
{
	const FString CardGameSave(TEXT("CardGameSave"));
}

namespace CCG_BB
{
	const FName Loop(TEXT("Loop"));
	const FName FinalLoop(TEXT("bFinalLoop"));
}

UCLASS()
class CCGPLUGIN_API UCCGConstVar : public UObject
{
	GENERATED_BODY()
};
