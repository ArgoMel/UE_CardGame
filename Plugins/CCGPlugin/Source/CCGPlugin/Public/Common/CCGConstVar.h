// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CCGConstVar.generated.h"

namespace CCG
{
	constexpr int32 MaxManaCost=10;
}

namespace CCG_MSG
{
	const FString GameStart(TEXT("Game has Started")); 
	const FString PlayerTurn(TEXT("Your Turn!")); 
	const FString OpponentTurn(TEXT("Opponents Turn!")); 
	const FString InvalidPlacement(TEXT("Placement Not Valid")); 
	const FString NotEnoughMana(TEXT("Not Enough Mana")); 
}

namespace CCG_TAG
{
	const FName Camera(TEXT("CardGameCamera")); 
}

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
	const FString HappySnaps(TEXT("HappySnaps"));
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
