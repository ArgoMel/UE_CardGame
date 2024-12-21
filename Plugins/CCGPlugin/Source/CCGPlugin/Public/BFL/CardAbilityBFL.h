// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CardAbilityBFL.generated.h"

class ACard3D;

UCLASS()
class CCGPLUGIN_API UCardAbilityBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Ability - Draw Card(s)")
	static void DrawCard(ACard3D* CallingCard, int32 AbilityIndex);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Ability - Increase Attack")
	static void IncreaseAttack(ACard3D* CallingCard, int32 AbilityIndex);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Ability - Clone Card")
	static void CloneCard(ACard3D* CallingCard, int32 AbilityIndex, bool RandomPlacement=false);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Ability - Increase Player Health")
	static void IncreasePlayerHealth(ACard3D* CallingCard, int32 AbilityIndex);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Ability - Deal Retaliation Damage")
	static void DealRetaliationDamage(ACard3D* CallingCard, int32 AbilityIndex);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Ability - Damage All Cards on Board")
	static void DamageAllCardsOnBoard(ACard3D* CallingCard, int32 AbilityIndex);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Ability - Spawn Random Card(s) from Deck")
	static void SpawnRandomCardFromDeck(ACard3D* CallingCard, int32 AbilityIndex);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Ability - Give Turn Points to All Active Cards")
	static void GiveTurnPointsToAllActiveCards(ACard3D* CallingCard, int32 AbilityIndex);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Ability - Increase Turn Points")
	static void IncreaseTurnPoints(ACard3D* CallingCard, int32 AbilityIndex);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Ability - Discard Random Card(s) From Hand")
	static void DiscardRandomCardFromHand(ACard3D* CallingCard, int32 AbilityIndex);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Ability - Pickup Cards from graveyard")
	static void PickupCardFromGraveyard(ACard3D* CallingCard, int32 AbilityIndex, bool AddDirectlyToBoard, ECardType CardType);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Ability - Change Played Card Owner")
	static void ChangePlayedCardOwner(ACard3D* CallingCard, int32 AbilityIndex);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable,DisplayName="Ability - Change In-Hand Card Owner")
	static void ChangeInHandCardOwner(ACard3D* CallingCard, int32 AbilityIndex);
};
