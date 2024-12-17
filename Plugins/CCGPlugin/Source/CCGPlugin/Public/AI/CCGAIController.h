// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "CCGAIController.generated.h"

class ACard3D;
class ACCGState;
class ACCGMode;
class ABoardPlayer;
class ACCGAIPawn;

UCLASS()
class CCGPLUGIN_API ACCGAIController : public AAIController
{
	GENERATED_BODY()
public:
	ACCGAIController();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPersonality")
	TArray<EAIPersonalityFocus> mPersonalityFocuses;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPersonality")
	FName mDefaultPersonality;
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Manager", ReplicatedUsing="OnRep_AICardsInHand")
	TArray<FName> mAICardsInHand;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Manager")
	FName mCardToAdd;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Manager")
	int32 mNumberOfCardsToAdd;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Manager")
	int32 mCardsInFirstHand;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Card Manager")
	double mCardPickupDelay;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPlayer", Replicated)
	TArray<FName> mAIDeck;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPlayer")
	TArray<FName> mAIPlayableDecks;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AIPlayer")
	TArray<FName> mTempDeck;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트")
	TObjectPtr<UBlackboardComponent> mAIBlackboard;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="디폴트")
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Play Data")
	TArray<ECardType> mValidInteractionTypes;
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Priority | Arrays")
	TArray<EAIPersonalityFocus> mActivePriorityFocusList;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Priority |Arrays")
	TArray<EAIPersonalityFocus> mPriorityFocusList;
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
	
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Black Board Keys")
	// FName mPlaySpeedKey;
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Black Board Keys")
	// FName mAwarenessKey;
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Black Board Keys")
	// FName Difficulty;
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Black Board Keys")
	// FName Risk;
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Black Board Keys")
	// FName TurnState;
	// /** Please add a variable description */
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Black Board Keys")
	// FName TurnActive;

protected:
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void OnRep_AICardsInHand();
	
public:
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Gameplay")
	// void AIPlayCard(FName CardName, TEnumAsByte<CardSet_Enum> CardSet, bool& PlayCardValid);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Setup")
	// void LoadAIPersonality(FName RowName);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Setup")
	// void SetupAI();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Setup")
	// void SetupAIDeck();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="AI State")
	// void UpdateAIPlayerState();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="AI State")
	// void EvaluateBoardState(bool& Success);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="AI State")
	// void UpdateAIState();
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Gameplay")
	// void RunCardInteraction(const FCardInteraction_Struct& AICardInteraction_Struct, bool& Success);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintPure, Category="AI State")
	// void GetFocusPriority(TEnumAsByte<AIPersonalityFocus_Enum> Focus, TEnumAsByte<Priority_Enum>& ReturnPriority, double& ReturnPriorityValue, TEnumAsByte<Priority_Enum> Priority, double PriorityValue);
	//
	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="AI State")
	// void CalculatePriorityFocus();

	// /** Please add a function description */
	// UFUNCTION(BlueprintCallable, Category="Setup")
	// void CreateAIDebugUI();

	FORCEINLINE FPointAllocation GetAIPointAllocation() const {return mAIPointAllocation;}
	FORCEINLINE TArray<FName> GetAIDeck(){return mAIDeck;}
	FORCEINLINE TArray<FName> GetAICardsInHand(){return mAICardsInHand;}
};
