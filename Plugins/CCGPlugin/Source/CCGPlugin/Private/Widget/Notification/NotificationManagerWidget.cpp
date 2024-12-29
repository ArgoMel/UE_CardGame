// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/Notification/NotificationManagerWidget.h"

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

// ReSharper disable once CppMemberFunctionMayBeConst
FEventReply UNotificationManagerWidget::OnBorderClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MessageBorder)
	{
		MessageBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
	return FEventReply();
}

void UNotificationManagerWidget::OnForwardAnimFinished()
{
	UnbindFromAnimationFinished(OnLogBegin, mForwardAnimFinishedDel);
	const UWorld* world = GetWorld();
	IF_RET_VOID(world);
	FTimerHandle tempTH;
	world->GetTimerManager().SetTimer(tempTH,this,&ThisClass::OnReverseAnimStarted,mRecentQueue.Duration);
}

void UNotificationManagerWidget::OnReverseAnimStarted()
{
	BindToAnimationFinished(OnLogBegin, mReverseAnimFinishedDel);
	PlayAnimationReverse(OnLogBegin);
}

void UNotificationManagerWidget::OnReverseAnimFinished()
{
	UnbindFromAnimationFinished(OnLogBegin, mReverseAnimFinishedDel);
	MessageBorder->SetVisibility(ESlateVisibility::Collapsed);
	mNotificationMap.Remove(mRecentQueue.Message);

	if (!mNotificationQueue.IsEmpty())
	{
		mNotificationQueue.Dequeue(mRecentQueue);
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
	if (mNotificationMap.Contains(Message))
	{
		return;
	}
	FMessageQueue messageQueue;
	messageQueue.Message=Message;
	messageQueue.Duration=Duration;
	messageQueue.Color=Color;
	mNotificationQueue.Enqueue(messageQueue);
	mNotificationMap.Add(Message,Color);

	if (MessageBorder->GetVisibility()==ESlateVisibility::Collapsed)
	{
		mNotificationQueue.Dequeue(mRecentQueue);
		LogMessage(mRecentQueue);
	}
}
