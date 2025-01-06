// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGStruct.h"
#include "GameFramework/GameMode.h"
#include "CCGMode.generated.h"

class ACCGPlayerController;
class AAIController;
class ABoardPlayer;
class ACCGState;

UCLASS()
class CCGPLUGIN_API ACCGMode : public AGameMode
{
	GENERATED_BODY()
public:
	ACCGMode();
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle mCountdownTH;

protected:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TObjectPtr<ACCGState> mGameState;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	int32 mCountdownTimer;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	double mGameSeconds;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="System")
	bool bSkipStartTimer;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	bool bGameActive;

public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TArray<AActor*> mPlayerAndAIStates;
	UPROPERTY(BlueprintReadWrite, Category="System")
	TArray<ACCGPlayerController*> mGameControllersArray;
	//TArray<AController*> mGameControllersArray;
	UPROPERTY(BlueprintReadWrite, Category="System")
	TArray<AAIController*> mAIControllersArray;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TArray<ABoardPlayer*> mBoardPlayersArray;

	/** 아레나셀렉션에서 컨트롤러를 통해서 설정 */
	UPROPERTY(BlueprintReadWrite, Category="Game Mode Settings")
	FCardGameOption mCardGameOption;

protected:
	virtual void CollectGameResults(TArray<EEndGameResults>& PlayerResults);
	bool CheckIsPlayerActive(int32 ControllerID) const;
	void CreateAIPawn();
	
	UFUNCTION(Category="Delay")
	void FinishCountdown();
	
public:
	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category="Gameplay")
	virtual int32 CalculateManaForTurn(int32 PlayerTurn);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category="Gameplay")
	int32 GetTurnMana(AController* Controller);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void SetBoardPlayerReference();

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category="System")
	void GetPlayerControllers(TArray<ACCGPlayerController*>& Players) const;
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="System")
	void SetCardGamePlayerId(AController* Controller);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="System")
	AController* AddPlayerToArray(AActor* PlayerState, AController* PlayerController);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="System")
	void SetBoardPlayerReferences(AController* Controller);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="System")
	void RemovePlayerFromGame(AController* Controller);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="AIOpponent")
	void CreateCardGameAIOpponent();

	UFUNCTION(Blueprintable,BlueprintNativeEvent)
	void CheckGamePreConditions();

	UFUNCTION(Blueprintable,BlueprintNativeEvent)
	void ForceSpawnAIOpponent();

	UFUNCTION(Blueprintable,BlueprintNativeEvent)
	void GameStartCountdown();
	
	UFUNCTION(Blueprintable,BlueprintNativeEvent)
	void CheckPlayerState();

	UFUNCTION(Blueprintable,BlueprintNativeEvent)
	void EndGame();

	FORCEINLINE bool IsGameActive() const { return bGameActive; }
	FORCEINLINE double GetGameSecond() const { return mGameSeconds; }
};
