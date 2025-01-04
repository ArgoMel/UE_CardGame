// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SaveGameBFL.generated.h"

class USaveGame;

UCLASS()
class CCGPLUGIN_API USaveGameBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static void RemoveSaveGameSlot(FString SaveGame);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Create/LoadCardGameSave")
	static bool CreateOrLoadCardGameSave(FString SlotName, TSubclassOf<USaveGame> SaveGameClass, USaveGame*& SaveGame);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static bool SaveCustomDeck(const FText& InText, UPARAM(ref) TArray<FName>& CustomDeck, bool bEditable);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static bool LoadCustomDeck(FString DeckName, TArray<FName>& CustomDeck, bool& bDeckEditable);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static bool GetRandomDeck(FString& RandomDeck);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool GetDeckValid(FString SlotName);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool GetDeckEditable(FString SlotName);
};
