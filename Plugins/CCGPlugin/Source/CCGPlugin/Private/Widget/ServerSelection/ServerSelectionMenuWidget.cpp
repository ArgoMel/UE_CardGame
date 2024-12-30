// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/ServerSelection/ServerSelectionMenuWidget.h"

#include "CCGPlugin.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Components/CircularThrobber.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Widget/ServerSelection/ServerSelectionRowWidget.h"

void UServerSelectionMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	const UGameInstance* gameInstance = GetGameInstance();
	IF_RET_VOID(gameInstance);
	mMultiplayerSessionsSubsystem = gameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	IF_RET_VOID(mMultiplayerSessionsSubsystem);
	mMultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UServerSelectionMenuWidget::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	LoadingThrobber->SetVisibility(ESlateVisibility::Collapsed);

	if (!bWasSuccessful)
	{
		NoGamesFoundVB->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		return;
	}

	for (auto& session : SessionResults)
	{
		UServerSelectionRowWidget* widget=CreateWidget<UServerSelectionRowWidget>(GetOwningPlayer(),mSeverSelectionRowClass);
		widget->Init(session);
		ServerSelectionSB->AddChild(widget);
	}
}

void UServerSelectionMenuWidget::ResetWindowState() const
{
	ServerSelectionSB->ClearChildren();
	LoadingThrobber->SetVisibility(ESlateVisibility::Collapsed);
	NoGamesFoundVB->SetVisibility(ESlateVisibility::Collapsed);
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void UServerSelectionMenuWidget::RefreshServerList()
{
	ResetWindowState();

	IF_RET_VOID(mMultiplayerSessionsSubsystem);
	mMultiplayerSessionsSubsystem->FindSessions(mNumberOfSet);

	LoadingThrobber->SetVisibility(ESlateVisibility::HitTestInvisible);
}
