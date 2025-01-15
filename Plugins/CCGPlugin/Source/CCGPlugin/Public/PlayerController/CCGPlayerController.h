// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "GameFramework/PlayerController.h"
#include "Interface/CardsInHandInterface.h"
#include "Interface/ControllerInterface.h"
#include "Interface/DeckInterface.h"
#include "Interface/GameStateInterface.h"
#include "CCGPlayerController.generated.h"

class UComboBox;
class ACCGMode;
class ACCGPlayerState;
class ACardPlacement;
class UDragDropOperation;
class ACCGState;
class ATargetDragSelection;
class ABoardPlayer;
class ACard3D;

UCLASS()
class CCGPLUGIN_API ACCGPlayerController : public APlayerController
	,public ICardsInHandInterface
	,public IControllerInterface
	,public IDeckInterface
	,public IGameStateInterface
{
	GENERATED_BODY()
public:
	friend ACCGMode;
	ACCGPlayerController();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
public:
	/** ICardsInHandInterface */
	virtual bool RemoveCardFromHand_Implementation(FName Card, int32 Index, bool RemoveAll) override;
	virtual void DrawCard_Implementation(FName CardName, bool IgnoreMaxCards, int32 NumberOfCardsToDraw) override;

	/** IControllerInterface */
	virtual ACard3D* CreatePlayableCard_Implementation(FTransform SpawnTransform) override;
	virtual bool AddCardToPlayersHand_Implementation(FName CardName) override;
	virtual int32 CurPlayerNum_Implementation() override;
	virtual void SetGameModeOption_Implementation(FCardGameOption Option) override;
	virtual UUserWidget* GetPlayerUI_Implementation() override;
	virtual UMaterialInterface* GetProfileImg_Implementation() override;
	virtual void CallCreateCard_Implementation(FName CardName,ECardSet CardSet,int32 CardHandIndex,UUserWidget* CardWidget) override;

	/** IDeckInterface */
	virtual void GetPlayerDeck_Implementation(TArray<FName>& Deck) override;
	virtual bool RemoveCardFromDeck_Implementation(bool RemoveAll, int32 IndexToRemove) override;
	
	/** IGameStateInterface */
	virtual void MatchEnd_Implementation(EEndGameResults Result) override;
	virtual void MatchBegin_Implementation() override;
	virtual void ChangeActivePlayerTurn_Implementation(bool TurnActive) override;

private:
	FTimerHandle mAddCardTH;
	bool bCardPlayerStateDirty;

protected:
	UPROPERTY(EditDefaultsOnly,Category="Asset")
	TSubclassOf<ACard3D> mCard3DClass;
	UPROPERTY(EditDefaultsOnly,Category="Asset")
	TSubclassOf<ATargetDragSelection> mDragSelectionClass;
	UPROPERTY(EditDefaultsOnly,Category="Asset")
	TSubclassOf<UUserWidget> mPlayerGameUIClass;
	UPROPERTY(EditDefaultsOnly,Category="Asset")
	TSubclassOf<UUserWidget> mOpponentUIClass;
	UPROPERTY(EditDefaultsOnly,Category="Asset")
	TSubclassOf<UUserWidget> mCountdownWidgetClass;
	UPROPERTY(EditDefaultsOnly,Category="Asset")
	TObjectPtr<UParticleSystem> mWarningParticle;
	
	UPROPERTY(BlueprintReadWrite, Category="Game Interaction", Replicated)
	TObjectPtr<ACard3D> mHitCard;
	/** The card which is sending the interaction event */
	UPROPERTY(BlueprintReadWrite, Category="Game Interaction")
	TObjectPtr<ACard3D> mTalkingCard ;
	/** The card which is receiving the interaction event */
	UPROPERTY(BlueprintReadWrite, Category="Game Interaction")
	TObjectPtr<ACard3D> mReceivingCard;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Game Interaction")
	TObjectPtr<ABoardPlayer> mReceivingPlayer;
	UPROPERTY(BlueprintReadWrite, Category="Game Interaction", Replicated)
	TObjectPtr<ATargetDragSelection> mTargetDragSelectionActor;
	UPROPERTY(BlueprintReadWrite, Category="Game Interaction", Replicated)
	bool bIsDragging;
	UPROPERTY(BlueprintReadWrite, Category="Game Interaction", Replicated)
	bool bIsCardSelected;
	
	UPROPERTY(BlueprintReadWrite, Category="UI")
	TObjectPtr<UUserWidget> mPlayerGameUI;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="UI")
	TObjectPtr<UUserWidget> mOpponentUI;
	UPROPERTY(BlueprintReadWrite, Category="UI")
	TObjectPtr<UUserWidget> mCountdownTimerWidget;
	
	UPROPERTY(BlueprintReadWrite, Category="Player", Replicated)
	TObjectPtr<ABoardPlayer> mBoardPlayer;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Player")
	ECardPlayerState mCardPlayerState;
	/*최근 상호작용한 적의 인덱스. 다른 사람의 소유물 클릭시 해당 ui 출력용*/
	UPROPERTY(BlueprintReadWrite, Category="Player", ReplicatedUsing="OnRep_RecentOpponentIndex")
	int32 mRecentOpponentIndex;
	/** The max card the player can hold in their hand at any one time. Note: Not used when 'IgnoreMaxCards?' is enabled on the 'Request Card pickup' event. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player")
	int32 mMaxCardsInHand;
	/** The number of the cards the player will pick up when the game begins. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player")
	int32 mCardsInFirstHand;
	/** The number of cards the player can draw at the beginning of their turn.  */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Player")
	int32 mCardsToDrawPerTurn;
	UPROPERTY(BlueprintReadWrite, Category="Player", ReplicatedUsing="OnRep_bTurnActive")
	bool bTurnActive;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Card Placement")
	TObjectPtr<ACard3D> mPlayCard_Client;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Card Placement")
	TObjectPtr<ACardPlacement> mTargetCardPlacement;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Card Placement")
	TObjectPtr<UUserWidget> mCardWidget;
	/** Distance the player will hold a card from the camera when placing on the board */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement")
	double mCardHoldDistance;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Placement")
	bool bEnableInHandMovementRotation;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Card Placement")
	bool bIsValidClientDrop;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Mobile")
	TObjectPtr<ACard3D> mEnabledMobilePreview;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<ACCGState> mGameState;
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<ACCGPlayerState> mPlayerState;
	UPROPERTY(BlueprintReadWrite, Category="System", ReplicatedUsing="OnRep_TurnState")
	EGameTurn mTurnState;
	UPROPERTY(BlueprintReadWrite, Category="System", Replicated)
	int32 mPlayerNum;
	
	UPROPERTY(BlueprintReadWrite, Category="Deck", Replicated)
	TArray<FName> mPlayerDeck;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Deck")
	bool bShuffleDeck;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Deck")
	bool bEnableWeightedCards;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Temp Variables")
	FName tCreateCardName;
	UPROPERTY(BlueprintReadWrite, Category="Temp Variables")
	ECardSet tChosenCardSet;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Temp Variables")
	int32 tHandIndex;
	
	UPROPERTY(BlueprintReadWrite, Category="Card Manager", Replicated)
	TArray<FName> mCardsInHand;
	UPROPERTY(BlueprintReadWrite, Category="Card Manager", Replicated)
	FName mCardToAdd;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Card Manager")
	int32 mNumberOfCardsToAdd;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Manager")
	double mCardPickupDelay;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Developer", Replicated)
	bool bSkipManaCheck;

protected:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_bTurnActive();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_TurnState();
	UFUNCTION(BlueprintCallable)
	void OnRep_RecentOpponentIndex();

	UFUNCTION(Category="Delegate")
	void TouchBegin(ETouchIndex::Type FingerIndex, AActor* TouchedActor);
	UFUNCTION(Category="Delegate")
	void TouchEnd(ETouchIndex::Type FingerIndex, AActor* TouchedActor);
	UFUNCTION(Category="Delegate")
	void ClickBegin(AActor* TouchedActor , FKey ButtonPressed);
	UFUNCTION(Category="Delegate")
	void ClickEnd(AActor* TouchedActor , FKey ButtonPressed);

	UFUNCTION(Category="Macro")
	bool IsPlatformMobile() const;
	UFUNCTION(Category="Macro")
	void DisableCardSelection(ACard3D* ReceivingCard) const;
	UFUNCTION(Category="Macro")
	void ClearCardInteractionState();
	UFUNCTION(Category="Macro")
	bool ValidatePlacement(ACardPlacement* PlacementTarget,FCard Card) const;
	UFUNCTION(Category="Macro")
	bool ValidatePlacementOwner(int32 PlayerIndex,int32 PlacementPlayerIndex,FCard Card) const;

	void CardInteraction();
	
public:
	UFUNCTION(BlueprintCallable, Category="Card Functions")
	void SetCardLocation(ACard3D* Card, FVector HoldLocation, FRotator Rotation) const;
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Card Functions")
	bool ValidateCardPlacement(AActor* HitActor);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Card Functions")
	ACard3D* Client_CreatePlaceableCard(FName Name, ECardSet CardSet, FVector SpawnTransformLocation);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Card Functions")
	ACard3D* Server_CreatePlaceableCard(FTransform SpawnTransform);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Card Functions")
	ACardPlacement* ServerValidateCardPlacement(ACardPlacement* CardPlacement, FCard CardStruct) const;
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Card Functions")
	ACard3D* SetCustomCardData(ACard3D* Card, bool ActiveAbility) const;
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Card Functions")
	void PlayCard(FName CardName, ECardSet CardSet, ACardPlacement* CardPlacement, int32 CardHandIndex, FTransform SpawnTransform);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Card Interaction Functions")
	void DetectCardInteraction();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Card Interaction Functions")
	bool ValidateInteractionState() const;
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Card Interaction Functions")
	void DetectInteractionOnMove();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Card Interaction Functions")
	void RunCardInteraction(ACard3D* InteractionTalkingCard, ACard3D* InteractionReceivingCard, ABoardPlayer* InteractionReceivingPlayer);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Card Interaction Functions")
	bool CreateDragActorVisual(bool UseActorLocation);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Game Setup")
	void SetupGameUI();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Game Setup")
	void SetupDeck(FString DeckName, TArray<FName>& PlayerDeck);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Game Setup")
	void SetTimer(int32 Time);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Game Setup")
	FString LoadClientDeck(TArray<FName>& Deck) const;
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Deck / Hand  Functions")
	void ShufflePlayerDeck(TArray<FName>& TargetArray) const;
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Game")
	void NotifyCardTurnActive() const;
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Game")
	void NotifyCardsEndTurn() const;
	
	UFUNCTION(BlueprintCallable,Category="Event")
	void DragCanceled();

	UFUNCTION(BlueprintCallable,Server,Unreliable,Category="Server")
	void Server_SpawnAIOpponent();
	UFUNCTION(BlueprintCallable,Server,Unreliable,Category="Server")
	void Server_SetupDeck();
	UFUNCTION(BlueprintCallable,Server,Reliable,Category="Server")
	void Server_ReturnPlayerDeck(const FString& DeckName,const TArray<FName>& DeckArray);
	UFUNCTION(BlueprintCallable,Server,Reliable,Category="Server")
	void Server_RequestChangeTurnState();
	UFUNCTION(BlueprintCallable,Server,Reliable,Category="Server")
	void Server_UpdatePlayerHealth();
	UFUNCTION(BlueprintCallable,Server,Reliable,Category="Server")
	void Server_UpdatePlayerState();
	UFUNCTION(BlueprintCallable,Server,Reliable,Category="Server")
	void Server_PlayCard(FName CardName, ECardSet CardSet, ACardPlacement* CardPlacement, int32 CardHandIndex, FTransform SpawnTransform);
	UFUNCTION(BlueprintCallable,Server,Unreliable,Category="Server")
	void Server_RunCardInteraction(ACard3D* InteractionTalkingCard, ACard3D* InteractionReceivingCard, ABoardPlayer* InteractionReceivingPlayer);
	UFUNCTION(BlueprintCallable,Server,Unreliable,Category="Server")
	void Server_SetSkipManaCheck(bool SkipCheck);
	UFUNCTION(BlueprintCallable,Server,Unreliable,Category="Server")
	void Server_ReshuffleDeck();
	UFUNCTION(BlueprintCallable,Server,Unreliable,Category="Server")
	void Server_ClearCardsInHand();
	
	UFUNCTION(BlueprintCallable,Client,Reliable,Category="Client")
	void Client_PostLogin();
	UFUNCTION(BlueprintCallable,Client,Reliable,Category="Client")
	void Client_SetCountdownTimer(int32 Time);
	UFUNCTION(BlueprintCallable,Client,Reliable,Category="Client")
	void Client_EndMatchState(EEndGameResults Result);
	UFUNCTION(BlueprintCallable,Client,Reliable,Category="Client")
	void Client_UpdateGameUI(bool ForceCleanUpdate=false);
	UFUNCTION(BlueprintCallable,Client,Reliable,Category="Client")
	void Client_DropCard(bool CardPlayed);
	UFUNCTION(BlueprintCallable,Client,Unreliable,Category="Client")
	void Client_CreateDisplayMessage(const FString& Msg,FLinearColor Color, bool ToScreen, float ScreenDuration, bool ToMsgLogger=false);
	UFUNCTION(BlueprintCallable,Client,Reliable,Category="Client")
	void Client_LogNotificationMessage(const FString& Msg,FLinearColor Color);
	UFUNCTION(BlueprintCallable,Client,Reliable,Category="Client")
	void Client_GetPlayerDeck();
	UFUNCTION(BlueprintCallable,Client,Reliable,Category="Client")
	void Client_DestroyCard();

	UFUNCTION(BlueprintCallable,Server,Unreliable,Category="Card Manager")
	void Debug_Server_AddCardToHand(FName CardName);
	UFUNCTION(Category="Card Manager")
	void AddCardToHand();
	UFUNCTION(BlueprintCallable,Client,Reliable,Category="Card Manager")
	void Client_AddCardToCardManager(FName CardName,ECardSet FromCardSet);
	UFUNCTION(BlueprintCallable,Client,Reliable,Category="Card Manager")
	void Client_RemoveCardFromCardManager(UUserWidget* CardWidget,int32 CardHandIndex,bool RemoveAll=false);

	FORCEINLINE UUserWidget* GetPlayerGameUI(){return mPlayerGameUI;}
	FORCEINLINE TArray<FName> GetPlayerDeck(){return mPlayerDeck;}
	FORCEINLINE TArray<FName> GetCardsInHand(){return mCardsInHand;}
	FORCEINLINE int32 GetOpponentIndex() const {return mRecentOpponentIndex;}
	FORCEINLINE ABoardPlayer* GetBoardPlayer() const {return mBoardPlayer;}
};
