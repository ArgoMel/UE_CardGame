// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "ServerSelectionRowWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS(Abstract)
class CCGPLUGIN_API UServerSelectionRowWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

private:
	FOnlineSessionSearchResult mSearchResult;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> JoinBtn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> Name;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurNumPlayerText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxNumPlayerText;

protected:
	UFUNCTION()
	void OnJoinBtnClicked();

public:
	void Init(const FOnlineSessionSearchResult& Result);
};
