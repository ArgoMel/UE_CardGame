// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerState/CCGPlayerState.h"

#include "Net/UnrealNetwork.h"

ACCGPlayerState::ACCGPlayerState()
: mMaxCardsInDeck(0)
, bForceUIUpdateOnReplication(false)
, mCardGamePlayerId(0)
{
}

void ACCGPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, mPlayerStat);

	DOREPLIFETIME(ThisClass, mCardGamePlayerId);
	DOREPLIFETIME(ThisClass, bForceUIUpdateOnReplication);
}

void ACCGPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ACCGPlayerState::OnRep_PlayerStat()
{
}

void ACCGPlayerState::OnRep_CardsInHandArray()
{
}

void ACCGPlayerState::InitAttributes()
{
}

void ACCGPlayerState::InitMana()
{
}

void ACCGPlayerState::InitHealth()
{
}

void ACCGPlayerState::UpdatePlayersCardStates(int32 CardsInHand, int32 CardsInDeck, int32 ActiveCards)
{
}

void ACCGPlayerState::SetMana(int32 Mana)
{
}

ACard3D* ACCGPlayerState::CreatePlayableCard(FTransform SpawnTransform)
{
	return nullptr;
}

bool ACCGPlayerState::AddCardToPlayersHand(FName CardName)
{
	return false;
}

void ACCGPlayerState::Server_NotifyPlayerStateChange_Implementation()
{
}

void ACCGPlayerState::Multicast_ForceUpdateUI_Implementation()
{
}
