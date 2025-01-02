// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ArrayFilterBFL.generated.h"

UCLASS()
class CCGPLUGIN_API UArrayFilterBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
private:
	static bool ValidateCardThroughFilter(FName CardName, ECardSet CardSet,TMap<ECardClass,bool>& IncludeClass, TMap<ECardType,bool>& IncludeType, TMap<ECardRarity,bool>& IncludeRarity, bool CardWithAbility, bool UnlockedOnly, bool SpecialOnly, bool IncludeAddToDeckOnly);
	
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void FilterCardArrayByName(UPARAM(ref) TArray<FName>& FilterArray, TArray<FName>& ReturnArray, EAlphabeticalSorting Method=EAlphabeticalSorting::AToZ);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void FilterCardArrayByManaCost(TArray<FName>& FilterArray, EManaCostSorting Method, int32 ManaMin, int32 ManaMax);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void FilterIncludedCards(UPARAM(ref) TArray<FName>& FilterArray, UPARAM(ref) TMap<ECardClass,bool>& IncludeClass, UPARAM(ref) TMap<ECardType,bool>& IncludeType, UPARAM(ref) TMap<ECardRarity,bool>& IncludeRarity, bool CardWithAbility, bool UnlockedOnly, bool SpecialOnly, bool IncludeAddToDeckOnly,TArray<FName>& ReturnArray);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static float GetManaInDeck(UPARAM(ref) const TArray<FName>& Array, TArray<int32>& ReturnArray);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static int32 CalculateDeckStatMana(UPARAM(ref) TArray<FName>& FilterArray, TArray<int32>& ManaStats);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static int32 CalculateDeckStatUnlockedCard(UPARAM(ref) TArray<FName>& FilterArray, TArray<FName>& UnlockedCards);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static int32 CalculateDeckStatRarity(UPARAM(ref) TArray<FName>& FilterArray, TArray<int32>& Rarity);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void FilterCardTypes(TArray<FName>& FilterArray, ECardType FilterFor);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void FilterCardClass(TArray<FName>& FilterArray, ECardClass FilterFor);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void FilterCardRarity(TArray<FName>& FilterArray, ECardRarity FilterFor);
};
