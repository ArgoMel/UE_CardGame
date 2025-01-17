// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "Common/CCGStruct.h"
#include "GameFramework/GameStateBase.h"
#include "CCGState.generated.h"

class ACCGMode;
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
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	/** 인덱스 0이 현재 플레이어 */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TArray<AController*> mPlayerTurns;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<ACCGMode> mCardGameMode;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System", Replicated)
	bool bGameActive;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	bool bEnableBattleHistory;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Game Manager")
	FTimerHandle mGameTH;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Game Manager", Replicated)
	int32 mGameTimeSecond;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Turn Manager")
	FTimerHandle mTurnTH;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Turn Manager")
	int32 mTurnDurationSecond;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Turn Manager", Replicated)
	int32 mTurnTimeSecond;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Board", Replicated)
	TArray<FPlayerBoard> mPlayerBoards;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Recorded Game Data")
	TArray<FGameStateSnapshot> mGameSnapshot;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Recorded Game Data")
	TArray<FBattleHistory> mBattleHistory;

public:
	UPROPERTY(BlueprintReadWrite, Category="System", Replicated)
	TArray<AActor*> mPlayerAndAIStates;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="Recorded Game Data")
	TArray<ACard3D*> mCardReferenceArray;

protected:
	void ResetTurn();
	
public:
	UFUNCTION(BlueprintCallable, Category="System")
	void GameTimer();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="System")
	void TurnTimer();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="System")
	void ResetTurnTimer();
	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category="System")
	bool RequestChangeTurnState(AController* Controller);
	/** ai만 사용 */
	UFUNCTION(BlueprintPure, Category="System")
	int32 GetRandomOpponentIndex(AActor* ExceptionController=nullptr) const;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Board")
	void AddCardToBoard(ACard3D* CardReference, int32 PlayerID);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Board")
	void RemoveCardOnBoard(ACard3D* CardReference, int32 PlayerID);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Board")
	void CompilePlacementsPerPlayer();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Board")
	void GetGraveyardReferencesPerPlayer();
	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category="Board")
	AGraveyard* GetGraveyardReference(int32 PlayerID);
	UFUNCTION(BlueprintPure, Category="Board")
	int32 GetTotalCardPlacementPos(int32 PlayerID);
	UFUNCTION(BlueprintPure, Category="Board")
	void GetAllPlayerCards(TArray<ACard3D*>& Cards);
	UFUNCTION(BlueprintPure, Category="Board")
	void GetActivePlayerCards(int32 Index, TArray<ACard3D*>& Cards);
	UFUNCTION(BlueprintPure, Category="Board")
	void GetCardPlacements(int32 Index, TArray<ACardPlacement*>& CardPlacements);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void RotatePlayerTurn();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void EndOfPlayerTurn(int32 PlayerID);
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void BeginPlayerTurn(int32 PlayerID);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Recorded Game Data")
	void RecordGameStateSnapshot();
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Recorded Game Data")
	void RecordBattleHistory(FBattleHistory BattleHistoryStruct);

	UFUNCTION(Server, Unreliable)
	void Server_OnGameStart();
	UFUNCTION(Server, Reliable)
	void Server_RequestChangeTurnState(AController* Controller);
	UFUNCTION(Server, Unreliable)
	void Server_ForceChangeTurnState();
	UFUNCTION(Server, Reliable)
	void Server_NotifyEndGameState(const TArray<EEndGameResults>& array);

	FORCEINLINE AController* GetCurPlayerTurn() { return mPlayerTurns[0]; }
	FORCEINLINE int32 GetGameTimeSecond() const { return mGameTimeSecond; }
};
