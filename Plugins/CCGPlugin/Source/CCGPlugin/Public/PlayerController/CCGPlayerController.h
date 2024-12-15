// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "GameFramework/PlayerController.h"
#include "CCGPlayerController.generated.h"

class ACCGPlayerState;
class ACardPlacement;
class UDragDropOperation;
class ACCGState;
class ATargetDragSelection;
class ABoardPlayer;
class ACard3D;

UCLASS()
class CCGPLUGIN_API ACCGPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACCGPlayerController();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Game Interaction", Replicated)
	TObjectPtr<ACard3D> mHitCard;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Game Interaction")
	TObjectPtr<ABoardPlayer> mHitPlayer;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Game Interaction")
	TObjectPtr<AActor> mHitActor;
	/** The card which is sending the interaction event */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Game Interaction")
	TObjectPtr<ACard3D> mTalkingCard ;
	/** The card which is receiving the interaction event */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Game Interaction")
	TObjectPtr<ACard3D> mReceivingCard;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Game Interaction")
	TObjectPtr<ABoardPlayer> mReceivingPlayer;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Game Interaction", Replicated)
	TObjectPtr<ATargetDragSelection> mTargetDragSelectionActor;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Game Interaction", Replicated)
	bool bIsDragging;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Game Interaction", Replicated)
	bool bIsCardSelected;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Game Interaction")
	bool bValidInteraction;
	
	UPROPERTY(BlueprintReadWrite, Category="Player")
	TObjectPtr<UUserWidget> mPlayerGameUI;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Player")
	TObjectPtr<UUserWidget> mOpponentUI;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player")
	ECardPlayerState mPlayerStateEnum;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player", Replicated)
	ECardSet mCardSet;
	/** The max card the player can hold in their hand at any one time. Note: Not used when 'IgnoreMaxCards?' is enabled on the 'Request Card pickup' event. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player")
	int32 mMaxCardsInHand;
	/** The number of the cards the player will pick up when the game begins. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player")
	int32 mCardsInFirstHand;
	/** The number of cards the player can draw at the beginning of their turn.  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Player")
	int32 mCardsToDrawPerTurn;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player", ReplicatedUsing="OnRep_bTurnActive")
	bool bTurnActive;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement")
	TObjectPtr<ACard3D> mPlayCard_Server;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement")
	TObjectPtr<ACard3D> mPlayCard_Client;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement")
	TObjectPtr<ACard3D> mCardOnBoard;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement")
	TObjectPtr<ACardPlacement> mTargetCardPlacement;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement")
	TObjectPtr<ACardPlacement> mCardPlacement;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Card Placement")
	TObjectPtr<UUserWidget> mCardWidget;
	/** Distance the player will hold a card from the camera when placing on the board */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement")
	double mCardHoldDistance;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Placement")
	bool bEnableInHandMovementRotation;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement")
	bool bIsValidClientDrop;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement", Replicated)
	bool bPlayCardSuccess;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement")
	bool bIsPlayed;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Mobile")
	TObjectPtr<ACard3D> mEnabledMobilePreview;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Mobile")
	bool bEnabledMobileCardPreview;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TObjectPtr<ACCGState> mGameState;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TObjectPtr<ACCGPlayerState> mPlayerState;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	ECardError mError;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", ReplicatedUsing="OnRep_TurnState")
	EGameTurn mTurnState;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Deck")
	TArray<FName> mTempDeck;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Deck", Replicated)
	TArray<FName> mPlayerDeck;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Deck")
	int32 mWeightedFilterIndex;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Deck")
	bool bShuffleDeck;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Deck")
	bool bEnableWeightedCards;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Temp Variables")
	TObjectPtr<UDragDropOperation> tDragDropOperation;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Temp Variables")
	FName tCreateCardName;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Temp Variables", Replicated)
	FVector tLocation;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Temp Variables")
	ECardSet tChosenCardSet;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Temp Variables")
	int32 tHandIndex;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Temp Variables")
	bool bCardIsClone;

	UPROPERTY(EditDefaultsOnly, Category="Game")
	TSubclassOf<UUserWidget> mCountdownTimerClass;
	UPROPERTY(BlueprintReadWrite, Category="Game")
	TObjectPtr<UUserWidget> mCountdownTimerWidget;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Manager")
	TArray<UUserWidget*> mCardSelection;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Manager", Replicated)
	TArray<FName> mCardsInHand;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Manager", Replicated)
	FName mCardToAdd;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Manager", Replicated)
	ECardSet mCardSetRef;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Manager")
	int32 mNumberOfCardsToAdd;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement")
	int32 mCardInHandIndex;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Manager")
	double CardPickupDelay;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Developer", Replicated)
	bool bSkipManaCheck;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player", Replicated)
	TObjectPtr<ABoardPlayer> mBoardPlayer;

protected:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_bTurnActive();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_TurnState();
	
public:
	// UFUNCTION(BlueprintCallable, Category="Card Functions")
	// void SetCardLocation(A3DCard_C* Card, FVector HoldLocation, FRotator Rotation);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Card Interaction Functions")
	// void DetectCardInteraction();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Card Interaction Functions")
	// void ValidateInteractionState(bool& ValidInteraction);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Card Interaction Functions")
	// void DetectInteractionOnMove();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Card Functions")
	// void ValidateCardPlacement(AActor* HitActor, bool& ValidPlacement);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Game Setup")
	// void SetupGameUI();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Game Setup")
	// void SetupDeck(FString DeckName, UPARAM(ref) TArray<FName>& PlayerDeck);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Deck / Hand  Functions")
	// void FilterWeightedCards();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Game Setup")
	// void SetTimer(int32 Time);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Deck / Hand  Functions")
	// void ShufflePlayerDeck(const TArray<FName>& TargetArray);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Card Interaction Functions")
	// void RunCardInteraction(A3DCard_C* Interaction_TalkingCard, A3DCard_C* Interaction_ReceivingCard, ABoardPlayer_C* Interaction Receiving Player, A3DCard_C* InteractionReceivingCard, A3DCard_C* InteractionTalkingCard, ABoardPlayer_C* InteractionReceivingPlayer);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Game")
	// void NotifyCardTurnActive();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Game")
	// void NotifyCardsEndTurn();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Card Interaction Functions")
	// void CreateDragActorVisual(bool UseActorLocation?, bool& Valid);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Deck / Hand  Functions")
	// void AddCardToHand_OLD(const FName& CardToAdd, TEnumAsByte<CardSet_Enum> FromCardSet);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Deck / Hand  Functions")
	// void RemoveCardFromHand_OLD(bool ClearAll, FName CardName, int32 IndexToRemove, bool& Success);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure, Category="Deck / Hand  Functions")
	// void GetCardInHand_OLD(int32 Index, bool LastIndex, FName& Item);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure, Category="Deck / Hand  Functions")
	// int32 CountCardsInHand_OLD();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure, Category="Deck / Hand  Functions")
	// int32 CountCardsInDeck_OLD();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure, Category="Deck / Hand  Functions")
	// void HasCardInHand_OLD(FName CardName, int32 Index, bool& HasCard);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Card Functions")
	// void CreatePlaceableCard(Client)(FName Name, TEnumAsByte<CardSet_Enum> CardSet, FVector Spawn Transform Location, A3DCard_C*& Card Created, FTransform TEMP_SpawnTransform);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Card Functions")
	// void CreatePlaceableCard(Server)(FTransform SpawnTransform, A3DCard_C*& Card Created, FTransform TEMP_SpawnTransform);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Card Functions")
	// void ServerValidateCardPlacement(ACardPlacement_C* CardPlacement, FCard_Struct CardStruct, bool& ValidPlacement, ACardPlacement_C*& ValidCardPlacement);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Card Functions")
	// void SetCustomCardData(A3DCard_C* Card, bool ActiveAbility, A3DCard_C*& ReturnCard);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Card Functions")
	// void PlayCard(FName CardName, TEnumAsByte<CardSet_Enum> CardSet, ACardPlacement_C* CardPlacement, int32 CardHandIndex, FTransform SpawnTransform);

	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Game Setup")
	// void LoadClientDeck(FString& DeckName, TArray<FName>& Deck);
	
	UFUNCTION(BlueprintCallable,Server,Reliable,Category="Server")
	void Server_SetupDeck();
	UFUNCTION(BlueprintCallable,Server,Reliable,Category="Server")
	void Server_SpawnAIOpponent();
	
	UFUNCTION(BlueprintCallable,Client,Reliable,Category="Cient")
	void Client_PostLogin();
	UFUNCTION(BlueprintCallable,Client,Reliable,Category="Cient")
	void Client_SetCountdownTimer(int32 Time);
	UFUNCTION(BlueprintCallable,Client,Reliable,Category="Cient")
	void Client_EndMatchState(EEndGameResults Result);
	UFUNCTION(BlueprintCallable,Client,Reliable,Category="Cient")
	void Client_UpdateGameUI(bool ForceCleanUpdate=false);

	FORCEINLINE UUserWidget* GetPlayerGameUI(){return mPlayerGameUI;}
	FORCEINLINE TArray<FName> GetPlayerDeck(){return mPlayerDeck;}
	FORCEINLINE TArray<FName> GetCardsInHand(){return mCardsInHand;}
};
