// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Message/NotificationManagerWidget.h"

#include "CCGPlugin.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UNotificationManagerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (MessageBorder)
	{
		MessageBorder->OnMouseButtonDownEvent.BindDynamic(this,&ThisClass::OnBorderClicked);
	}
	
	mForwardAnimFinishedDel.BindDynamic(this, &ThisClass::OnForwardAnimFinished);
	mReverseAnimFinishedDel.BindDynamic(this, &ThisClass::OnReverseAnimFinished);
}

void UNotificationManagerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (MessageBorder)
	{
		MessageBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FEventReply UNotificationManagerWidget::OnBorderClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	UnbindFromAnimationFinished(OnLogBegin, mForwardAnimFinishedDel);
	StopAnimation(OnLogBegin);
	const UWorld* world = GetWorld();
	IF_RET(FEventReply(),world);
	world->GetTimerManager().ClearTimer(mNotificationTH);
	OnReverseAnimFinished();
	return FEventReply();
}

void UNotificationManagerWidget::OnForwardAnimFinished()
{
	UnbindFromAnimationFinished(OnLogBegin, mForwardAnimFinishedDel);
	const UWorld* world = GetWorld();
	IF_RET_VOID(world);
	world->GetTimerManager().ClearTimer(mNotificationTH);
	world->GetTimerManager().SetTimer(mNotificationTH,this,&ThisClass::OnReverseAnimStarted,mRecentQueue.Duration);
}

void UNotificationManagerWidget::OnReverseAnimStarted()
{
	BindToAnimationFinished(OnLogBegin, mReverseAnimFinishedDel);
	PlayAnimationReverse(OnLogBegin);
}

void UNotificationManagerWidget::OnReverseAnimFinished()
{
	UnbindFromAnimationFinished(OnLogBegin, mReverseAnimFinishedDel);
	if (MessageBorder)
	{
		MessageBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
	mMessageMap.Remove(mRecentQueue.Message);

	if (!mMessageQueue.IsEmpty())
	{
		mMessageQueue.Dequeue(mRecentQueue);
		LogMessage(mRecentQueue);
	}
}

void UNotificationManagerWidget::LogMessage(FMessageQueue MessageQueue)
{
	MessageBorder->SetVisibility(ESlateVisibility::Visible);
	MessageText->SetText(FText::FromString(MessageQueue.Message));
	MessageText->SetColorAndOpacity(MessageQueue.Color);

	BindToAnimationFinished(OnLogBegin, mForwardAnimFinishedDel);
	PlayAnimation(OnLogBegin);
}

void UNotificationManagerWidget::LogNotificationMessage(FString Message, FLinearColor Color, float Duration)
{
	if (mMessageMap.Contains(Message))
	{
		return;
	}
	FMessageQueue messageQueue;
	messageQueue.Message=Message;
	messageQueue.Duration=Duration;
	messageQueue.Color=Color;
	mMessageQueue.Enqueue(messageQueue);
	mMessageMap.Add(Message,Color);

	if (MessageBorder->GetVisibility()==ESlateVisibility::Collapsed)
	{
		mMessageQueue.Dequeue(mRecentQueue);
		LogMessage(mRecentQueue);
	}
}
