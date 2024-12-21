// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "Common/CCGStruct.h"
#include "GameFramework/GameStateBase.h"
#include "CCGState.generated.h"

class AGraveyard;
class ACard3D;
struct FPlayerBoard;
class ACardPlacement;

UCLASS()
class CCGPLUGIN_API ACCGState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ACCGState();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
// 	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", meta=(EditInline="true"))
// 	TObjectPtr<UCardManager_C> CardSystem_Ref;
//
// 	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
// 	TObjectPtr<ABP_CardGameMode_C> CardGameMode_Ref;
//
// 	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
// 	TEnumAsByte<Platform_Enum> Platform;
//
// 	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="GameManager")
// 	int32 CountdownTimer;
//
// 	/** Please add a variable description */
// 	static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGameState, GameTime_Seconds) to GetLifetimeReplicatedProps");
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="GameManager", Replicated)
// 	int32 GameTime_Seconds;
//
// 	/** Please add a variable description */
// 	static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGameState, GameTime_Minutes) to GetLifetimeReplicatedProps");
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Game Manager", Replicated)
// 	int32 GameTime_Minutes;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Turn Manager", Replicated)
	EGameTurn mGameTurnState;

// 	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Turn Manager")
// 	FTimerHandle TurnTimer_Ref;
//
// 	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Turn Manager")
// 	int32 TurnDuration_Seconds;
//
// 	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Turn Manager")
// 	int32 TurnDuration_Minutes;
//
// 	/** Please add a variable description */
// 	static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGameState, TurnTime_Seconds) to GetLifetimeReplicatedProps");
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Turn Manager", Replicated)
// 	int32 TurnTime_Seconds;
//
// 	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Game Manager")
// 	FTimerHandle GameTimer_Ref;
//
// 	/** Please add a variable description */
// 	static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGameState, TurnTime_Minutes) to GetLifetimeReplicatedProps");
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Turn Manager", Replicated)
// 	int32 TurnTime_Minutes;
//
// 	/** Seconds the timer will tick. 1 = 1 second. 0.1 = 10 times a second. */
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="System")
// 	double GameSeconds;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Board", Replicated)
	TArray<FPlayerBoard> mPlayerBoards;

	/** 인덱스 0이 현재 플레이어 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	TArray<AController*> mPlayerTurns;

// 	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Recorded Game Data")
// 	TArray<FGameStateSnapshot_Struct> GameSnapshot;
//
// 	/** Please add a variable description */
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Recorded Game Data")
// 	TArray<FBattleHistory_Struct> BattleHistory;
	
// 	/** Please add a variable description */
// 	static_assert(false, "You will need to add DOREPLIFETIME(ABP_CardGameState, bGameActive) to GetLifetimeReplicatedProps");
// 	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
// 	bool bGameActive;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="System")
	bool EnableBattleHistory;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System", Replicated)
	TArray<AActor*> mPlayerAndAIStates;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Recorded Game Data")
	TArray<ACard3D*> mCardReferenceArray;
	
public:
// 	UFUNCTION(BlueprintCallable, Category="System")
// 	void GameTimer();
//
// 	/** Please add a function description */
// 	UFUNCTION(BlueprintCallable, Category="System")
// 	void TurnTimer();
//
// 	/** Please add a function description */
// 	UFUNCTION(BlueprintCallable, Category="System")
// 	void RestetTurnTimer();
//
// 	/** Please add a function description */
// 	UFUNCTION(BlueprintPure, Category="System")
// 	void CheckPlayerTurnState(TEnumAsByte<GameTurn_Enum> GameTurnState, bool& ActiveTurn);
//
// 	/** Please add a function description */
// 	UFUNCTION(BlueprintPure, Category="System")
// 	void RequestChangeTurnState(AController* Controller, bool& ChangeRequestValid);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Board")
	void AddCardToBoard(ACard3D* CardReference, int32 PlayerID);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Board")
	void RemoveCardOnBoard(ACard3D* CardReference, int32 PlayerID);

// 	/** Please add a function description */
// 	UFUNCTION(BlueprintPure, Category="Board")
// 	void GetBoardState(int32 PlayerID, TArray<A3DCard_C*>& PlayerCards, TArray<A3DCard_C*>& OppenedCards);
//
// 	/** Please add a function description */
// 	UFUNCTION(BlueprintCallable, Category="Board")
// 	void CompilePlacementsPerPlayer();
//
// 	/** Please add a function description */
// 	UFUNCTION(BlueprintPure, Category="Board")
// 	void GetCardPlacementReferences(int32 PlayerID, TArray<ACardPlacement_C*>& PlacementArray, int32& Total);
//
// 	/** Please add a function description */
// 	UFUNCTION(BlueprintCallable, Category="Gameplay")
// 	void RotatePlayerTurn();
//
// 	/** Please add a function description */
// 	UFUNCTION(BlueprintCallable, Category="Gameplay")
// 	void EndOfPlayerTurn(int32 PlayerID);
//
// 	/** Please add a function description */
// 	UFUNCTION(BlueprintCallable, Category="Gameplay")
// 	void BeginPlayerTurn(int32 PlayerID);
//
// 	/** Please add a function description */
// 	UFUNCTION(BlueprintCallable, Category="Recorded Game Data")
// 	void RecordGameStateSnapshot();
//
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Recorded Game Data")
	void RecordBattleHistory(FBattleHistory BattleHistoryStruct);

// 	/** Please add a function description */
// 	UFUNCTION(BlueprintCallable, Category="Board")
// 	void GetGraveyardReferencesPerPlayer();

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category="Board")
	AGraveyard* GetGraveyardReference(int32 PlayerID);

	void GetAllPlayerCards(TArray<ACard3D*>& Cards);
	void GetActivePlayerCards(int32 Index, TArray<ACard3D*>& Cards);
	void GetCardPlacements(int32 Index, TArray<ACardPlacement*>& CardPlacements);

	UFUNCTION(Server, Unreliable)
	void OnGameStart();
	void Server_NotifyEndGameState(TArray<EEndGameResults>& array);

	FORCEINLINE AController* GetCurPlayerTurn() { return mPlayerTurns[0]; }
};
