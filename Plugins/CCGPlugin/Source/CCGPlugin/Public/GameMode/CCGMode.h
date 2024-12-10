// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CCGEnum.h"
#include "GameFramework/GameMode.h"
#include "CCGMode.generated.h"

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
	TArray<AActor*> mPlayerStateArray;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	TArray<ABoardPlayer*> mBoardPlayersArray;
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="System")
	bool bSkipStartTimer;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, Category="System")
	bool bGameActive;
	
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Game Mode Settings")
	int32 mMana_Min;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Game Mode Settings")
	int32 mMana_Max;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Game Mode Settings")
	int32 mPlayerStartingHealth;
	/** Currently only 2 player is supported by the game mode */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Game Mode Settings")
	int32 mMaxNumOfPlayers;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Game Mode Settings")
	bool bSingleClientEnabled;
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Game Mode Settings")
	bool bSpectator;

public:
	UPROPERTY(BlueprintReadWrite, Category="System")
	TArray<AController*> mGameControllersArray;

protected:
	void CollectGameResults(TArray<EEndGameResults>& PlayerResults);
	bool CheckIsPlayerActive(int32 ControllerID);

	UFUNCTION(Category="Delay")
	void FinishCountdown();
	
public:
	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category="Gameplay")
	int32 CalculateManaForTurn(int32 PlayerTurn);

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category="Gameplay")
	int32 GetTurnMana(AController* Controller);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	void SetBoardPlayerReference();

	/** Please add a function description */
	UFUNCTION(BlueprintPure, Category="System")
	void GetPlayerControllers(TArray<AController*>& Players);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="System")
	void SetCardGamePlayerId(UObject* PlayerState, AController* Controller);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="System")
	AController* AddPlayerToArray(AActor* PlayerState, AController* PlayerController);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, Category="System")
	void SetBoardPlayerReferences(UPARAM(ref) AController*& Controller);

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
};
