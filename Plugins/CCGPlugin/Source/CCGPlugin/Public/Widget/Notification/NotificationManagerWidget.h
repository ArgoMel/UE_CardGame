// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationManagerWidget.generated.h"

class UTextBlock;
class UBorder;

USTRUCT(BlueprintType)
struct FMessageQueue
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString Message;
	UPROPERTY(EditAnywhere)
	FLinearColor Color;
	UPROPERTY(EditAnywhere)
	float Duration;

	FMessageQueue()
	: Color()
	, Duration(1.f)
	{
	}
};

UCLASS(Abstract)
class CCGPLUGIN_API UNotificationManagerWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

private:
	FWidgetAnimationDynamicEvent mForwardAnimFinishedDel;
	FWidgetAnimationDynamicEvent mReverseAnimFinishedDel;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBorder> MessageBorder;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MessageText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> OnLogBegin;
	
	TQueue<FMessageQueue> mNotificationQueue;
	TMap<FString,FLinearColor> mNotificationMap;
	FMessageQueue mRecentQueue;

protected:
	UFUNCTION(Category="Delegate")
	FEventReply OnBorderClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	UFUNCTION(Category="Delegate")
	void OnForwardAnimFinished();
	UFUNCTION(Category="Delegate")
	void OnReverseAnimStarted();
	UFUNCTION(Category="Delegate")
	void OnReverseAnimFinished();

	UFUNCTION(Category="Event")
	void LogMessage(FMessageQueue MessageQueue);
	
public:
	UFUNCTION(BlueprintCallable,Category="Event")
	void LogNotificationMessage(FString Message,FLinearColor Color,float Duration=1.f);
};
