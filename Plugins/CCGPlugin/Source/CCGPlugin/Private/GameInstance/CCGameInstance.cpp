// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstance/CCGameInstance.h"

#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "BFL/CCGBFL.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

UCCGameInstance::UCCGameInstance()
{
	mSelectedCardSet = TEXT("Creature Reborn");
	mCurGameState = ECardGameState::Startup;
	mChosenCardSet = ECardSet::BasicSet;
	mArena = EArenaList::ArenaClassic;
	mPlatform = EPlatform::Windows;
	bAIOpponent = false;

	mPathToLobby=TEXT("/CCGToolkit/Maps/ExampleArenas/");
	mNumPublicConnections=4;
	mMatchType=CCG_MatchType::Default;
}

void UCCGameInstance::Init()
{
	Super::Init();
	const FString curPlatformName = UGameplayStatics::GetPlatformName();
	for (const EPlatform platform : TEnumRange<EPlatform>())
	{
		FString platformName = *UEnum::GetDisplayValueAsText(platform).ToString();
		if(platformName == curPlatformName)
		{
			mPlatform = platform;
			break;
		}
	}
}

void UCCGameInstance::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		MoveToGameState(ECardGameState::Playing);
		UWorld* world = GetWorld();
		if (world)
		{
			world->ServerTravel(FString::Printf(TEXT("%s%s?listen"),*mPathToLobby,*UEnum::GetDisplayValueAsText(mArena).ToString()));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CCG 세션 생성 실패"));
		ShowMainMenuEvent();
	}
}

void UCCGameInstance::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	switch (Result)
	{
	case EOnJoinSessionCompleteResult::Success:
		MoveToGameState(ECardGameState::Playing);
		break;
	case EOnJoinSessionCompleteResult::SessionIsFull:
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
	case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
	case EOnJoinSessionCompleteResult::AlreadyInSession:
	case EOnJoinSessionCompleteResult::UnknownError:
		ShowMainMenuEvent();
		UE_LOG(LogTemp, Warning, TEXT("CCG 세션 참여 실패"));
		break;
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UCCGameInstance::OnDestroySession(bool bWasSuccessful)
{
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UCCGameInstance::OnStartSession(bool bWasSuccessful)
{
}

bool UCCGameInstance::MoveToGameState(ECardGameState InState)
{
	if(InState==mCurGameState)
	{
		return false;
	}
	const UWorld* world = GetWorld();
	IF_RET_BOOL(world);
	if (UCCGBFL::CanExecuteCosmeticEvents(GetWorld()))
	{
		switch (mCurGameState)
		{
		case ECardGameState::Startup:
			break;
		case ECardGameState::MainMenu:
			mMainMenu->RemoveFromParent();
			break;
		case ECardGameState::LoadingScreen:
			mLoadingScreen->RemoveFromParent();
			break;
		case ECardGameState::Playing:
			break;
		case ECardGameState::Store:
			break;
		case ECardGameState::DeckBuildingMenu:
			mMainMenu->RemoveFromParent();
			break;
		default: ;
		}
	}
	mCurGameState = InState;
	return true;
}

void UCCGameInstance::ShowMainMenuEvent_Implementation()
{
	const UWorld* world = GetWorld();
	IF_RET_VOID(world)
	
	if(mCurGameState==ECardGameState::Playing)
	{
		UGameplayStatics::OpenLevel(world, CCG_Level::MainMenu);
	}
	MoveToGameState(ECardGameState::MainMenu);
	if(UCCGBFL::CanExecuteCosmeticEvents(this))
	{
		if(!mMainMenu)
		{
			mMainMenu=CreateWidget<UUserWidget>(world->GetFirstPlayerController(), mMainMenuClass);
			mMainMenu->AddToViewport(CCG_ZOrder::MainMenu);
		}
	}
	UMultiplayerSessionsSubsystem* subsystem=GetSubsystem<UMultiplayerSessionsSubsystem>();
	if (subsystem)
	{
		subsystem->DestroySession();
	}
}

void UCCGameInstance::ShowLoadingScreen_Implementation()
{
	const UWorld* world = GetWorld();
	IF_RET_VOID(world)
	
	if (!MoveToGameState(ECardGameState::LoadingScreen))
	{
		return;	
	}
	if(!UCCGBFL::CanExecuteCosmeticEvents(world))
	{
		return;
	}
	if(!mLoadingScreen)
	{
		mLoadingScreen=CreateWidget<UUserWidget>(world->GetFirstPlayerController(), mLoadingScreenClass);
		mLoadingScreen->AddToViewport();
	}
}

void UCCGameInstance::HostGame_Implementation(EArenaList SelectedArena)
{
	ShowLoadingScreen();
	UMultiplayerSessionsSubsystem* subsystem=GetSubsystem<UMultiplayerSessionsSubsystem>();
	if (subsystem)
	{
		mArena=SelectedArena;
		subsystem->CreateSession(mNumPublicConnections,mMatchType);
	}
}

void UCCGameInstance::JoinGameSession(const FOnlineSessionSearchResult& Result)
{
	ShowLoadingScreen();
	UMultiplayerSessionsSubsystem* subsystem=GetSubsystem<UMultiplayerSessionsSubsystem>();
	if (subsystem)
	{
		subsystem->JoinSession(Result);
	}
}

void UCCGameInstance::Setup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	if (mCurGameState==ECardGameState::Startup)
	{
		mPathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
		mNumPublicConnections = NumberOfPublicConnections;
		mMatchType = TypeOfMatch;
		UMultiplayerSessionsSubsystem* subsystem = GetSubsystem<UMultiplayerSessionsSubsystem>(); 
		IF_RET_VOID(subsystem);  
		
		subsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);        
		subsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);      
		subsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		subsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);    
		
		ShowMainMenuEvent();
	}
}

bool UCCGameInstance::IsMobilePlatform() const
{
	return mPlatform==EPlatform::Android||mPlatform==EPlatform::Mac;
}

bool UCCGameInstance::IsSameGameState(ECardGameState InState) const
{
	return InState==mCurGameState;
}
