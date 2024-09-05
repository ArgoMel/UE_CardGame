// Fill out your copyright notice in the Description page of Project Settings.

#include "CardGameInstance.h"
#include "Widget/Menu/MainMenuWidget.h"
#include "Widget/Menu/LoadingScreenWidget.h"
#include "Widget/Menu/DeckBuilder/DeckBuilderWidget.h"
#include <Kismet/GameplayStatics.h>

UCardGameInstance::UCardGameInstance()
{
	GetClassAsset(mMainMenuClass, UUserWidget,"");
	GetClassAsset(mLoadingScreenClass, UUserWidget,"");
	GetClassAsset(mDeckBuilderClass, UUserWidget,"");

	mSelectedCardSet = TEXT("Creature Reborn");
	mCurGameState = EGameState::Startup;
	mChosenCardSet = ECardSet::BasicSet;
	mArena = EArenaList::ArenaClassic;
	mPlatform = EPlatform::Windows;
	bEnableLAN = true;
	bAIOpponent = false;
}

void UCardGameInstance::Init()
{
	Super::Init();
	FString curPlatformName = UGameplayStatics::GetPlatformName();
	for (EPlatform platform : TEnumRange<EPlatform>())
	{
		FString platformName = *UEnum::GetDisplayValueAsText(platform).ToString();
		if(platformName == curPlatformName)
		{
			mPlatform = platform;
			break;
		}
	}
}

void UCardGameInstance::ShowMainMenuEvent_Implementation()
{
	if(mCurGameState==EGameState::Playing)
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
	}
	MoveToGameState(EGameState::MainMenu);
	if(CanExecuteCosmeticEvents(GetWorld()))
	{
		if(!IsValid(mMainMenu))
		{
			mMainMenu=CreateWidget<UMainMenuWidget>(UGameplayStatics::GetPlayerController(GetWorld(),0), mMainMenuClass);
			mMainMenu->AddToViewport(5);
		}
	}
	//destroysession
}

bool UCardGameInstance::MoveToGameState(EGameState InState)
{
	if(InState==mCurGameState)
	{
		return false;
	}
	if (CanExecuteCosmeticEvents(GetWorld()))
	{
		switch (mCurGameState)
		{
		case EGameState::Startup:
			break;
		case EGameState::MainMenu:
			mMainMenu->RemoveFromParent();
			break;
		case EGameState::LoadingScreen:
			mLoadingScreen->RemoveFromParent();
			break;
		case EGameState::Playing:
			break;
		case EGameState::Store:
			break;
		case EGameState::DeckBuildingMenu:
			mDeckBuilder->RemoveFromParent();
			break;
		}
	}
	mCurGameState = InState;
	return true;
}

bool UCardGameInstance::CanExecuteCosmeticEvents(const UObject* WorldContextObject) const
{
	return !UKismetSystemLibrary::IsDedicatedServer(GetWorld());
}
