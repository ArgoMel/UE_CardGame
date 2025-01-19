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
	virtual int32 GetCurrentPlayerIndex_Implementation() const override;

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
	TObjectPtr<UDataTable> mAIPersonalityData;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Asset")
	TObjectPtr<UDataTable> mAIPointAllocationData;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Asset")
	TSubclassOf<ACard3D> mCard3DClass;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Asset")
	TSubclassOf<UUserWidget> mAIBarClass;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="AIPersonality")
	FPointAllocation mAIPointAllocation;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="AIPersonality")
	FAIPersonality mAIPersonality;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<ACCGMode> mGameMode;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<ACCGState> mGameState;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<ACCGAIPawn> mControlledPawn;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<ABoardPlayer> mBoardPlayer;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	int32 mMaxCardsInHand;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	bool bShuffleDeck;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	bool bTurnActive;
	/** ONLY ON NONE DEDICATED SERVERS */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	bool bShowDebugUI;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	bool bEnableWeightedCards;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Card Manager")
	int32 mNumberOfCardsToAdd;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Manager")
	int32 mCardsInFirstHand;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Card Manager")
	double mCardPickupDelay;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="AIPlayer")
	TObjectPtr<UUserWidget> mAIBarWidget;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Play Data")
	TArray<FName> mPlayableCardsInHandList;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Play Data")
	TArray<ACard3D*> mCardsOnBoardWithTurnPoints;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Play Data")
	FEvaluationState mEvaluationStateTotal;

	/** AIInfoBar에서 사용 */
	UPROPERTY(BlueprintReadWrite, Category="Priority")
	TArray<EPriority> mPriorityArray;
	/** AIInfoBar에서 사용 */
	UPROPERTY(BlueprintReadWrite, Category="Priority")
	TArray<float> mPriorityValueArray;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Priority")
	double mHealth_Low;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Priority")
	double mCardsInHand_Low;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Priority")
	double mBoardState_Low;

public:
	UPROPERTY(BlueprintReadWrite, Category="Card Manager", ReplicatedUsing="OnRep_AICardsInHand")
	TArray<FName> mAICardsInHand;
	
	UPROPERTY(BlueprintReadWrite, Category="AIPlayer", Replicated)
	TArray<FName> mAIDeck;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Play Data")
	TArray<ECardType> mValidInteractionTypes;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Priority")
	TArray<EAIPersonalityFocus> mPriorityFocusList;

private:
	void StartDrawCard(FName CardName);
	FAIState GetAISate(int32 PlayerIndex) const;
	
protected:
	/** Please add a function description */
	UFUNCTION()
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
	void UpdateAIPlayerState() const;
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
	void CreateAIDebugUI();
	
	FORCEINLINE FAIPersonality* GetAIPersonality() {return &mAIPersonality;}
	FORCEINLINE FPointAllocation* GetAIPointAllocation() {return &mAIPointAllocation;}
	FORCEINLINE ABoardPlayer* GetBoardPlayer() {return mBoardPlayer;}
};
