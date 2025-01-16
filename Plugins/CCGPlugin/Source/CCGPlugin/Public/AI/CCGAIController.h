// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "Interface/CardsInHandInterface.h"
#include "Interface/ControllerInterface.h"
#include "Interface/DeckInterface.h"
#include "Interface/GameStateInterface.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "CCGAIController.generated.h"

class ACard3D;
class ACCGState;
class ACCGMode;
class ABoardPlayer;
class ACCGAIPawn;

UCLASS()
class CCGPLUGIN_API ACCGAIController : public AAIController
	,public ICardsInHandInterface
	,public IControllerInterface
	,public IDeckInterface
	,public IGameStateInterface
{
	GENERATED_BODY()
public:
	friend ACCGMode;
	ACCGAIController();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	/** ICardsInHandInterface */
	virtual bool AddCardToPlayersHand_Implementation(FName CardName) override;
	virtual bool RemoveCardFromHand_Implementation(FName Card, int32 Index, bool RemoveAll) override;
	virtual void DrawCard_Implementation(FName CardName, bool IgnoreMaxCards, int32 NumberOfCardsToDraw) override;

	/** IControllerInterface */
	virtual ACard3D* CreatePlayableCard_Implementation(FTransform SpawnTransform) override;
	virtual UMaterialInterface* GetProfileImg_Implementation() override;

	/** IDeckInterface */
	virtual void GetPlayerDeck_Implementation(TArray<FName>& Deck) override;
	virtual bool RemoveCardFromDeck_Implementation(bool RemoveAll, int32 IndexToRemove) override;

	/** IGameStateInterface */
	virtual void MatchEnd_Implementation(EEndGameResults Result) override;
	virtual void MatchBegin_Implementation() override;
	virtual void ChangeActivePlayerTurn_Implementation(bool TurnActive) override;
	virtual void RequestChangePlayerTurn_Implementation() override;

private:
	FTimerHandle mDrawCardTH;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Asset")
	TObjectPtr<UBlackboardData> mBlackBoardData;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Asset")
	TObjectPtr<UBehaviorTree> mBehaviorTree;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Asset")
	TSubclassOf<ACard3D> mCard3D;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPersonality")
	TArray<EAIPersonalityFocus> mPersonalityFocuses;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPersonality")
	FName mPersonalityName;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPersonality")
	FPointAllocation mAIPointAllocation;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPersonality")
	FAIPersonality mAIPersonality;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPersonality")
	EAIDifficulty mDifficulty;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPersonality")
	EAIRisk mRisk;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPersonality")
	int32 mAwareness;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPersonality")
	double mPlaySpeed;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TObjectPtr<ACCGMode> mGameMode;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TObjectPtr<ACCGState> mGameState;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TObjectPtr<ACCGAIPawn> mControlledPawn;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TObjectPtr<ABoardPlayer> mBoardPlayer;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	FName mSelectedDeck;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="System")
	int32 mMaxCardsInHand;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	bool bAIReady;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	bool bShuffleDeck;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	bool bPrintAIDebugLogs;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	bool bTurnActive;
	/** ONLY ON NONE DEDICATED SERVERS */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="System")
	bool bShowDebugUI;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="System")
	bool bEnableWeightedCards;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Manager")
	int32 mNumberOfCardsToAdd;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Manager")
	int32 mCardsInFirstHand;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Manager")
	double mCardPickupDelay;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPlayer")
	TArray<FName> mTempDeck;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="AIPlayer")
	TObjectPtr<UUserWidget> mAIBarWidget;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Play Data")
	TArray<FPlaySubStruct> mPlayList;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Play Data")
	TArray<FName> mPlayableCardsInHandList;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Play Data")
	TArray<FCardInteraction> mValidPlaysForTurn;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Play Data")
	TArray<ACard3D*> mCardsOnBoardWithTurnPoints;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Play Data")
	TArray<EAIPersonalityFocus> tAIFocusPriorities;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Play Data")
	FEvaluationState mEvaluationStateTotal;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Play Data")
	FAIState mEvaluationStateSelf;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Play Data")
	FAIState EvaluationStateOpponent;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Priority |Arrays")
	TArray<EPriority> mPriorityList;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Priority |Arrays")
	TArray<EPriority> tPriorityList;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Priority |Arrays")
	TArray<EPriority> tPriorityArray;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Priority |Arrays")
	TArray<double> mPriorityValues;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Priority | StateValues")
	double mHealth_Low;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Priority | StateValues")
	double mCardsInHand_Low;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Priority | StateValues")
	double mBoardState_Low;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Manager", ReplicatedUsing="OnRep_AICardsInHand")
	TArray<FName> mAICardsInHand;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPlayer")
	TArray<FName> mAIPlayableDecks;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPlayer", Replicated)
	TArray<FName> mAIDeck;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Play Data")
	TArray<ECardType> mValidInteractionTypes;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Priority |Arrays")
	TArray<EAIPersonalityFocus> mPriorityFocusList;

private:
	void StartDrawCard(FName CardName);
	
protected:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_AICardsInHand();

	UFUNCTION(BlueprintCallable, Category="Timer")
	void AddCardToHand(FName CardName);
	
public:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	bool AIPlayCard(FName CardName, ECardSet CardSet);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	bool RunCardInteraction(const FCardInteraction& AICardInteraction);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="AI State")
	void UpdateAIPlayerState();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="AI State")
	bool EvaluateBoardState();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="AI State")
	void UpdateAIState();
	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category="AI State")
	EPriority GetFocusPriority(EAIPersonalityFocus Focus, float& ReturnPriorityValue);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="AI State")
	void CalculatePriorityFocus();
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Setup")
	void LoadAIPersonality(FName RowName);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Setup")
	void SetupAI();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Setup")
	void SetupAIDeck();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Setup")
	void CreateAIDebugUI();

	FORCEINLINE ABoardPlayer* GetBoardPlayer() const {return mBoardPlayer;};
	FORCEINLINE FPointAllocation* GetAIPointAllocation() {return &mAIPointAllocation;}
};
