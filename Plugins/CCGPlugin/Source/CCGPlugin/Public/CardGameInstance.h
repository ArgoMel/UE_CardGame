// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <CCGPlugin.h>
#include "Engine/GameInstance.h"
#include "CardGameInstance.generated.h"

class UMainMenuWidget;
class ULoadingScreenWidget;
class UDeckBuilderWidget;

UCLASS()
class CCGPLUGIN_API UCardGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UCardGameInstance();
protected:
	virtual void Init() override;

protected:
	/** Main Menu UMG Blueprint reference */
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> mMainMenuClass;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Widgets")
	TObjectPtr<UMainMenuWidget> mMainMenu;
	/** Loading Screen UMG Blueprint reference */
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> mLoadingScreenClass;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Widgets")
	TObjectPtr<ULoadingScreenWidget> mLoadingScreen;
	/** Please add a variable description */
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> mDeckBuilderClass;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Widgets")
	TObjectPtr<UDeckBuilderWidget> mDeckBuilder;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "디폴트")
	TArray<FName> mDeck;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "디폴트")
	FString mSelectedCardSet;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "디폴트")
	EGameState mCurGameState;
	/** Holds an enum reference to the chosen deck */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "디폴트")
	ECardSet mChosenCardSet;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "디폴트")
	EArenaList mArena;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "디폴트")
	EPlatform mPlatform;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "디폴트")
	bool bEnableLAN;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "디폴트")
	bool bAIOpponent;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ShowMainMenuEvent();
	void ShowMainMenuEvent_Implementation();

	/** 같은 상태면 false를 반환 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool MoveToGameState(EGameState InState);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanExecuteCosmeticEvents(const UObject* WorldContextObject) const;

	///** Please add a function description */
	//UFUNCTION(BlueprintCallable)
	//void StartGame();

	///** Please add a function description */
	//UFUNCTION(BlueprintPure)
	//void GetCurrentPlatform(EPlatform& Platform, bool& Mobile);

	///** Please add a function description */
	//UFUNCTION(BlueprintPure)
	//void GetGameState(TEnumAsByte<GameState_Enum> InState, TEnumAsByte<GameState_Enum>& CurrentState, bool& In State);
};
