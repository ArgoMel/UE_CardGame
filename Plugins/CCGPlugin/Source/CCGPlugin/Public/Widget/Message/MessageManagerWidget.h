// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NotificationManagerWidget.h"
#include "MessageManagerWidget.generated.h"

class UVerticalBox;

UCLASS(Abstract)
class CCGPLUGIN_API UMessageManagerWidget : public UNotificationManagerWidget
{
	GENERATED_BODY()
public:
	explicit UMessageManagerWidget(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

protected:
	virtual FEventReply OnBorderClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnReverseAnimFinished() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> MessageVB;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	TSubclassOf<UUserWidget> mMessageClass;
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	int32 mMaxMessageDisplayCount;

private:
	FString AddGameTime(FString Message) const;

public:
	/*Create and add a message to add to the message manager */
	UFUNCTION(BlueprintCallable)
	void DisplayMessage(FString Message,FLinearColor Color,float Duration=5.f);
};
