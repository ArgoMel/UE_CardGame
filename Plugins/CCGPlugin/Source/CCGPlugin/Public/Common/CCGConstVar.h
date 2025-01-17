// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CCGConstVar.generated.h"

namespace CCG_MSG
{
	const FString GameStart(TEXT("Game has Started")); 
	const FString PlayerTurn(TEXT("Your Turn!")); 
	const FString OpponentTurn(TEXT("Opponents Turn!")); 
	const FString InvalidPlacement(TEXT("Placement Not Valid")); 
	const FString NotEnoughMana(TEXT("Not Enough Mana")); 
	const FString DeckCreated(TEXT("Deck Created!")); 
}

namespace CCG_Default
{
	const FName DefaultAIPersonality(TEXT("Personality_1"));
	constexpr int32 MaxManaCost=10;
}

namespace CCG_ZOrder
{
	constexpr int32 MainMenu=5;
	constexpr int32 DisplayMessage=5;
	constexpr int32 Countdown=5;
	constexpr int32 PlayerGameUI=2;
	constexpr int32 OpponentUI=1;
	constexpr int32 AIBar=11;
}

namespace CCG_PlayerIndex
{
	constexpr int32 InvalidIndex=-1;
	constexpr int32 NeutralIndex=-2;
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
	
	const FName PlaySpeed(TEXT("PlaySpeed"));
	const FName Awareness(TEXT("Awareness"));
	const FName Difficulty(TEXT("Difficulty"));
	const FName Risk(TEXT("Risk"));
	const FName TurnState(TEXT("TurnState"));
	const FName TurnActive(TEXT("bTurnActive"));
}

UCLASS()
class CCGPLUGIN_API UCCGConstVar : public UObject
{
	GENERATED_BODY()
};
