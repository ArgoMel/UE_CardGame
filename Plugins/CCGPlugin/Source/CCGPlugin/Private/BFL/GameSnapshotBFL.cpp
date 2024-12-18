// Fill out your copyright notice in the Description page of Project Settings.

#include "BFL/GameSnapshotBFL.h"

#include "CCGPlugin.h"
#include "AI/CCGAIController.h"
#include "AI/EvaluationRuleBFL.h"
#include "BFL/CCGBFL.h"
#include "BFL/ControllerBFL.h"
#include "GameMode/CCGMode.h"
#include "Gameplay/Card3D.h"
#include "GameState/CCGState.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "Gameplay/BoardPlayer.h"
#include "Gameplay/CardPlacement.h"

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
			cards.Add(*playerCard->GetCardData());
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
	UControllerBFL::GetControllerIDs(controller,players);
	players.Remove(ControllerID);
	const int32 randOpponentID=players[FMath::RandRange(0,players.Num()-1)];
	TArray<ACard3D*> playerCards;
	gameState->GetActivePlayerCards(ControllerID,playerCards);

	//Simulate - Attack Cards
	TArray<ACard3D*> opponentCards;
	gameState->GetActivePlayerCards(randOpponentID,opponentCards);
	UCCGBFL::ShuffleArray(opponentCards,FMath::Rand()%9999);
	for (const auto& opponentCard:opponentCards)
	{
		FCardInteraction cardInteraction;
		int32 index=0;
		bool canSimulate=false;
		for (const auto& interaction:ReturnInteractionArray)
		{
			canSimulate=SimulateAttackCards(ControllerID,Card,opponentCard,AIController->GetAIPointAllocation().Calculation.CardDamaged,AIController->GetAIPointAllocation().Calculation.CardRemovedFromPlay,
				AIController->GetAIPointAllocation().Calculation.bIncludeAIPointPerCard,cardInteraction);
			if (canSimulate&&
				cardInteraction.Value>interaction.Value)
			{
				ReturnInteractionArray.Insert(cardInteraction,index);
				break;
			}
			++index;
		}
		if (canSimulate&&
			index>=ReturnInteractionArray.Num()-1)
		{
			ReturnInteractionArray.Add(cardInteraction);
		}
	}

	//Simulate - Attack Opponent Player
	FCardInteraction cardInteraction;
	int32 index=0;
	bool canSimulate=false;
	for (const auto& interaction:ReturnInteractionArray)
	{
		canSimulate=SimulateAttackPlayer(ControllerID,Card,randOpponentID,AIController->GetAIPointAllocation().Calculation.PerDamagePointToPlayer,AIController->GetAIPointAllocation().Calculation.PlayerRemovedFromPlay,cardInteraction);
		if (canSimulate&&
			cardInteraction.Value>interaction.Value)
		{
			ReturnInteractionArray.Insert(cardInteraction,index);
			break;
		}
		++index;
	}
	if (canSimulate&&
		index>=ReturnInteractionArray.Num()-1)
	{
		ReturnInteractionArray.Add(cardInteraction);
	}
	
	//Find valid own card which can receive attack
	//Simulate - Attack Cards
	for (const auto& playerCard:playerCards)
	{
		if (playerCard->GetPlacementOwner()->GetPlayerIndex()==ControllerID&&
			playerCard!=Card&&
			(playerCard->GetCardData()->CardSystemData.InteractionConditions.Contains(EInteractionConditions::CanReceiveSelfInteraction_AIOnly)||playerCard->GetCardData()->CardSystemData.InteractionConditions.Contains(EInteractionConditions::CanReceiveSelfInteractionIfDoesNotRemoveCardFromPlay_AIOnly)))
		{
			cardInteraction=FCardInteraction();
			index=0;
			canSimulate=false;
			for (const auto& interaction:ReturnInteractionArray)
			{
				canSimulate=SimulateAttackOwnedCard(ControllerID,Card,playerCard,AIController->GetAIPointAllocation().Calculation.AttackOwnedCardWithValue,cardInteraction);
				if (canSimulate&&
					cardInteraction.Value>interaction.Value)
				{
					ReturnInteractionArray.Insert(cardInteraction,index);
					break;
				}
				++index;
			}
			if (canSimulate&&
				index>=ReturnInteractionArray.Num()-1)
			{
				ReturnInteractionArray.Add(cardInteraction);
			}
		}
	}
}

bool UGameSnapshotBFL::SimulateAttackCards(int32 CallingPlayerID, ACard3D* CallingCard, ACard3D* ReceivingCard, int32 PtsForDamage, int32 PtsForRemovingCardFromPlay, bool IncludeCardValue, FCardInteraction& ReturnInteraction)
{
	IF_RET_BOOL(CallingCard);
	IF_RET_BOOL(ReceivingCard);
	FPlayerStat playerStat;
	UControllerBFL::GetControllersStateStat(CallingCard,CallingPlayerID,playerStat);
	if (!CallingCard->GetCardData()->Attack.CanAttackCards||
		ReceivingCard->GetCardData()->Attack.ManaCost>playerStat.Mana)
	{
		return false;
	}

	const int32 add=UEvaluationRuleBFL::RuleCalculateAdditionalAbilityPointOffset(CallingCard,ReceivingCard);
	int32 ruleTotal=0;
	const int32 total=UEvaluationRuleBFL::RuleCalculateAttackPoints(add,PtsForDamage,PtsForRemovingCardFromPlay,CallingCard->GetCardData()->Attack.Damage,ReceivingCard->GetCardData()->Health.Health,ReceivingCard->GetCardData()->CardSystemData.CardValue,IncludeCardValue,ruleTotal);

	ReturnInteraction.OwningControllerID=CallingPlayerID;
	ReturnInteraction.InteractionType=EPlayType::AttackCard;
	ReturnInteraction.TalkingCard=CallingCard;
	ReturnInteraction.ReceivingActor=ReceivingCard;
	ReturnInteraction.Value=total;
	return true;
}

bool UGameSnapshotBFL::SimulateAttackPlayer(int32 CallingPlayerID, ACard3D* CallingCard, int32 ReceivingPlayerID, int32 PtsForPlayerDamage, int32 PtsForRemovingPlayer, FCardInteraction& ReturnInteraction)
{
	IF_RET_BOOL(CallingCard);
	if (!CallingCard->GetCardData()->Attack.CanAttackCards)
	{
		return false;
	}
	
	FPlayerStat playerStat;
	UControllerBFL::GetControllersStateStat(CallingCard,CallingPlayerID,playerStat);
	if (CallingCard->GetCardData()->Attack.ManaCost>playerStat.Mana)
	{
		return false;
	}

	const ACCGAIController* AIController= Cast<ACCGAIController>(UControllerBFL::GetControllerReferenceFromID(CallingCard,CallingPlayerID));
	IF_RET_BOOL(AIController);
	const int32 add=AIController->mPriorityFocusList.Contains(EAIPersonalityFocus::DamageOpponentPlayer);
	UControllerBFL::GetControllersStateStat(CallingCard,ReceivingPlayerID,playerStat);
	int32 ruleTotal=0;
	const int32 total=UEvaluationRuleBFL::RuleCalculateDamageToPlayerPoints(add,PtsForPlayerDamage,PtsForRemovingPlayer,CallingCard->GetCardData()->Attack.Damage,playerStat.Health,ruleTotal);

	const UWorld* world=CallingCard->GetWorld();
	IF_RET_BOOL(world);
	ACCGMode* gameMode=Cast<ACCGMode>(world->GetAuthGameMode());
	IF_RET_BOOL(gameMode);
	ReturnInteraction.OwningControllerID=CallingPlayerID;
	ReturnInteraction.InteractionType=EPlayType::AttackPlayer;
	ReturnInteraction.TalkingCard=CallingCard;
	ReturnInteraction.ReceivingActor=gameMode->mBoardPlayersArray[ReceivingPlayerID-1];
	ReturnInteraction.Value=total;
	return true;
}

bool UGameSnapshotBFL::SimulateAttackOwnedCard(int32 CallingPlayerID, ACard3D* CallingCard, ACard3D* ReceivingCard, int32 AttackOwnerCardDefaultValue, FCardInteraction& ReturnInteraction)
{
	IF_RET_BOOL(CallingCard);
	IF_RET_BOOL(ReceivingCard);
	if (!CallingCard->GetCardData()->Attack.CanAttackCards)
	{
		return false;
	}

	FPlayerStat playerStat;
	UControllerBFL::GetControllersStateStat(CallingCard,CallingPlayerID,playerStat);
	if (ReceivingCard->GetCardData()->Attack.ManaCost>playerStat.Mana)
	{
		return false;
	}

	int32 point=0;
	if (!UEvaluationRuleBFL::RuleCalculateOwnedCardAbilityPointOffset(CallingCard,ReceivingCard,point))
	{
		return false;
	}

	ReturnInteraction.OwningControllerID=CallingPlayerID;
	ReturnInteraction.InteractionType=EPlayType::AttackCard;
	ReturnInteraction.TalkingCard=CallingCard;
	ReturnInteraction.ReceivingActor=ReceivingCard;
	ReturnInteraction.Value=point+AttackOwnerCardDefaultValue;
	return true;
}
