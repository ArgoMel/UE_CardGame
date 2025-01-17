// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlacementBFL.generated.h"

class ACardPlacement;

UCLASS()
class CCGPLUGIN_API UPlacementBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool GetPlayersAvailablePlacementsForCard(const UObject* WorldContextObject,int32 PlayerID, FName CardName, TArray<ACardPlacement*>& ReturnAvailablePlacements);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static ACardPlacement* GetClosestAvailablePlacement(ACardPlacement* Placement, FName CardName, ECardSet CardSet);
	
	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool GetIsPlacementAvailable(ACardPlacement* Placement, FName CardName);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	static bool GetIsValidPlacement(ACardPlacement* Placement, FName CardName, ECardSet CardSet);

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	static bool GetDoesPlacementSupportCardType(ACardPlacement* Placement, FName CardName);
};
