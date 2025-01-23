// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerState/CCGPlayerState.h"

#include "CCGPlugin.h"
#include "GameMode/CCGMode.h"
#include "Net/UnrealNetwork.h"
#include "PlayerController/CCGPlayerController.h"

ACCGPlayerState::ACCGPlayerState()
: mCardGamePlayerId(0)
{
	SetNetUpdateFrequency(100.f);
	SetMinNetUpdateFrequency(3.f);
}

void ACCGPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, mPlayerStat);
	
	DOREPLIFETIME(ThisClass, mCardGamePlayerId);
}

void ACCGPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ACCGPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

int32 ACCGPlayerState::IncreasePlayerHealth_Implementation(int32 IncreaseBy)
{
	mPlayerStat.Health=FMath::Clamp(mPlayerStat.Health+IncreaseBy, 0, mPlayerStat.HealthMax);
	return mPlayerStat.Health;
}

int32 ACCGPlayerState::DecreasePlayerHealth_Implementation(int32 DecreaseBy)
{
	mPlayerStat.Health=FMath::Clamp(mPlayerStat.Health-DecreaseBy, 0, mPlayerStat.HealthMax);
	return mPlayerStat.Health;
}

int32 ACCGPlayerState::IncreasePlayerMana_Implementation(int32 IncreaseBy)
{
	mPlayerStat.Mana=FMath::Clamp(mPlayerStat.Mana+IncreaseBy, 0, mPlayerStat.ManaMax);
	return mPlayerStat.Mana;
}

int32 ACCGPlayerState::DecreasePlayerMana_Implementation(int32 DecreaseBy)
{
	mPlayerStat.Mana=FMath::Clamp(mPlayerStat.Mana-DecreaseBy, 0, mPlayerStat.ManaMax);
	return mPlayerStat.Mana;
}

int32 ACCGPlayerState::GetManaForTurn_Implementation()
{
	IF_RET_BOOL(mGameMode);
	mPlayerStat.Mana=mGameMode->GetTurnMana(mOwningPlayerController);
	mPlayerStat.ManaMax=mGameMode->GetTurnMana(mOwningPlayerController);
	return mPlayerStat.Mana;
}

void ACCGPlayerState::OnRep_PlayerStat()
{
	UpdateUI();
	if (mPlayerStat.Health<0||
		!mPlayerStat.CheckCardState())
	{
		Server_NotifyPlayerStateChange();
	}
}

void ACCGPlayerState::UpdateUI()
{
	IF_RET_VOID(mOwningPlayerController);
	mOwningPlayerController->Client_UpdateGameUI(mOwningPlayerController->PlayerState!=this&&mPlayerStat.CardsInHand==0);
}

void ACCGPlayerState::Init(int32 PlayerID, ACCGPlayerController* PlayerController, ACCGMode* GameMode)
{
	mCardGamePlayerId=PlayerID;
	mOwningPlayerController=PlayerController;
	mGameMode=GameMode;
	InitAttributes();
}

void ACCGPlayerState::InitAttributes()
{
	if (HasAuthority())
	{
		mPlayerStat.Mana=0;
		mPlayerStat.Health=FMath::Clamp(mGameMode->mCardGameOption.PlayerStartingHealth,0,mPlayerStat.HealthMax);
	}
}

void ACCGPlayerState::UpdatePlayersCardStates(int32 CardsInHand, int32 CardsInDeck, int32 ActiveCards)
{
	if (HasAuthority())
	{
		mPlayerStat.CardsInHand=CardsInHand;
		mPlayerStat.CardsInDeck=CardsInDeck;
		mPlayerStat.ActiveCard=ActiveCards;
	}
}

void ACCGPlayerState::Server_NotifyPlayerStateChange_Implementation()
{
	IF_RET_VOID(mGameMode);
	if (!mGameMode->IsGameActive())
	{
		return;
	}
	if (mPlayerStat.Health>0&&
		mPlayerStat.CheckCardState())
	{
		return;
	}
	mGameMode->CheckPlayerState();
}

void ACCGPlayerState::Multicast_ForceUpdateUI_Implementation()
{
	UpdateUI();
}
