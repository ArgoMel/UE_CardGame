// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <CCGPlugin.h>
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CCGameInstance.generated.h"

UCLASS()
class CCGPLUGIN_API UCCGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UCCGameInstance();
protected:
	virtual void Init() override;
	
protected:
	/** Main Menu UMG Blueprint reference */
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> mMainMenuClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	TObjectPtr<UUserWidget> mMainMenu;
	/** Loading Screen UMG Blueprint reference */
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> mLoadingScreenClass;
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	TObjectPtr<UUserWidget> mLoadingScreen;

	/*덱셀렉션 메뉴에서 설정하고 플레이어컨트롤러에서 클라이언트덱 받아올때 사용*/
	UPROPERTY(BlueprintReadWrite, Category = "디폴트")
	FString mSelectedCardSet;
	UPROPERTY(BlueprintReadWrite, Category = "디폴트")
	ECardGameState mCurGameState;
	UPROPERTY(BlueprintReadWrite, Category = "디폴트")
	EArenaList mArena;
	UPROPERTY(BlueprintReadOnly, Category = "디폴트")
	EPlatform mPlatform;

public:
	UPROPERTY(BlueprintReadOnly, Category = "OnlineSetting")
	FString mPathToLobby;
	UPROPERTY(BlueprintReadWrite, Category = "OnlineSetting")
	FString mMatchType;
	UPROPERTY(BlueprintReadWrite, Category = "OnlineSetting")
	int32 mNumPublicConnections;

protected:
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);

	/** 같은 상태면 false를 반환 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool MoveToGameState(ECardGameState InState);

public:
	UFUNCTION(BlueprintCallable,Category="Event")
	void ShowMainMenuEvent();
	UFUNCTION(BlueprintCallable,Category="Event")
	void ShowLoadingScreen();
	UFUNCTION(BlueprintCallable,Category="Event")
	void HostGame(EArenaList SelectedArena);
	
	void JoinGameSession(const FOnlineSessionSearchResult& Result);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void Setup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = TEXT("Default"), FString LobbyPath = FString(TEXT("/CCGToolkit/Maps/ExampleArenas/Test")));

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	bool IsMobilePlatform() const;

	/** 현재 스테이트 반환 */
	UFUNCTION(BlueprintPure)
	bool IsSameGameState(ECardGameState InState) const;

	FORCEINLINE ECardGameState GetCurGameState() const { return mCurGameState; }
	FORCEINLINE EPlatform GetPlatform() const { return mPlatform; }
	FORCEINLINE FString GetSelectedCardSet() const { return mSelectedCardSet; }
};