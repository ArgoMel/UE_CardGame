// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/CardsInHandBFL.h"

#include "CCGPlugin.h"
#include "BFL/CCGBFL.h"
#include "BFL/ControllerBFL.h"
#include "BFL/DeckBFL.h"

int32 UCardsInHandBFL::CountCardsInHand(TArray<FName>& CardsInHand)
{
	return CardsInHand.Num();
}

FName UCardsInHandBFL::GetCardInHand(TArray<FName>& CardsInHand, int32 Index, bool LastIndex)
{
	if (LastIndex)
	{
		return CardsInHand.Last();
	}
	return CardsInHand[Index];
}

bool UCardsInHandBFL::HasCardInHand(TArray<FName>& CardsInHand, FName Card, int32 Index)
{
	return CardsInHand[Index]==Card;
}

void UCardsInHandBFL::FindPlayableCardsInHand(AController* Controller, TArray<FName>& PlayableCardsInHand)
{
	IF_RET_VOID(Controller);
	PlayableCardsInHand.Empty();
	const int32 playerID=UControllerBFL::GetControllerID(Controller);
	FPlayerStat playerStat;
	TArray<FName> deck;
	TArray<FName> cardsInHand;
	UControllerBFL::GetControllersStateProfile(Controller,playerID,playerStat,deck,cardsInHand);

	for (auto& cardInHand : cardsInHand)
	{
		const FCard card= UDeckBFL::GetCardData(cardInHand,ECardSet::Empty);
		if (card.PlacementSetting.ManaCost<=playerStat.Mana)
		{
			PlayableCardsInHand.Add(cardInHand);
		}
	}
}

void UCardsInHandBFL::SortCardsByFocus(const TArray<FName>& CardsArray, TArray<EAIPersonalityFocus>& FocusArray, bool IncludeOrderByValue, TArray<FName>& ReturnCardArray)
{
	ReturnCardArray.Empty();
	TArray<FName> tempCardArray=CardsArray;
	UCCGBFL::ShuffleArray(tempCardArray,FMath::Rand()%9999);

	for (auto& focus : FocusArray)
	{
		TArray<FName> tempFocusCardArray;
		for (auto& tempCardName : tempCardArray)
		{
			const FCard card= UDeckBFL::GetCardData(tempCardName,ECardSet::Empty);
			if (!card.CardSystemData.CardPurposes.Contains(focus))
			{
				continue;
			}

			//Order by Card Value
			if (IncludeOrderByValue)
			{
				int32 index=0;
				for (const auto& focusCardName : tempFocusCardArray)
				{
					const FCard focusCard= UDeckBFL::GetCardData(focusCardName,ECardSet::Empty);
					if (card.CardSystemData.CardValue>focusCard.CardSystemData.CardValue)
					{
						tempFocusCardArray.Insert(tempCardName,index);
						break;
					}
					++index;
				}
				if (tempFocusCardArray.Num()-1<=index)
				{
					tempFocusCardArray.Add(tempCardName);
				}
			}
			else
			{
				ReturnCardArray.Emplace(tempCardName);
			}
		}
		ReturnCardArray.Append(tempFocusCardArray);
	}
}
