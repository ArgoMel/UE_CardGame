// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/PlacementBFL.h"

#include "CCGPlugin.h"
#include "BFL/DeckBFL.h"
#include "Gameplay/CardPlacement.h"
#include "GameState/CCGState.h"

bool UPlacementBFL::GetPlayersAvailablePlacementsForCard(const UObject* WorldContextObject,int32 PlayerID, FName CardName, TArray<ACardPlacement*>& ReturnAvailablePlacements)
{
	IF_RET_BOOL(WorldContextObject);
	const UWorld* world=WorldContextObject->GetWorld();
	IF_RET_BOOL(world);
	ACCGState* gameState=Cast<ACCGState>(world->GetGameState());
	IF_RET_BOOL(gameState);
	TArray<ACardPlacement*> tempPlacements;
	gameState->GetCardPlacements(PlayerID,tempPlacements);
	for (auto& tempPlacement:tempPlacements)
	{
		if (GetIsPlacementAvailable(tempPlacement,CardName))
		{
			ReturnAvailablePlacements.Add(tempPlacement);
		}
	}
	return !ReturnAvailablePlacements.IsEmpty();
}

ACardPlacement* UPlacementBFL::GetClosestAvailablePlacement(ACardPlacement* Placement, FName CardName, ECardSet CardSet)
{
	if (GetIsValidPlacement(Placement,CardName,CardSet))
	{
		return Placement;
	}

	ACardPlacement* leftPlacement=Placement->GetPlacementLeft();
	ACardPlacement* rightPlacement=Placement->GetPlacementRight();
	while (leftPlacement || rightPlacement)
	{
		if (leftPlacement)
		{
			if (GetIsValidPlacement(leftPlacement,CardName,CardSet))
			{
				return leftPlacement;
			}
			leftPlacement=leftPlacement->GetPlacementLeft();
		}

		if (rightPlacement)
		{
			if (GetIsValidPlacement(rightPlacement,CardName,CardSet))
			{
				return rightPlacement;
			}
			rightPlacement=rightPlacement->GetPlacementLeft();
		}
	}
	return nullptr;
}

bool UPlacementBFL::GetIsPlacementAvailable(ACardPlacement* Placement, FName CardName)
{
	if (!Placement)
	{
		return false;
	}
	return !Placement->IsFull()&&
		GetDoesPlacementSupportCardType(Placement, CardName);
}

bool UPlacementBFL::GetIsValidPlacement(ACardPlacement* Placement, FName CardName, ECardSet CardSet)
{
	if (!Placement)
	{
		return false;
	}
	const FCard card=UDeckBFL::GetCardData(CardName,CardSet);
	return Placement->mValidCardTypes.Contains(card.Type)&&
		!Placement->IsFull();
}

bool UPlacementBFL::GetDoesPlacementSupportCardType(ACardPlacement* Placement, FName CardName)
{
	const FCard card=UDeckBFL::GetCardData(CardName,ECardSet::Empty);
	return Placement->mValidCardTypes.Contains(card.Type);
}
