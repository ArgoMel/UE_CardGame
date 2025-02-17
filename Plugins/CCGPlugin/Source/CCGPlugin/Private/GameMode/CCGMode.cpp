// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/CCGMode.h"

#include "CCGPlugin.h"
#include "AI/CCGAIController.h"
#include "AI/CCGAIPawn.h"
#include "BFL/ControllerBFL.h"
#include "Common/CCGConstVar.h"
#include "GameFramework/PlayerState.h"
#include "Gameplay/BoardPlayer.h"
#include "GameState/CCGState.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/CCGPlayerController.h"
#include "PlayerState/CCGPlayerState.h"
#include "SaveGame/CCGSG.h"

ACCGMode::ACCGMode()
: mCountdownTimer(4)
, mGameSeconds(1)
, bSkipStartTimer(true)
, bGameActive(false)
{
	bDelayedStart=true;
}

void ACCGMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	const UWorld* world = GetWorld();
	IF_RET_VOID(world);
	/*agg_test_MainMenu 이런식으로 출력 */
	if (world->GetMapName().Contains(CCG_Level::MainMenu.ToString()))
	{
		return;
	}
	
	if (UGameplayStatics::DoesSaveGameExist(CCG_SaveSlotName::CardGameSave,0))
	{
		const UCCGSG* saveGame=Cast<UCCGSG>(UGameplayStatics::LoadGameFromSlot(CCG_SaveSlotName::CardGameSave,0));
		if (saveGame)
		{
			mCardGameOption=saveGame->mCardGameOption;
		}
	}
	
	if (mGameControllersArray.Num()>=mCardGameOption.MaxNumOfPlayers||
		mCardGameOption.bSpectator)
	{
		SetBoardPlayerReference();
	}
	else
	{
		AController* controller= AddPlayerToArray(NewPlayer->PlayerState,NewPlayer);
		SetCardGamePlayerId(controller);
		SetBoardPlayerReference();
		SetBoardPlayerReferences(controller);
		ACCGPlayerController* playerController=Cast<ACCGPlayerController>(controller);
		if (playerController)
		{
			playerController->Client_PostLogin();
			playerController->Server_SetupDeck();
		}
	}
	CheckGamePreConditions();
}

void ACCGMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	RemovePlayerFromGame(Exiting);
}

void ACCGMode::BeginPlay()
{
	Super::BeginPlay();
}

void ACCGMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!mGameState)
	{
		mGameState=GetGameState<ACCGState>();
		if (mGameState)
		{
			mGameState->mPlayerAndAIStates=mPlayerAndAIStates;
		}
	}
}

void ACCGMode::CollectGameResults(TArray<EEndGameResults>& PlayerResults)
{
	int32 victory=0;
	int32 defeat=0;
	for (int32 i=0;i<mGameControllersArray.Num();++i)
	{
		if (CheckIsPlayerActive(i+1))
		{
			PlayerResults.Add(EEndGameResults::Victory);
			++victory;
		}
		else
		{
			PlayerResults.Add(EEndGameResults::Defeat);
			++defeat;
		}
	}
	if (defeat==0||victory==0)
	{
		PlayerResults.Init(EEndGameResults::Draw,mGameControllersArray.Num());
	}
}

bool ACCGMode::CheckIsPlayerActive(int32 ControllerID) const
{
	FPlayerStat playerStat;
	UControllerBFL::GetControllersStateStat(this,ControllerID,playerStat);
	return playerStat.Health>0&&
		(playerStat.CardsInHand>0||playerStat.CardsInDeck>0||playerStat.ActiveCard>0);
}

void ACCGMode::CreateAIPawn()
{
	IF_RET_VOID(mAIPawnClass);
	FActorSpawnParameters spawnParams;
	const FVector spawnLocation=mBoardPlayersArray[mGameControllersArray.Num()]->GetActorLocation();
	spawnParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ACCGAIPawn* AIPawn=GetWorld()->SpawnActor<ACCGAIPawn>(mAIPawnClass,spawnLocation,FRotator::ZeroRotator,spawnParams);
	IF_RET_VOID(AIPawn);

	AddPlayerToArray(AIPawn,AIPawn->GetController());
	SetBoardPlayerReferences(AIPawn->GetController());
	AIPawn->mPlayerID=mGameControllersArray.Num();
}

void ACCGMode::FinishCountdown()
{
	GetWorldTimerManager().ClearTimer(mCountdownTH);
	bGameActive=true;
	IF_RET_VOID(mGameState);
	mGameState->Server_OnGameStart();
}

int32 ACCGMode::CalculateManaForTurn(int32 PlayerTurn)
{
	return FMath::Clamp(PlayerTurn+mCardGameOption.Mana.X,mCardGameOption.Mana.X,mCardGameOption.Mana.Y);
}

int32 ACCGMode::GetTurnMana(AController* Controller)
{
	const int32 id=UControllerBFL::GetControllerID(Controller);
	FPlayerStat playerStat;
	UControllerBFL::GetControllersStateStat(this,id,playerStat);
	return CalculateManaForTurn(playerStat.PlayerTurn);
}

void ACCGMode::SetBoardPlayerReference()
{
	mBoardPlayersArray.Empty();
	TArray<AActor*> allBoard;
	UGameplayStatics::GetAllActorsOfClass(this,ABoardPlayer::StaticClass(),allBoard);
	mBoardPlayersArray.Init(nullptr,allBoard.Num());
	for (const auto& board : allBoard)
	{
		ABoardPlayer* boardPlayer=Cast<ABoardPlayer>(board);
		if (boardPlayer)
		{
			mBoardPlayersArray[boardPlayer->mPlayerIndex]=boardPlayer;
		}
	}
}

void ACCGMode::GetPlayerControllers(TArray<ACCGPlayerController*>& Players) const
{
	Players=mGameControllersArray;
}

void ACCGMode::SetCardGamePlayerId(AController* Controller)
{
	ACCGPlayerController* playerController=Cast<ACCGPlayerController>(Controller);
	IF_RET_VOID(playerController);
	ACCGPlayerState* playerState=Controller->GetPlayerState<ACCGPlayerState>();
	IF_RET_VOID(playerState);
	playerState->Init(mGameControllersArray.Num(),playerController,this);
}

AController* ACCGMode::AddPlayerToArray(AActor* PlayerState, AController* PlayerController)
{
	ACCGPlayerController* playerController=Cast<ACCGPlayerController>(PlayerController);
	if (playerController)
	{
		mGameControllersArray.AddUnique(playerController);
	}
	mPlayerAndAIStates.AddUnique(PlayerState);
	if (mGameState)
	{
		mGameState->mPlayerAndAIStates=mPlayerAndAIStates;
	}
	return PlayerController;
}

void ACCGMode::SetBoardPlayerReferences(AController* Controller)
{
	ACCGPlayerController* playerController=Cast<ACCGPlayerController>(Controller);
	ABoardPlayer* boardPlayer=mBoardPlayersArray[mGameControllersArray.Find(playerController)];
	if (playerController)
	{
		playerController->mBoardPlayer=boardPlayer;
		boardPlayer->SetOwner(playerController);
	}
	else
	{
		ACCGAIController* AIController=Cast<ACCGAIController>(Controller);
		if (AIController)
		{
			AIController->mBoardPlayer=boardPlayer;
			boardPlayer->SetOwner(AIController);
			mAIControllersArray.Add(AIController);
		}
	}
}

void ACCGMode::RemovePlayerFromGame(AController* Controller)
{
	ACCGPlayerController* playerController=Cast<ACCGPlayerController>(Controller);
	if (playerController)
	{
		mGameControllersArray.Remove(playerController);
	}
	mPlayerAndAIStates.Remove(Controller);
	if (mGameState)
	{
		mGameState->mPlayerAndAIStates=mPlayerAndAIStates;
	}
}

void ACCGMode::CreateCardGameAIOpponent()
{
	if (mGameControllersArray.Num()>=mCardGameOption.MaxNumOfPlayers)
	{
		return;
	}
	if (mCardGameOption.bSpectator)
	{
		CreateAIPawn();
	}
	CreateAIPawn();
}

void ACCGMode::CheckGamePreConditions_Implementation()
{
	if (mGameControllersArray.Num()<mCardGameOption.MaxNumOfPlayers)
	{
		if (!mCardGameOption.bSingleClientEnabled)
		{
			return;
		}
		for (int32 i=mGameControllersArray.Num();i<mCardGameOption.MaxNumOfPlayers;++i)
		{
			ForceSpawnAIOpponent();
		}
	}
	GetWorldTimerManager().SetTimer(mCountdownTH,this,&ThisClass::GameStartCountdown,mGameSeconds,!bSkipStartTimer);
}

void ACCGMode::ForceSpawnAIOpponent_Implementation()
{
	CreateCardGameAIOpponent();
	GetWorldTimerManager().SetTimer(mCountdownTH,this,&ThisClass::GameStartCountdown,mGameSeconds,!bSkipStartTimer);
}

void ACCGMode::GameStartCountdown_Implementation()
{
	if (!bSkipStartTimer)
	{
		--mCountdownTimer;
		for (const auto& gameController:mGameControllersArray)
		{
			gameController->Client_SetCountdownTimer(mCountdownTimer);
		}
	}
	if (mCountdownTimer<1||
		bSkipStartTimer)
	{
		FinishCountdown();
	}
}

void ACCGMode::CheckPlayerState_Implementation()
{
	for (int32 i=1;i<=mCardGameOption.MaxNumOfPlayers;++i)
	{
		if (!CheckIsPlayerActive(i))
		{
			EndGame();
			break;
		}
	}
}

void ACCGMode::EndGame_Implementation()
{
	bGameActive=false;
	TArray<EEndGameResults> playerResults;
	CollectGameResults(playerResults);
	mGameState->Server_NotifyEndGameState(playerResults);
}
