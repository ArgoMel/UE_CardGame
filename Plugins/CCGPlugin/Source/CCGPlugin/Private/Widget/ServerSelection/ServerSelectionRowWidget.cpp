// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/ServerSelection/ServerSelectionRowWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameInstance/CCGameInstance.h"
#include "GameState/CCGState.h"
#include "Interface/ControllerInterface.h"

void UServerSelectionRowWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	JoinBtn->OnClicked.AddDynamic(this,&ThisClass::OnJoinBtnClicked);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UServerSelectionRowWidget::OnJoinBtnClicked()
{
	UCCGameInstance* gameInstance=Cast<UCCGameInstance>(GetGameInstance());
	IF_RET_VOID(gameInstance);
	gameInstance->JoinGameSession(mSearchResult);
}

void UServerSelectionRowWidget::Init(const FOnlineSessionSearchResult& Result)
{
	mSearchResult=Result;
	Name->SetText(FText::FromString(mSearchResult.Session.OwningUserName));
	MaxNumPlayerText->SetText(FText::AsNumber(mSearchResult.Session.NumOpenPublicConnections));

	//서버에서 현재 인원수 받아오기
	const UWorld* world=GetWorld();
	IF_RET_VOID(world);
	const ACCGState* gameState=world->GetGameState<ACCGState>();
	IF_RET_VOID(gameState);
	const int32 curPlayerNum=gameState->mPlayerAndAIStates.Num();
	CurNumPlayerText->SetText(FText::AsNumber(curPlayerNum));
}
