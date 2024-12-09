// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstance/CardGameInstance.h"

#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "BFL/CCGBFL.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

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
	bAIOpponent = false;

	mPathToLobby=TEXT("/CCGToolkit/Maps/ExampleArenas/");
	mNumPublicConnections=4;
	mMatchType=CCG_MatchType::Default;
}

void UCardGameInstance::Init()
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

void UCardGameInstance::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		MoveToGameState(EGameState::Playing);
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

// ReSharper disable once CppMemberFunctionMayBeConst
void UCardGameInstance::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	UMultiplayerSessionsSubsystem* subsystem=GetSubsystem<UMultiplayerSessionsSubsystem>();
	if (!subsystem)
	{
		return;
	}
	
	for (auto& result : SessionResults)
	{
		FString settingsValue;
		result.Session.SessionSettings.Get(FName("MatchType"), settingsValue);
		if (settingsValue == mMatchType)
		{
			subsystem->JoinSession(result);
			return;
		}
	}
}

void UCardGameInstance::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	switch (Result)
	{
	case EOnJoinSessionCompleteResult::Success:
		MoveToGameState(EGameState::Playing);
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
void UCardGameInstance::OnDestroySession(bool bWasSuccessful)
{
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UCardGameInstance::OnStartSession(bool bWasSuccessful)
{
}

bool UCardGameInstance::MoveToGameState(EGameState InState)
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
		default: ;
		}
	}
	mCurGameState = InState;
	return true;
}

void UCardGameInstance::ShowMainMenuEvent_Implementation()
{
	const UWorld* world = GetWorld();
	IF_RET_VOID(world)
	
	if(mCurGameState==EGameState::Playing)
	{
		UGameplayStatics::OpenLevel(world, CCG_Level::MainMenu);
	}
	MoveToGameState(EGameState::MainMenu);
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

void UCardGameInstance::ShowLoadingScreen_Implementation()
{
	const UWorld* world = GetWorld();
	IF_RET_VOID(world)
	
	if (!MoveToGameState(EGameState::LoadingScreen))
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

void UCardGameInstance::HostGame_Implementation(EArenaList SelectedArena)
{
	ShowLoadingScreen();
	UMultiplayerSessionsSubsystem* subsystem=GetSubsystem<UMultiplayerSessionsSubsystem>();
	if (subsystem)
	{
		mArena=SelectedArena;
		subsystem->CreateSession(mNumPublicConnections,mMatchType);
	}
}

void UCardGameInstance::JoinGameSession_Implementation()
{
	ShowLoadingScreen();
	UMultiplayerSessionsSubsystem* subsystem=GetSubsystem<UMultiplayerSessionsSubsystem>();
	if (subsystem)
	{
		subsystem->FindSessions(10000);
	}
}

void UCardGameInstance::Setup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	if (mCurGameState==EGameState::Startup)
	{
		mPathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
		mNumPublicConnections = NumberOfPublicConnections;
		mMatchType = TypeOfMatch;
		UMultiplayerSessionsSubsystem* subsystem = GetSubsystem<UMultiplayerSessionsSubsystem>(); 
		IF_RET_VOID(subsystem);  
		
		subsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);        
		subsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);    
		subsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);      
		subsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		subsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);    
		
		ShowMainMenuEvent();
	}
}

EPlatform UCardGameInstance::GetCurrentPlatform(bool& IsMobile) const
{
	IsMobile=mPlatform==EPlatform::Android||mPlatform==EPlatform::Mac;
	return mPlatform;
}

EGameState UCardGameInstance::GetGameState(EGameState InState, bool& IsSameState) const
{
	IsSameState=InState==mCurGameState;
	return mCurGameState;
}
