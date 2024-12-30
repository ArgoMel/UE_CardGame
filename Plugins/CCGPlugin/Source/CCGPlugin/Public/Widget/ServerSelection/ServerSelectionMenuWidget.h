// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerSelectionMenuWidget.generated.h"

class UMultiplayerSessionsSubsystem;
class UVerticalBox;
class UCircularThrobber;
class UScrollBox;

UCLASS(Abstract)
class CCGPLUGIN_API UServerSelectionMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UScrollBox> ServerSelectionSB;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCircularThrobber> LoadingThrobber;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> NoGamesFoundVB;

	TSubclassOf<UUserWidget> mSeverSelectionRowClass;
	TObjectPtr<UMultiplayerSessionsSubsystem> mMultiplayerSessionsSubsystem;
	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly,Category="ServerSelection")
	int32 mNumberOfSet;

protected:
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	
	void ResetWindowState() const;

public:
	UFUNCTION(BlueprintCallable,Category="Event")
	void RefreshServerList();
};
