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
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static bool ValidateCardThroughFilter(FName CardName, ECardSet CardSet,TMap<ECardClass,bool>& IncludeClass, TMap<ECardType,bool>& IncludeType, TMap<ECardRarity,bool>& IncludeRarity, bool CardWithAbility, bool UnlockedOnly, bool SpecialOnly, bool IncludeAddToDeckOnly);
	
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void FilterCardArrayByName(TArray<FName>& FilterArray, EAlphabeticalSorting Method=EAlphabeticalSorting::AToZ);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void FilterCardArrayByManaCost(TArray<FName>& FilterArray, EManaCostSorting Method, int32 ManaMin, int32 ManaMax);
	
	 /** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void FilterIncludedCards(TArray<FName>& FilterArray, TMap<ECardClass,bool>& IncludeClass, TMap<ECardType,bool>& IncludeType, TMap<ECardRarity,bool>& IncludeRarity, bool CardWithAbility, bool UnlockedOnly, bool SpecialOnly, bool IncludeAddToDeckOnly);
	
	 /** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static float GetManaInDeck(TArray<FName>& Array, TArray<int32>& ReturnArray);
	
	 /** Please add a function description */
	 UFUNCTION(BlueprintCallable)
	static int32 CalculateDeckStatRarity(TArray<FName>& FilterArray, TArray<int32>& Rarity);
	
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable)
	//static void FilterCardTypes(UPARAM(ref) TArray<FName>& FilterArray, TEnumAsByte<CardType_Enum> FilterFor, UObject* __WorldContext, TArray<FName>& FilteredArray, TArray<FName> TempArray);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable)
	//static void FilterCardClass(UPARAM(ref) TArray<FName>& FilterArray, TEnumAsByte<CardClass_Enum> FilterFor, UObject* __WorldContext, TArray<FName>& FilteredArray, TArray<FName> TempArray);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable)
	//static void FilterCardRarity(UPARAM(ref) TArray<FName>& FilterArray, TEnumAsByte<CardRarity_Enum> FilterFor, UObject* __WorldContext, TArray<FName>& FilteredArray, TArray<FName> TempArray);
};
