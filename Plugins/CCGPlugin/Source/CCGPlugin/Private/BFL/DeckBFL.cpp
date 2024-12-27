// Fill out your copyright notice in the Description page of Project Settings.


#include "BFL/DeckBFL.h"

#include "CCGPlugin.h"
#include "Interface/DeckInterface.h"
#include "Kismet/DataTableFunctionLibrary.h"

TObjectPtr<UDataTable> UDeckBFL::mBasicSet;
TObjectPtr<UDataTable> UDeckBFL::mDebugSet;

UDeckBFL::UDeckBFL()
{
	GetObjectAsset(mBasicSet,UDataTable,"/CCGPlugin/CCGToolkit/CardGame/CardSets/Basic_Set/BasicSet_DataTable.BasicSet_DataTable");
	GetObjectAsset(mDebugSet,UDataTable,"/CCGPlugin/CCGToolkit/CardGame/CardSets/Debug_Set/DebugSet_DataTable.DebugSet_DataTable");
}

void UDeckBFL::GetChosenDeckArray(int32 Index, const TArray<UDataTable*>& TargetArray, TArray<FName>& OutRowNames)
{
	UDataTableFunctionLibrary::GetDataTableRowNames(TargetArray[Index], OutRowNames);
}

FCard UDeckBFL::GetCardData(FName CardName, ECardSet CardSet)
{
	FCard card;
	switch (CardSet)
	{
	case ECardSet::Empty:
	case ECardSet::BasicSet:
		UDataTableFunctionLibrary::GetDataTableRowFromName(mBasicSet,CardName,card);
		break;
	case ECardSet::DebugCardSet:
		UDataTableFunctionLibrary::GetDataTableRowFromName(mDebugSet,CardName,card);
		break;
	default:
		break;
	}
	return card;
}

int32 UDeckBFL::GetRandomCardFromDeck(AController* Controller, FName& Card)
{
	if (!Controller->Implements<UDeckInterface>())
	{
		return -1;
	}
	TArray<FName> deck;
	IDeckInterface::Execute_GetPlayerDeck(Controller,deck);
	const int32 randIndex=FMath::RandRange(0,deck.Num()-1);
	Card=deck[randIndex];
	return randIndex;
}

FName UDeckBFL::GetCardFromDeck(TArray<FName>& Deck, int32 Index, bool LastIndex)
{
	if (LastIndex)
	{
		return Deck.Last();
	}
	return Deck[Index];
}

int32 UDeckBFL::FindCardInArray(FName SearchForCard, TArray<FName>& CardArray, TArray<int32>& IndexArray)
{
	IndexArray.Empty();
	int32 numberFound=0;
	int32 index=0;
	for (const auto& card : CardArray)
	{
		if (SearchForCard == card)
		{
			IndexArray.Add(index);
			++numberFound;
		}
		++index;
	}
	return numberFound;
}

void UDeckBFL::GetAllCardsInActiveSet(ECardSet CardSet, TArray<FName>& ReturnedCardSet)
{
	switch (CardSet)
	{
	case ECardSet::Empty:
		{
			TArray<FName> debugCardSets;
			UDataTableFunctionLibrary::GetDataTableRowNames(mBasicSet,ReturnedCardSet);
			UDataTableFunctionLibrary::GetDataTableRowNames(mDebugSet,debugCardSets);
			ReturnedCardSet.Append(debugCardSets);
		}
		break;
	case ECardSet::BasicSet:
		UDataTableFunctionLibrary::GetDataTableRowNames(mBasicSet,ReturnedCardSet);
		break;
	case ECardSet::DebugCardSet:
		UDataTableFunctionLibrary::GetDataTableRowNames(mDebugSet,ReturnedCardSet);
		break;
	default: ;
	}
}

void UDeckBFL::RemoveCardFromDeck(int32 IndexToRemove, TArray<FName>& Deck,bool RemoveAll)
{
	if (RemoveAll)
	{
		Deck.Empty();
	}
	else
	{
		Deck.RemoveAt(IndexToRemove);
	}
}

int32 UDeckBFL::CountCardsInDeck(TArray<FName>& Deck)
{
	return Deck.Num();
}

void UDeckBFL::FilterWeightedCardsInDeck(TArray<FName>& CardsInDeck)
{
	TArray<FName> tempDeck=CardsInDeck;
	TArray<FName> returnPlayerDeck;
	returnPlayerDeck.Init(FName(),tempDeck.Num());
	int32 weightedFilterIndex;

	int32 index=0;
	for (const auto& cardName : tempDeck)
	{
		const FCard card= GetCardData(cardName,ECardSet::Empty);
		if (card.DeckSetting.Weight>=2)
		{
			if (card.DeckSetting.Weight+index>=returnPlayerDeck.Num())
			{
				weightedFilterIndex=index;
			}
			else
			{
				weightedFilterIndex=card.DeckSetting.Weight+index;
			}
			while (!returnPlayerDeck[weightedFilterIndex].IsNone())
			{
				if (weightedFilterIndex+1>=returnPlayerDeck.Num())
				{
					weightedFilterIndex=1;
				}
				else
				{
					++weightedFilterIndex;
				}
			}
			returnPlayerDeck[weightedFilterIndex]=cardName;
		}
		++index;
	}

	for (const auto& cardName : returnPlayerDeck)
	{
		tempDeck.Remove(cardName);
	}

	index=0;
	for (const auto& cardName : returnPlayerDeck)
	{
		if (cardName.IsNone()&&
			!tempDeck.IsEmpty())
		{
			returnPlayerDeck[index]=tempDeck.Last();
			tempDeck.Pop();
		}
		++index;	
	}
	CardsInDeck=returnPlayerDeck;
}
