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
	const FString NoAbility(TEXT("This Card has no ability")); 
}

namespace CCG_Default
{
	const FName DefaultAIPersonality(TEXT("Personality_1"));
	constexpr int32 MaxManaCost=10;
	/** The maximum amount of characters that the 3D text renderer can hold per line. Note: This does not currently filter full words. Which may result in words being split between lines.  */
	constexpr int32 MaxDescLineLength=43;
	constexpr float CardPreviewScale=0.7f;
}

namespace CCG_ZOrder
{
	constexpr int32 MainMenu=5;
	constexpr int32 DisplayMessage=5;
	constexpr int32 Countdown=5;
	constexpr int32 CardPreview=5;
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
	const FName TurnState(TEXT("TurnState"));
	const FName TurnActive(TEXT("bTurnActive"));
}

namespace CCG_Mat_Param
{
	const FName Tint(TEXT("Tint"));
	const FName Brightness(TEXT("Brightness"));
	const FName OutlineColor(TEXT("OutlineColor"));
}

UCLASS()
class CCGPLUGIN_API UCCGConstVar : public UObject
{
	GENERATED_BODY()
};
