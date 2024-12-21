// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/ArrayFilterBFL.h"

#include "BFL/DeckBFL.h"
#include "BFL/MiscBFL.h"
#include "Common/CCGConstVar.h"

bool UArrayFilterBFL::ValidateCardThroughFilter(FName CardName, ECardSet CardSet, TMap<ECardClass, bool>& IncludeClass, TMap<ECardType, bool>& IncludeType, TMap<ECardRarity, bool>& IncludeRarity, bool CardWithAbility, bool UnlockedOnly,
                                                bool SpecialOnly, bool IncludeAddToDeckOnly)
{
	FCard card=UDeckBFL::GetCardData(CardName,CardSet);
	if (!IncludeClass.Contains(card.Class))
	{
		return false;
	}
	if (!IncludeType.Contains(card.Type))
	{
		return false;
	}
	if (!IncludeRarity.Contains(card.Rarity))
	{
		return false;
	}
	if (CardWithAbility)
	{
		if (card.Abilities.IsEmpty()
			||(card.Abilities.Num()<2&&card.Abilities[0].Type==EAbilityType::None))
		{
			return false;
		}
	}
	if (UnlockedOnly
		&&!card.DeckSetting.Unlocked)
	{
		return false;
	}
	if (IncludeAddToDeckOnly
		&&!card.DeckSetting.AddToPlayerDeck)
	{
		return false;
	}
	if (SpecialOnly
	&&!card.Special)
	{
		return false;
	}
	return true;
}

void UArrayFilterBFL::FilterCardArrayByName(TArray<FName>& FilterArray, EAlphabeticalSorting Method)
{
	const TArray<FString> Alphabet={"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
	const int32 alphabetNum=Alphabet.Num();
	TArray<FName> returnArray;
	
	int32 filterIndex=0;
	switch (Method)
	{
	case EAlphabeticalSorting::AToZ:
		while (filterIndex<alphabetNum)
		{
			++filterIndex;
			for (auto& name : FilterArray)
			{
				FString str=name.ToString();
				if (str.StartsWith(Alphabet[filterIndex]))
				{
					returnArray.Add(name);
				}
			}
		}
		break;
	case EAlphabeticalSorting::ZToA:
		filterIndex=alphabetNum;
		while (filterIndex>0)
		{
			--filterIndex;
			for (auto& name : FilterArray)
			{
				FString str=name.ToString();
				if (str.StartsWith(Alphabet[filterIndex]))
				{
					returnArray.Add(name);
				}
			}
		}
		break;
	default: ;
	}
	FilterArray=returnArray;
}

void UArrayFilterBFL::FilterCardArrayByManaCost(TArray<FName>& FilterArray, EManaCostSorting Method, int32 ManaMin, int32 ManaMax)
{
	TArray<FName> returnArray;
	int32 validCheck=0;
	switch (Method)
	{
	case EManaCostSorting::LowestToHighest:
		validCheck=FMath::Clamp(ManaMin,0,CCG::MaxManaCost);
		break;
	case EManaCostSorting::HighestToLowest:
		validCheck=FMath::Clamp(ManaMax,0,CCG::MaxManaCost);
		break;
	default: ;
	}

	int32 filterCount=FilterArray.Num();
	while (filterCount>0)
	{
		for (auto& name : FilterArray)
		{
			const FCard card=UDeckBFL::GetCardData(name, ECardSet::Empty);
			if (card.PlacementSetting.ManaCost==validCheck
				&&ManaMin<=validCheck
				&&validCheck<=ManaMax)
			{
				returnArray.Add(name);
			}
		}
		switch (Method)
		{
		case EManaCostSorting::LowestToHighest:
			++validCheck;
			break;
		case EManaCostSorting::HighestToLowest:
			--validCheck;
			break;
		default: ;
		}
		--filterCount;
	}
	FilterArray=returnArray;
}

void UArrayFilterBFL::FilterIncludedCards(TArray<FName>& FilterArray, TMap<ECardClass, bool>& IncludeClass, TMap<ECardType, bool>& IncludeType, TMap<ECardRarity, bool>& IncludeRarity, bool CardWithAbility, bool UnlockedOnly, bool SpecialOnly, bool IncludeAddToDeckOnly)
{
	TArray<FName> returnArray;
	for (auto& name : FilterArray)
	{
		if (ValidateCardThroughFilter(name,ECardSet::Empty,IncludeClass,IncludeType,IncludeRarity,CardWithAbility,UnlockedOnly,SpecialOnly,IncludeAddToDeckOnly))
		{
			returnArray.Add(name);
		}
	}
	FilterArray=returnArray;
}

float UArrayFilterBFL::GetManaInDeck(TArray<FName>& Array, TArray<int32>& ReturnArray)
{
	ReturnArray.Init(0,CCG::MaxManaCost+1);
	if (Array.IsEmpty())
	{
		return 0.f;
	}

	for (const auto& name : Array)
	{
		const FCard card=UDeckBFL::GetCardData(name, ECardSet::Empty);
		++ReturnArray[card.PlacementSetting.ManaCost];
	}

	int32 average=0.f;
	for (int32 i=0;i<=CCG::MaxManaCost;++i)
	{
		average+=ReturnArray[i]*i;
	}
	return UMiscBFL::CalculateFloatPrecision(average/ReturnArray.Num());
}

int32 UArrayFilterBFL::CalculateDeckStatRarity(TArray<FName>& FilterArray, TArray<int32>& Rarity)
{
	return 0;
}
