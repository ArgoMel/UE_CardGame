// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Message/MessageManagerWidget.h"

#include "CCGPlugin.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "GameState/CCGState.h"
#include "Interface/WidgetInterface.h"

UMessageManagerWidget::UMessageManagerWidget(const FObjectInitializer& ObjectInitializer)
: mMaxMessageDisplayCount(12)
{
	GetClassAsset(mMessageClass,UUserWidget,"/CCGPlugin/CCGToolkit/BP/Widgets/Message/WBP_Message.WBP_Message_C")
}

void UMessageManagerWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UMessageManagerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (MessageVB)
	{
		MessageVB->ClearChildren();
	}
}

FEventReply UMessageManagerWidget::OnBorderClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MessageBorder)
	{
		MessageBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
	return FEventReply();
}

void UMessageManagerWidget::OnReverseAnimFinished()
{
	UnbindFromAnimationFinished(OnLogBegin, mReverseAnimFinishedDel);
	if (MessageBorder)
	{
		MessageBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FString UMessageManagerWidget::AddGameTime(FString Message) const
{
	const UWorld* world = GetWorld();
	IF_RET(FString(),world);
	const ACCGState* gameState=Cast<ACCGState>(world->GetGameState());
	IF_RET(FString(),gameState);
	const int32 gameTime=gameState->GetGameTimeSecond();
	return FString::Printf(TEXT("[%i:%i] %s"), gameTime/60,gameTime%60,*Message);
}

void UMessageManagerWidget::DisplayMessage(FString Message, FLinearColor Color, float Duration)
{
	IF_RET_VOID(MessageVB);

	if (MessageVB->GetChildrenCount()>=mMaxMessageDisplayCount)
	{
		MessageVB->RemoveChildAt(0);
	}
	UUserWidget* messageWidget=CreateWidget(GetOwningPlayer(),mMessageClass);
	if (messageWidget)
	{
		Message=AddGameTime(Message);
		if (messageWidget->Implements<UWidgetInterface>())
		{
			IWidgetInterface::Execute_DisplayMessage(messageWidget,Message,Color,Duration);
		}
		MessageVB->AddChild(messageWidget);
	}
	const UWorld* world = GetWorld();
	IF_RET_VOID(world);
	FTimerHandle tempTH;
	world->GetTimerManager().SetTimer(tempTH,this,&ThisClass::OnReverseAnimStarted,Duration);
}
