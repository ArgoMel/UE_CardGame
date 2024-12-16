// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/GameSnapshotBFL.h"

#include "CCGPlugin.h"
#include "AI/CCGAIController.h"
#include "BFL/CCGBFL.h"
#include "BFL/ControllerBFL.h"
#include "GameMode/CCGMode.h"
#include "Gameplay/Card3D.h"
#include "GameState/CCGState.h"

void UGameSnapshotBFL::GetGameStateSnapshot(const UObject* WorldContextObject, FGameStateSnapshot& ReturnSnapshot)
{
	IF_RET_VOID(WorldContextObject);
	const UWorld* world=WorldContextObject->GetWorld();
	IF_RET_VOID(world);
	ACCGMode* gameMode =Cast<ACCGMode>(world->GetAuthGameMode());
	IF_RET_VOID(gameMode);
	ACCGState* gameState =Cast<ACCGState>(world->GetGameState());
	IF_RET_VOID(gameState);
	TArray<int32> cardIDs;
	TArray<FPlayerStateSnapshot> playerSnapshots;

	for (const auto& controller:gameMode->mGameControllersArray)
	{
		TArray<FCard> cards;
		const int32 id=UControllerBFL::GetControllerID(controller);
		TArray<ACard3D*> playerCards;
		gameState->GetActivePlayerCards(id,playerCards);
		for (const auto& playerCard:playerCards)
		{
			cards.Add(playerCard->GetCardData());
			cardIDs.Add(playerCard->GetCardID());
		}
		FPlayerStat playerStat;
		TArray<FName> deck;
		TArray<FName> cardsInHand;
		UControllerBFL::GetControllersStateProfile(WorldContextObject,id,playerStat,deck,cardsInHand);
		FPlayerStateSnapshot playerSnapshot;
		playerSnapshot.PlayerID=id;
		playerSnapshot.CardArray=cards;
		playerSnapshot.CardIDArray=cardIDs;
		playerSnapshot.CardsInHand=cardsInHand;
		playerSnapshot.Deck=deck;
		playerSnapshot.Health=playerStat.Health;
		playerSnapshot.Mana=playerStat.Mana;
		playerSnapshot.Turn=playerStat.PlayerTurn;
		playerSnapshots.Add(playerSnapshot);
	}
	ReturnSnapshot.PlayerSnapshots=playerSnapshots;
}

void UGameSnapshotBFL::GenerateSimulationPlayList(const UObject* WorldContextObject, int32 ControllerID, FPlayStruct& PlayList)
{
	PlayList.CardInteractions.Empty();
	IF_RET_VOID(WorldContextObject);
	const UWorld* world=WorldContextObject->GetWorld();
	IF_RET_VOID(world);
	ACCGState* gameState =Cast<ACCGState>(world->GetGameState());
	IF_RET_VOID(gameState);
	TArray<ACard3D*> playerCards;
	gameState->GetActivePlayerCards(ControllerID,playerCards);

	for (const auto& playerCard:playerCards)
	{
		TArray<FCardInteraction> interactionArray;
		GenerateCardSnapshotSimulation(WorldContextObject,playerCard,ControllerID,interactionArray);
		PlayList.CardInteractions.Add(FPlaySubStruct(interactionArray));
	}
}

void UGameSnapshotBFL::GenerateCardSnapshotSimulation(const UObject* WorldContextObject, ACard3D* Card, int32 ControllerID, TArray<FCardInteraction>& ReturnInteractionArray)
{
	ReturnInteractionArray.Empty();
	IF_RET_VOID(WorldContextObject);
	IF_RET_VOID(Card);
	AController* controller=UControllerBFL::GetControllerReferenceFromID(WorldContextObject,ControllerID);
	IF_RET_VOID(controller);
	const ACCGAIController* AIController=Cast<ACCGAIController>(controller);
	IF_RET_VOID(AIController);

	if (Card->GetTurnPoint()<=0||
		!AIController->mValidInteractionTypes.Contains(Card->GetCardType()))
	{
		return;
	}

	const UWorld* world=controller->GetWorld();
	IF_RET_VOID(world);
	ACCGState* gameState =Cast<ACCGState>(world->GetGameState());
	IF_RET_VOID(gameState);
	
	TArray<int32> players;
	UControllerBFL::GetControllerIDs(WorldContextObject,controller,players);
	players.Remove(ControllerID);
	const int32 randOpponentID=players[FMath::RandRange(0,players.Num()-1)];
	TArray<ACard3D*> playerCards;
	gameState->GetActivePlayerCards(ControllerID,playerCards);
	TArray<ACard3D*> opponentCards;
	gameState->GetActivePlayerCards(randOpponentID,opponentCards);
	UCCGBFL::ShuffleArray(opponentCards,FMath::Rand()%9999);
}
